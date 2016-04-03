#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "deferred_renderer.h"

#include "voxelizer_renderer.h"
#include "shadow_map_renderer.h"
#include "../misc/geometry_buffer.h"
#include "../scene/camera.h"
#include "../scene/scene.h"
#include "../scene/material.h"
#include "../scene/light.h"
#include "../rendering/render_window.h"
#include "../core/assets_manager.h"
#include "../programs/geometry_program.h"
#include "../programs/lighting_program.h"

#include <oglplus/vertex_attrib.hpp>
#include <oglplus/bound/texture.hpp>
#include <glm/detail/func_matrix.hpp>

DeferredRenderer::DeferredRenderer(RenderWindow &window) : Renderer(window)
{
    // create textures and attachments for framebuffer in deferredhandler
    SetupGeometryBuffer(Window().Info().width, Window().Info().height);
}

DeferredRenderer::~DeferredRenderer()
{
}

const GeometryBuffer &DeferredRenderer::GBuffer() const
{
    return *geometryBuffer;
}

void DeferredRenderer::Render()
{
    using namespace oglplus;
    static Context gl;
    static auto &gbuffer = GBuffer();
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();

    if (!camera || !scene || !scene->IsLoaded() || VoxelizerRenderer::ShowVoxels)
    {
        return;
    }

    SetAsActive();
    // bind g buffer for writing
    gbuffer.Bind(FramebufferTarget::Draw);
    gl.ColorMask(true, true, true, true);
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl.Viewport(Window().Info().width, Window().Info().height);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    CurrentProgram<GeometryProgram>(GeometryPass());
    // rendering and GL flags
    gl.ClearDepth(1.0f);
    gl.Enable(Capability::DepthTest);
    gl.Enable(Capability::Blend);
    gl.BlendFunc(BlendFunction::SrcAlpha,
                 BlendFunction::OneMinusSrcAlpha);
    gl.Enable(Capability::CullFace);
    gl.FrontFace(FaceOrientation::CCW);
    gl.CullFace(Face::Back);
    UseFrustumCulling = true;
    // draw whole scene tree from root node
    scene->rootNode->DrawList();
    // start light pass
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
    gl.ColorMask(true, true, true, true);
    gl.Viewport(Window().Info().width, Window().Info().height);
    gl.Clear().ColorBuffer().DepthBuffer();
    CurrentProgram<LightingProgram>(LightingPass());
    // bind g buffer textures for reading
    gbuffer.ActivateTextures();
    // pass light info and texture locations for final light pass
    SetLightPassUniforms();
    // draw the result onto a fullscreen quad
    fsQuad.Draw();
}

void DeferredRenderer::SetMatricesUniforms(const Node &node) const
{
    auto &prog = CurrentProgram<GeometryProgram>();
    static auto &camera = Camera::Active();
    prog.matrices.normal.Set(node.InverseTranspose());
    prog.matrices.modelViewProjection.Set(camera->ViewProjectionMatrix() *
                                          node.transform.Matrix());
}

void DeferredRenderer::SetMaterialUniforms(const Material &material)
const
{
    using namespace oglplus;
    auto &prog = CurrentProgram<GeometryProgram>();
    prog.material.diffuse.Set(material.Diffuse());
    prog.material.specular.Set(material.Specular());
    prog.material.shininess.Set(material.Shininess());
    prog.material.useNormalsMap.Set(material.HasTexture(RawTexture::Normals));
    // set textures
    Texture::Active(RawTexture::Diffuse);
    material.BindTexture(RawTexture::Diffuse);
    prog.diffuseMap.Set(RawTexture::Diffuse);
    Texture::Active(RawTexture::Specular);
    material.BindTexture(RawTexture::Specular);
    prog.specularMap.Set(RawTexture::Specular);
    Texture::Active(RawTexture::Normals);
    material.BindTexture(RawTexture::Normals);
    prog.normalsMap.Set(RawTexture::Normals);
}

void DeferredRenderer::SetLightPassUniforms() const
{
    static auto &camera = Camera::Active();
    auto &prog = CurrentProgram<LightingProgram>();;
    prog.inverseProjectionView.Set(camera->InverseViewMatrix() *
                                   camera->InverseProjectionMatrix());
    prog.gDepth.Set(GeometryBuffer::Depth);
    prog.gNormal.Set(GeometryBuffer::Normal);
    prog.gAlbedo.Set(GeometryBuffer::Albedo);
    prog.gSpecular.Set(GeometryBuffer::Specular);
    // set directional lights uniforms
    auto &directionals = Light::Directionals();
    auto &points = Light::Points();
    auto &spots = Light::Spots();
    // uniform arrays of lights
    auto &uDirectionals = prog.directionalLight;
    auto &uPoints = prog.pointLight;
    auto &uSpots = prog.spotLight;

    for (int i = 0; i < directionals.size(); i++)
    {
        auto &light = directionals[i];
        auto &uLight = uDirectionals[i];
        auto &intensity = light->Intensities();
        // update view space direction-position
        uLight.direction.Set(light->Direction());
        uLight.ambient.Set(light->Ambient() * intensity.x);
        uLight.diffuse.Set(light->Diffuse() * intensity.y);
        uLight.specular.Set(light->Specular() * intensity.z);
    }

    for (int i = 0; i < points.size(); i++)
    {
        auto &light = points[i];
        auto &uLight = uPoints[i];
        auto &intensity = light->Intensities();
        // update view space direction-position
        uLight.position.Set(light->Position());
        uLight.ambient.Set(light->Ambient() * intensity.x);
        uLight.diffuse.Set(light->Diffuse() * intensity.y);
        uLight.specular.Set(light->Specular() * intensity.z);
        uLight.attenuation.constant.Set(light->attenuation.Constant());
        uLight.attenuation.linear.Set(light->attenuation.Linear());
        uLight.attenuation.quadratic.Set(light->attenuation.Quadratic());
    }

    for (int i = 0; i < spots.size(); i++)
    {
        auto &light = spots[i];
        auto &uLight = uSpots[i];
        auto &intensity = light->Intensities();
        // update view space direction-position
        uLight.position.Set(light->Position());
        uLight.direction.Set(light->Direction());
        uLight.ambient.Set(light->Ambient() * intensity.x);
        uLight.diffuse.Set(light->Diffuse() * intensity.y);
        uLight.specular.Set(light->Specular() * intensity.z);
        uLight.attenuation.constant.Set(light->attenuation.Constant());
        uLight.attenuation.linear.Set(light->attenuation.Linear());
        uLight.attenuation.quadratic.Set(light->attenuation.Quadratic());
        uLight.angleInnerCone.Set(cos(light->AngleInnerCone()));
        uLight.angleOuterCone.Set(cos(light->AngleOuterCone()));
    }

    // pass number of lights per type
    prog.lightTypeCount[0].Set(directionals.size());
    prog.lightTypeCount[1].Set(points.size());
    prog.lightTypeCount[2].Set(spots.size());
    // pass shadowing parameters
    auto &shadowing = *static_cast<ShadowMapRenderer *>(AssetsManager::Instance()
                      ->renderers["Shadowmapping"].get());
    prog.lightViewProjection.Set(shadowing.LightSpaceMatrix());
    shadowing.BindReading(6);
    prog.shadowMap.Set(6);
    prog.exponents.Set(shadowing.Exponents());
    prog.lightBleedingReduction.Set(shadowing.LightBleedingReduction());
}

GeometryProgram &DeferredRenderer::GeometryPass()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<GeometryProgram *>
                        (assets->programs["Geometry"].get());
    return prog;
}

LightingProgram &DeferredRenderer::LightingPass()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<LightingProgram *>
                        (assets->programs["Lighting"].get());
    return prog;
}

void DeferredRenderer::SetupGeometryBuffer(unsigned windowWidth,
        unsigned windowHeight)
{
    using namespace oglplus;
    static Context gl;

    if (!geometryBuffer)
    {
        geometryBuffer = std::make_unique<GeometryBuffer>();
    }
    // already setup the geometry buffer, need to delete previous to resetup
    else { return; }

    // initialize geometry buffer
    geometryBuffer->Bind(FramebufferTarget::Draw);
    // build textures -- normal
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Normal))
    .Image2D(0, PixelDataInternalFormat::RGB8SNorm, windowWidth, windowHeight,
             0, PixelDataFormat::RGB, PixelDataType::Float, nullptr)
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer->AttachTexture(GeometryBuffer::Normal, FramebufferTarget::Draw);
    // build textures -- albedo
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Albedo))
    .Image2D(0, PixelDataInternalFormat::RGB8, windowWidth, windowHeight,
             0, PixelDataFormat::RGB, PixelDataType::Float, nullptr)
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer->AttachTexture(GeometryBuffer::Albedo, FramebufferTarget::Draw);
    // build textures -- specular color and power
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Specular))
    .Image2D(0, PixelDataInternalFormat::RGBA8, windowWidth, windowHeight,
             0, PixelDataFormat::RGBA, PixelDataType::Float, nullptr)
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer->AttachTexture(GeometryBuffer::Specular,
                                  FramebufferTarget::Draw);
    // attach depth texture for depth testing
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Depth))
    .Image2D(0, PixelDataInternalFormat::DepthComponent24, windowWidth,
             windowHeight, 0, PixelDataFormat::DepthComponent,
             PixelDataType::Float, nullptr)
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer->AttachTexture(GeometryBuffer::Depth, FramebufferTarget::Draw);
    // set draw buffers
    geometryBuffer->DrawBuffers();

    // check if success building frame buffer
    if (!Framebuffer::IsComplete(FramebufferTarget::Draw))
    {
        auto status = Framebuffer::Status(FramebufferTarget::Draw);
        Framebuffer::HandleIncompleteError(FramebufferTarget::Draw, status);
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}
