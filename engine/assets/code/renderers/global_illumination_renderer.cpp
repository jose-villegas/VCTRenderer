#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "global_illumination_renderer.h"

#include "voxelizer_renderer.h"
#include "deferred_renderer.h"
#include "../programs/gi_program.h"
#include "../../../scene/camera.h"
#include "../../../scene/scene.h"

#include <oglplus/context.hpp>
#include <oglplus/bound/texture.hpp>
#include "../../../core/assets_manager.h"
#include "../../../rendering/render_window.h"

GIRenderer::GIRenderer(RenderWindow &window) : Renderer(window),
    maxTracingDistance(1), globalIlluminationStrength(1),
    ambientOcclusionFalloff(800), ambientOcclusionAlpha(0)
{
    SetupFramebuffer(window.Info().width / 2, window.Info().height / 2);
}

GIRenderer::~GIRenderer()
{
}

void GIRenderer::Render()
{
    using namespace oglplus;
    static Context gl;
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();

    if (!camera || !scene || !scene->IsLoaded() || VoxelizerRenderer::ShowVoxels)
    {
        return;
    }

    SetAsActive();
    // indirect lighting program
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<GIProgram *>
                        (assets->programs["GlobalIllumination"].get());
    static auto &composite = *static_cast<FinalCompositionProgram *>
                             (assets->programs["Composite"].get());
    static auto &deferred = *static_cast<DeferredRenderer *>
                            (assets->renderers["Deferred"].get());
    static auto &voxel = *static_cast<VoxelizerRenderer *>(AssetsManager::Instance()
                         ->renderers["Voxelizer"].get());
    // render to fbo
    giFramebuffer.Bind(FramebufferTarget::Draw);
    gl.ColorMask(true, true, true, true);
    gl.Viewport(renderSize.x, renderSize.y);
    gl.Clear().ColorBuffer().DepthBuffer();
    // light pass shader
    CurrentProgram<GIProgram>(prog);
    // pass gi program uniforms
    auto &textures = deferred.BufferTextures();
    prog.cameraPosition.Set(camera->Position());
    prog.inverseProjectionView.Set(camera->InverseViewMatrix() *
                                   camera->InverseProjectionMatrix());

    for (int i = 0; i < textures.size(); i++)
    {
        textures[i].Active(i);
        textures[i].Bind(TextureTarget::_2D);
    }

    prog.gNormal.Set(0);
    prog.gAlbedo.Set(1);
    prog.gSpecular.Set(2);
    prog.gDepth.Set(3);
    prog.volumeDimension.Set(voxel.VolumeDimension());
    prog.voxelScale.Set(1.0f / voxel.VolumeGridSize());
    prog.worldMinPoint.Set(scene->rootNode->boundaries.MinPoint());
    voxel.VoxelTexture().Active(7);
    voxel.VoxelTexture().Bind(TextureTarget::_3D);
    voxel.VoxelTextureMipmap().Active(8);
    voxel.VoxelTextureMipmap().Bind(TextureTarget::_3D);
    prog.voxelTex.Set(7);
    prog.voxelTexMipmap.Set(8);
    // global illum setup
    prog.maxTracingDistanceGlobal.Set(maxTracingDistance);
    prog.bounceStrength.Set(globalIlluminationStrength);
    prog.aoFalloff.Set(ambientOcclusionFalloff);
    prog.aoAlpha.Set(ambientOcclusionAlpha);
    prog.enableAo.Set(true);
    // draw the result onto a fullscreen quad
    fsQuad.Draw();
    // default fbo
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
    // recover viewport
    gl.Viewport(Window().Info().width, Window().Info().height);
    gl.ColorMask(true, true, true, true);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate composition program
    CurrentProgram<FinalCompositionProgram>(composite);
    // pass textures for final composition
    deferred.DirectLightPass().Active(0);
    deferred.DirectLightPass().Bind(TextureTarget::_2D);
    indirectPass.Active(1);
    indirectPass.Bind(TextureTarget::_2D);
    // draw the result onto a fullscreen quad
    fsQuad.Draw();
}

const float &GIRenderer::MaxTracingDistance() const
{
    return maxTracingDistance;
}

void GIRenderer::MaxTracingDistance(const float &val)
{
    maxTracingDistance = val;
}

const float &GIRenderer::GlobalIlluminationStrength() const
{
    return globalIlluminationStrength;
}

void GIRenderer::GlobalIlluminationStrength(const float &val)
{
    globalIlluminationStrength = val;
}

const float &GIRenderer::AmbientOclussionFalloff() const
{
    return ambientOcclusionFalloff;
}

void GIRenderer::AmbientOclussionFalloff(const float &val)
{
    ambientOcclusionFalloff = val;
}

const float &GIRenderer::AmbientOclussionAlpha() const
{
    return ambientOcclusionAlpha;
}

void GIRenderer::AmbientOclussionAlpha(const float &val)
{
    ambientOcclusionAlpha = val;
}

oglplus::Texture &GIRenderer::IndirectLightPass()
{
    return indirectPass;
}

void GIRenderer::GlobalIlluminationScale(const float &val)
{
    using namespace oglplus;
    static Context gl;
    auto cVal = glm::clamp(val, 0.0f, 1.0f);
    int width = Window().Info().width * cVal;
    int height = Window().Info().width * cVal;
    renderSize = glm::uvec2(width, height);
    gl.Bound(TextureTarget::_2D, indirectPass)
    .Image2D(0, PixelDataInternalFormat::RGBA8, width, height,
             0, PixelDataFormat::RGBA, PixelDataType::UnsignedByte, nullptr)
    .MinFilter(TextureMinFilter::Linear)
    .MagFilter(TextureMagFilter::Linear);
}

void GIRenderer::SetupFramebuffer(unsigned int width, unsigned int height)
{
    using namespace oglplus;
    static Context gl;
    renderSize = glm::uvec2(width, height);
    // initialize indirect light storage fbo
    giFramebuffer.Bind(FramebufferTarget::Draw);
    // build textures -- normal
    gl.Bound(TextureTarget::_2D, indirectPass)
    .Image2D(0, PixelDataInternalFormat::RGBA8, width, height,
             0, PixelDataFormat::RGBA, PixelDataType::UnsignedByte, nullptr)
    .MinFilter(TextureMinFilter::Linear)
    .MagFilter(TextureMagFilter::Linear);
    giFramebuffer.AttachColorTexture(FramebufferTarget::Draw, 0,
                                     indirectPass, 0);
    gl.DrawBuffer(FramebufferColorAttachment::_0);

    // check if success building frame buffer
    if (!Framebuffer::IsComplete(FramebufferTarget::Draw))
    {
        auto status = Framebuffer::Status(FramebufferTarget::Draw);
        Framebuffer::HandleIncompleteError(FramebufferTarget::Draw, status);
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}
