#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shadow_map_renderer.h"

#include "../scene/light.h"
#include "../programs/depth_program.h"
#include "../../../scene/scene.h"
#include "../../../core/assets_manager.h"

#include <oglplus/context.hpp>
#include <oglplus/bound/texture.hpp>

void ShadowMapRenderer::SetMatricesUniforms(const Node &node) const
{
    auto &prog = CurrentProgram<DepthProgram>();
    static auto &camera = Camera::Active();
    prog.matrices.modelViewProjection.Set(camera->ProjectionMatrix() *
                                          camera->ViewMatrix() *
                                          node.ModelMatrix());
}

void ShadowMapRenderer::Render()
{
    using namespace oglplus;
    static Context gl;
    static auto &scene = Scene::Active();
    auto camera = Camera::Active().get();

    if (!camera || !scene || !scene->IsLoaded() || !shadowCaster)
    {
        return;
    }

    SetAsActive();
    lightView.SetAsActive();
    shadowFramebuffer.Bind(FramebufferTarget::Draw);
    gl.Viewport(shadowMapSize.x, shadowMapSize.y);
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl.Clear().DepthBuffer().ColorBuffer();
    // activate geometry pass shader program
    CurrentProgram<DepthProgram>(DepthShader());
    // rendering flags
    gl.Disable(Capability::Blend);
    gl.Enable(Capability::DepthTest);
    // scene spatial cues
    auto sceneBB = scene->rootNode->boundaries;
    auto &center = sceneBB.Center();
    auto radius = distance(center, sceneBB.MaxPoint());
    auto direction = -shadowCaster->Direction();
    // fix light frustum to fit scene bounding sphere
    lightView.OrthoRect(glm::vec4(-radius, radius, -radius, radius));
    lightView.ClipPlaneNear(-radius);
    lightView.ClipPlaneFar(2.0f * radius);
    lightView.Projection(Camera::ProjectionMode::Orthographic);
    lightView.transform.Position(center - direction * radius);
    lightView.transform.Forward(direction);
    // draw whole scene tree from root node
    scene->rootNode->DrawList();
    // recover original render camera
    camera->SetAsActive();
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
}

void ShadowMapRenderer::Caster(const Light * caster)
{
    shadowCaster = caster;
}

const glm::mat4x4 &ShadowMapRenderer::LightSpaceMatrix()
{
    static glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
                                0.0, 0.5, 0.0, 0.0,
                                0.0, 0.0, 0.5, 0.0,
                                0.5, 0.5, 0.5, 1.0);
    return lightSpaceMatrix = biasMatrix * lightView.ProjectionMatrix()
                              * lightView.ViewMatrix();
}

const Light * ShadowMapRenderer::Caster() const
{
    return shadowCaster;
}

void ShadowMapRenderer::BindReading(unsigned unit) const
{
    shadowMap.Active(unit);
    shadowMap.Bind(oglplus::TextureTarget::_2D);
}

const Camera &ShadowMapRenderer::LightCamera() const
{
    return lightView;
}

const oglplus::Texture &ShadowMapRenderer::ShadowMap() const
{
    return shadowMap;
}

ShadowMapRenderer::ShadowMapRenderer(RenderWindow &window) : Renderer(window),
    shadowCaster(nullptr)
{
    CreateFramebuffer(2048, 2048);
}

ShadowMapRenderer::~ShadowMapRenderer()
{
}

DepthProgram &ShadowMapRenderer::DepthShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<DepthProgram *>
                        (assets->programs["Depth"].get());
    return prog;
}

void ShadowMapRenderer::CreateFramebuffer(const unsigned &w,
        const unsigned &h)
{
    using namespace oglplus;
    static Context gl;
    // save size
    shadowMapSize = glm::uvec2(w, h);
    // create render buffer for depth testing
    depthRender.Bind(RenderbufferTarget::Renderbuffer);
    depthRender.Storage(RenderbufferTarget::Renderbuffer,
                        PixelDataInternalFormat::DepthComponent24, w, h);
    // setup framebuffer
    shadowFramebuffer.Bind(FramebufferTarget::Draw);
    gl.Bound(TextureTarget::_2D, shadowMap)
    .Image2D(0, PixelDataInternalFormat::RG32F, w, h, 0,
             PixelDataFormat::RG, PixelDataType::Float, nullptr)
    .MinFilter(TextureMinFilter::Linear).MagFilter(TextureMagFilter::Linear);
    shadowFramebuffer.AttachTexture(FramebufferTarget::Draw,
                                    FramebufferColorAttachment::_0,
                                    shadowMap, 0);
    shadowFramebuffer.AttachRenderbuffer(FramebufferTarget::Draw,
                                         FramebufferAttachment::Depth,
                                         depthRender);
    gl.DrawBuffer(FramebufferColorAttachment::_0);

    // check if success building frame buffer
    if (!Framebuffer::IsComplete(FramebufferTarget::Draw))
    {
        auto status = Framebuffer::Status(FramebufferTarget::Draw);
        Framebuffer::HandleIncompleteError(FramebufferTarget::Draw, status);
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}
