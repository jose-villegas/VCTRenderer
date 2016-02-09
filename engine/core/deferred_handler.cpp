#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "deferred_handler.h"

#include <oglplus/vertex_attrib.hpp>
#include <oglplus/bound/texture.hpp>

#include "lighting_program.h"
#include "geometry_program.h"
#include "geometry_buffer.h"

DeferredHandler::DeferredHandler(unsigned int windowWith,
                                 unsigned int windowHeight)
{
    geometryBuffer = std::make_unique<GeometryBuffer>();
    LoadShaders();
    SetupGeometryBuffer(windowWith, windowHeight);
    CreateFullscreenQuad();
}

void DeferredHandler::CreateFullscreenQuad() const
{
    using namespace oglplus;
    // bind vao for full screen quad
    fullscreenQuadVertexArray.Bind();
    // data for fs quad
    static const std::array<float, 20> fsQuadVertexBufferData =
    {
        // X    Y    Z
        1.0f, 1.0f, 0.0f, // vertex 0
        -1.0f, 1.0f, 0.0f, // vertex 1
        1.0f, -1.0f, 0.0f, // vertex 2
        -1.0f, -1.0f, 0.0f, // vertex 3
    };
    // bind vertex buffer and fill
    fullscreenQuadVertexBuffer.Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, fsQuadVertexBufferData);
    // set up attrib points
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, 3 * sizeof(float),
             reinterpret_cast<const void *>(0));
    // data for element buffer array
    static const std::array<unsigned int, 6> indexData =
    {
        0, 1, 2, // first triangle
        2, 1, 3, // second triangle
    };
    // bind and fill element array
    fullscreenQuadElementBuffer.Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, indexData);
    // unbind vao
    NoVertexArray().Bind();
}

const std::unique_ptr<GeometryBuffer> &DeferredHandler::GBuffer() const
{
    return geometryBuffer;
}

void DeferredHandler::RenderFullscreenQuad() const
{
    static oglplus::Context gl;
    fullscreenQuadVertexArray.Bind();
    gl.DrawElements(
        oglplus::PrimitiveType::Triangles, 6,
        oglplus::DataType::UnsignedInt
    );
}

void DeferredHandler::LoadShaders()
{
    using namespace oglplus;

    if (!lightingProgram)
    {
        lightingProgram = std::make_unique<LightingProgram>();
    }

    if (!geometryProgram)
    {
        geometryProgram = std::make_unique<GeometryProgram>();
    }

    // geometry pass shader source code and compile
    geometryProgram->Build("resources\\shaders\\geometry_pass.vert",
                           "resources\\shaders\\geometry_pass.frag");
    //// light pass shader source code and compile
    lightingProgram->Build("resources\\shaders\\light_pass.vert",
                           "resources\\shaders\\light_pass.frag");
}

void DeferredHandler::SetupGeometryBuffer(unsigned int windowWidth,
        unsigned int windowHeight)
{
    using namespace oglplus;
    static Context gl;
    renderingSize = glm::vec2(windowWidth, windowHeight);
    // initialize geometry buffer
    geometryBuffer->Bind(FramebufferTarget::Draw);
    // build textures -- position
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Position))
    .Image2D(0, PixelDataInternalFormat::RGB16F, windowWidth, windowHeight,
             0, PixelDataFormat::RGB, PixelDataType::Float, nullptr)
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer->AttachTexture(GeometryBuffer::Position,
                                  FramebufferTarget::Draw);
    // build textures -- normal
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Normal))
    .Image2D(0, PixelDataInternalFormat::RGB16F, windowWidth, windowHeight,
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
    // build textures -- specular
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Specular))
    .Image2D(0, PixelDataInternalFormat::RGB8, windowWidth, windowHeight,
             0, PixelDataFormat::RGB, PixelDataType::Float, nullptr)
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geometryBuffer->AttachTexture(GeometryBuffer::Specular,
                                  FramebufferTarget::Draw);
    // build textures -- depth
    gl.Bound(TextureTarget::_2D,
             geometryBuffer->RenderTarget(GeometryBuffer::Depth))
    .Image2D(0, PixelDataInternalFormat::DepthComponent32F, windowWidth,
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

DeferredHandler::~DeferredHandler() {}