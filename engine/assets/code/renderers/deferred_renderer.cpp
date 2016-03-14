#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "deferred_renderer.h"

#include <oglplus/vertex_attrib.hpp>
#include <oglplus/bound/texture.hpp>

#include "voxelizer_renderer.h"
#include "../misc/geometry_buffer.h"
#include "../scene/camera.h"
#include "../scene/scene.h"
#include "../scene/material.h"
#include "../scene/light.h"
#include "../rendering/render_window.h"
#include "../core/assets_manager.h"
#include "../programs/geometry_program.h"
#include "../programs/lighting_program.h"

std::unique_ptr<GeometryBuffer> DeferredRenderer::geometryBuffer = nullptr;

DeferredRenderer::DeferredRenderer(RenderWindow &window) : Renderer(window)
{
    // create textures and attachments for framebuffer in deferredhandler
    SetupGeometryBuffer(Window().Info().width, Window().Info().height);
    // rendering quad plane
    CreateFullscreenQuad();
}

DeferredRenderer::~DeferredRenderer()
{
    // geometry buffer has context dependant components
    // its deletion needs to be included with the destructor
    // so it can be called before the context ceases to exist.
    delete geometryBuffer.release();
}

void DeferredRenderer::DrawFullscreenQuad() const
{
    static oglplus::Context gl;
    fsQuadVertexArray.Bind();
    gl.DrawElements(
        oglplus::PrimitiveType::Triangles, 6,
        oglplus::DataType::UnsignedInt
    );
}

const GeometryBuffer &DeferredRenderer::GBuffer()
{
    return *geometryBuffer;
}

void DeferredRenderer::Render()
{
    static oglplus::Context gl;
    static auto &gbuffer = GBuffer();
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();

    if (!camera || !scene || !scene->IsLoaded() || VoxelizerRenderer::ShowVoxels)
    {
        return;
    }

    SetAsActive();
    // bind g buffer for writing
    gbuffer.Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    CurrentProgram<GeometryProgram>(GeometryPass());
    // rendering and GL flags
    gl.Viewport(Window().Info().width, Window().Info().height);
    gl.ColorMask(true, true, true, true);
    gl.ClearDepth(1.0f);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.Enable(oglplus::Capability::CullFace);
    gl.FrontFace(oglplus::FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    UseFrustumCulling = true;
    // draw whole scene tree from root node
    scene->rootNode->DrawList();
    // start light pass
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    CurrentProgram<LightingProgram>(LightingPass());
    // bind g buffer textures for reading
    gbuffer.ActivateTextures();
    // pass light info and texture locations for final light pass
    SetLightPassUniforms();
    // draw the result onto a fullscreen quad
    DrawFullscreenQuad();
}

void DeferredRenderer::SetMatricesUniforms(const Node &node) const
{
    auto &prog = CurrentProgram<GeometryProgram>();
    prog.matrices.modelView.Set(node.ModeView());
    prog.matrices.modelViewProjection.Set(node.ModelViewProjection());
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
    prog.inverseProjection.Set(camera->InverseProjectionMatrix());
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
        auto &intensity = light->Intensity();
        // update view space direction-position
        uLight.direction.Set(light->Direction(camera->ViewMatrix()));
        uLight.ambient.Set(light->Ambient() * intensity.x);
        uLight.diffuse.Set(light->Diffuse() * intensity.y);
        uLight.specular.Set(light->Specular() * intensity.z);
    }

    for (int i = 0; i < points.size(); i++)
    {
        auto &light = points[i];
        auto &uLight = uPoints[i];
        auto &intensity = light->Intensity();
        // update view space direction-position
        uLight.position.Set(light->Position(camera->ViewMatrix()));
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
        auto &intensity = light->Intensity();
        // update view space direction-position
        uLight.position.Set(light->Position(camera->ViewMatrix()));
        uLight.direction.Set(light->Direction(camera->ViewMatrix()));
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
}

GeometryProgram &DeferredRenderer::GeometryPass()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<GeometryProgram *>
                        (assets->programs[AssetsManager::GeometryPass].get());
    return prog;
}

LightingProgram &DeferredRenderer::LightingPass()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<LightingProgram *>
                        (assets->programs[AssetsManager::LightPass].get());
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

void DeferredRenderer::CreateFullscreenQuad() const
{
    using namespace oglplus;
    // bind vao for full screen quad
    fsQuadVertexArray.Bind();
    // data for fs quad
    static const std::array<float, 20> fsQuadVertexBufferData =
    {
        // X    Y    Z     U     V
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // vertex 0
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // vertex 1
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // vertex 2
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // vertex 3
    };
    // bind vertex buffer and fill
    fsQuadVertexBuffer.Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, fsQuadVertexBufferData);
    // set up attrib points
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, 5 * sizeof(float),
             reinterpret_cast<const GLvoid *>(0));
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(2, DataType::Float, false, 5 * sizeof(float),
             reinterpret_cast<const GLvoid *>(12));
    // data for element buffer array
    static const std::array<unsigned int, 6> indexData =
    {
        0, 1, 2, // first triangle
        2, 1, 3, // second triangle
    };
    // bind and fill element array
    fsQuadElementBuffer.Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, indexData);
    // unbind vao
    NoVertexArray().Bind();
}
