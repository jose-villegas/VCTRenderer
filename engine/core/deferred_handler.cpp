#include "deferred_handler.h"

#include <oglplus/vertex_attrib.hpp>
#include <oglplus/bound/texture.hpp>

#include <iostream>
#include "lighting_program.h"
#include "geometry_program.h"

DeferredHandler::DeferredHandler(unsigned int windowWith,
                                 unsigned int windowHeight)
{
    LoadShaders();
    SetupGBuffer(windowWith, windowHeight);
    CreateFullscreenQuad();
}

void DeferredHandler::CreateFullscreenQuad()
{
    using namespace oglplus;
    // bind vao for full screen quad
    fullscreenQuadVertexArray.Bind();
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
    fullscreenQuadVertexBuffer.Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, fsQuadVertexBufferData);
    // set up attrib points
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, 5 * sizeof(float),
             reinterpret_cast<const void *>(0));
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(2, DataType::Float, false, 5 * sizeof(float),
             reinterpret_cast<const void *>(12));
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

/// <summary>
/// Renders a full screen quad, useful for the light pass stage
/// </summary>
void DeferredHandler::RenderFullscreenQuad()
{
    static oglplus::Context gl;
    fullscreenQuadVertexArray.Bind();
    gl.DrawElements(
        oglplus::PrimitiveType::Triangles, 6,
        oglplus::DataType::UnsignedInt
    );
}

const std::vector<oglplus::Texture> &DeferredHandler::BufferTextures() const
{
    return bufferTextures;
}



/// <summary>
/// Loads the deferred rendering required shaders
/// </summary>
void DeferredHandler::LoadShaders()
{
    using namespace oglplus;

    if (!lightingProgram)
    {
        lightingProgram = std::make_unique<LightingProgram>();
    }

    // light pass shader source code and compile
    std::ifstream vsLightPassFile("resources\\shaders\\light_pass.vert");
    std::string vsLightPassSource(
        (std::istreambuf_iterator<char>(vsLightPassFile)),
        std::istreambuf_iterator<char>()
    );
    lightingProgram->vertexShader.Source(vsLightPassSource.c_str());
    lightingProgram->vertexShader.Compile();
    // fragment shader
    std::ifstream fsLightPassFile("resources\\shaders\\light_pass.frag");
    std::string fsLightPassSource(
        (std::istreambuf_iterator<char>(fsLightPassFile)),
        std::istreambuf_iterator<char>()
    );
    lightingProgram->fragmentShader.Source(fsLightPassSource.c_str());
    lightingProgram->fragmentShader.Compile();
    // create a new shader program and attach the shader
    lightingProgram->program.AttachShader(lightingProgram->vertexShader);
    lightingProgram->program.AttachShader(lightingProgram->fragmentShader);
    // link attached shader
    lightingProgram->program.Link().Use();
    // extract relevant uniforms
    lightingProgram->ExtractActiveUniforms();
    // close source files
    vsLightPassFile.close();
    fsLightPassFile.close();
    // geometry pass shader source code and compile
    std::ifstream vsGeomPassFile("resources\\shaders\\geometry_pass.vert");
    std::string vsGeomPassSource(
        (std::istreambuf_iterator<char>(vsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );

    if (!geometryProgram)
    {
        geometryProgram = std::make_unique<GeometryProgram>();
    }

    geometryProgram->vertexShader.Source(vsGeomPassSource.c_str());
    geometryProgram->vertexShader.Compile();
    // fragment shader
    std::ifstream fsGeomPassFile("resources\\shaders\\geometry_pass.frag");
    std::string fsGeomPassSource(
        (std::istreambuf_iterator<char>(fsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    geometryProgram->fragmentShader.Source(fsGeomPassSource.c_str());
    geometryProgram->fragmentShader.Compile();
    // create a new shader program and attach the shaders
    geometryProgram->program.AttachShader(geometryProgram->vertexShader);
    geometryProgram->program.AttachShader(geometryProgram->fragmentShader);
    // link attached shaders
    geometryProgram->program.Link().Use();
    // extract relevant uniforms
    geometryProgram->ExtractActiveUniforms();
    // close source files
    vsGeomPassFile.close();
    fsGeomPassFile.close();
}

/// <summary>
/// Setups the geometry buffer accordingly
/// with the rendering resolution
/// </summary>
/// <param name="windowWith">The window width.</param>
/// <param name="windowHeight">Height of the window.</param>
void DeferredHandler::SetupGBuffer(unsigned int windowWidth,
                                   unsigned int windowHeight)
{
    using namespace oglplus;
    static Context gl;
    colorBuffers.clear();
    bufferTextures.clear();
    geometryBuffer.Bind(FramebufferTarget::Draw);
    bufferTextures.resize((int)GBufferTextureId::TEXTURE_TYPE_MAX);
    // position color buffer
    {
        gl.Bound(Texture::Target::_2D,
                 bufferTextures[(int)GBufferTextureId::Position])
        .Image2D(
            0, PixelDataInternalFormat::RGB16F, windowWidth, windowHeight,
            0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
        ) // high precision texture
        .MinFilter(TextureMinFilter::Nearest)
        .MagFilter(TextureMagFilter::Nearest);
        geometryBuffer.AttachTexture(
            FramebufferTarget::Draw,
            FramebufferColorAttachment::_0,
            bufferTextures[(int)GBufferTextureId::Position], 0
        );
        colorBuffers.push_back(FramebufferColorAttachment::_0);
    }
    // normal color buffer
    {
        gl.Bound(Texture::Target::_2D,
                 bufferTextures[(int)GBufferTextureId::Normal])
        .Image2D(
            0, PixelDataInternalFormat::RGB16F, windowWidth, windowHeight,
            0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
        ) // high precision texture
        .MinFilter(TextureMinFilter::Nearest)
        .MagFilter(TextureMagFilter::Nearest);
        geometryBuffer.AttachTexture(
            FramebufferTarget::Draw,
            FramebufferColorAttachment::_1,
            bufferTextures[(int)GBufferTextureId::Normal], 0
        );
        colorBuffers.push_back(FramebufferColorAttachment::_1);
    }
    // albedo color buffer
    {
        gl.Bound(Texture::Target::_2D,
                 bufferTextures[(int)GBufferTextureId::Albedo])
        .Image2D(
            0, PixelDataInternalFormat::RGB8, windowWidth, windowHeight,
            0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
        ) // 8 bit per component texture
        .MinFilter(TextureMinFilter::Nearest)
        .MagFilter(TextureMagFilter::Nearest);
        geometryBuffer.AttachTexture(
            FramebufferTarget::Draw,
            FramebufferColorAttachment::_2,
            bufferTextures[(int)GBufferTextureId::Albedo], 0
        );
        colorBuffers.push_back(FramebufferColorAttachment::_2);
    }
    // specular intensity and shininess buffer
    {
        gl.Bound(Texture::Target::_2D,
                 bufferTextures[(int)GBufferTextureId::Specular])
        .Image2D(
            0, PixelDataInternalFormat::R8, windowWidth, windowHeight,
            0, PixelDataFormat::Red, PixelDataType::Float, nullptr
        ) // 8 bit per component texture
        .MinFilter(TextureMinFilter::Nearest)
        .MagFilter(TextureMagFilter::Nearest);
        geometryBuffer.AttachTexture(
            FramebufferTarget::Draw,
            FramebufferColorAttachment::_3,
            bufferTextures[(int)GBufferTextureId::Specular], 0
        );
        colorBuffers.push_back(FramebufferColorAttachment::_3);
    }
    // depth buffer
    gl.Bound(Texture::Target::_2D, bufferTextures[(int)GBufferTextureId::Depth])
    .Image2D(
        0, PixelDataInternalFormat::DepthComponent32F, windowWidth, windowHeight,
        0, PixelDataFormat::DepthComponent, PixelDataType::Float, nullptr
    ); // high precision float texture
    geometryBuffer.AttachTexture(
        FramebufferTarget::Draw,
        FramebufferAttachment::Depth,
        bufferTextures[(int)GBufferTextureId::Depth], 0
    );
    gl.DrawBuffers(colorBuffers.size(), colorBuffers.data());

    if (!Framebuffer::IsComplete(Framebuffer::Target::Draw))
    {
        auto status = Framebuffer::Status(Framebuffer::Target::Draw);
        std::cout << "(DeferredHandler) Framebuffer Error:"
                  << static_cast<unsigned int>(status);
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}

DeferredHandler::~DeferredHandler() {}

/// <summary>
/// Binds the geometry buffer for reading or drawing
/// </summary>
/// <param name="bindingMode">The binding mode.</param>
void DeferredHandler::BindGeometryBuffer(const oglplus::FramebufferTarget
        & bindingMode)
{
    static oglplus::Context gl;
    gl.Bind(bindingMode, geometryBuffer);
}

/// <summary>
/// Reads from the specified color buffer attached to the geometry buffer.
/// </summary>
/// <param name="bufferTexId">The texture target to read from.</param>
void DeferredHandler::ReadGeometryBuffer(const GBufferTextureId
        & bufferTexId)
{
    static oglplus::Context gl;
    gl.ReadBuffer(colorBuffers[static_cast<unsigned int>(bufferTexId)]);
}

/// <summary>
/// Activates texture slots for the geometry buffer
/// texture targets and binds the textures
/// </summary>
void DeferredHandler::ActivateBindTextureTargets()
{
    using namespace oglplus;
    // bind and active position buffer texture
    Texture::Active((int)GBufferTextureId::Position);
    bufferTextures[(int)GBufferTextureId::Position]
    .Bind(TextureTarget::_2D);
    // bind and active normal buffer texture
    Texture::Active((int)GBufferTextureId::Normal);
    bufferTextures[(int)GBufferTextureId::Normal]
    .Bind(TextureTarget::_2D);
    // bind and active albedo buffer texture
    Texture::Active((int)GBufferTextureId::Albedo);
    bufferTextures[(int)GBufferTextureId::Albedo]
    .Bind(TextureTarget::_2D);
    // bind and active specular buffer texture
    Texture::Active((int)GBufferTextureId::Specular);
    bufferTextures[(int)GBufferTextureId::Specular]
    .Bind(TextureTarget::_2D);
}