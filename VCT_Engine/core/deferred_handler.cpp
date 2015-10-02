#include "stdafx.h"
#include "deferred_handler.h"


DeferredHandler::DeferredHandler(unsigned int windowWith,
                                 unsigned int windowHeight)
{
    LoadShaders();
    InitializeGBuffer(windowWith, windowHeight);
    CreateFullscreenQuad();
}


void DeferredHandler::CreateFullscreenQuad()
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
    .Pointer(3, DataType::Float, false, 5 * sizeof(float), (const GLvoid*)0);
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(2, DataType::Float, false, 5 * sizeof(float), (const GLvoid*)12);
    // data for element buffer array
    static const std::array<float, 6> indexData =
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

void DeferredHandler::LoadShaders()
{
    using namespace oglplus;
    // load shaders source code and compile
    std::ifstream vsGeomPassFile("resources\\shaders\\geometry_pass.vert");
    std::string vsGeomPassSource(
        (std::istreambuf_iterator<char>(vsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    vsGeometryPass.Source(vsGeomPassSource.c_str());
    vsGeometryPass.Compile();
    // fragment shader
    std::ifstream fsGeomPassFile("resources\\shaders\\geometry_pass.frag");
    std::string fsGeomPassSource(
        (std::istreambuf_iterator<char>(fsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    fsGeometryPass.Source(fsGeomPassSource.c_str());
    fsGeometryPass.Compile();
    // create a new shader program and attach the shaders
    geometryPass.AttachShader(vsGeometryPass);
    geometryPass.AttachShader(fsGeometryPass);
    // link attached shaders
    geometryPass.Link().Use();
}

void DeferredHandler::InitializeGBuffer(unsigned int windowWith,
                                        unsigned int windowHeight)
{
    using namespace oglplus;
    openedColorBuffers.clear();
    geomBuffer.Bind(Framebuffer::Target::Draw);
    // position color buffer
    gl.Bound(Texture::Target::_2D, bTextures[GBufferTextureType::Position])
    .Image2D(
        0, PixelDataInternalFormat::RGB16F, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // high precision texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geomBuffer.AttachTexture(
        Framebuffer::Target::Draw,
        FramebufferColorAttachment::_0, bTextures[GBufferTextureType::Position], 0
    );
    openedColorBuffers.push_back(FramebufferColorAttachment::_0);
    // normal color buffer
    gl.Bound(Texture::Target::_2D, bTextures[GBufferTextureType::Normal])
    .Image2D(
        0, PixelDataInternalFormat::RGB16F, windowWith, windowHeight,
        0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
    ) // high precision texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geomBuffer.AttachTexture(
        Framebuffer::Target::Draw,
        FramebufferColorAttachment::_1, bTextures[GBufferTextureType::Normal], 0
    );
    openedColorBuffers.push_back(FramebufferColorAttachment::_1);
    // color + specular color buffer
    gl.Bound(Texture::Target::_2D, bTextures[GBufferTextureType::AlbedoSpecular])
    .Image2D(
        0, PixelDataInternalFormat::RGBA8, windowWith, windowHeight,
        0, PixelDataFormat::RGBA, PixelDataType::Float, nullptr
    ) // 8 bit per component texture
    .MinFilter(TextureMinFilter::Nearest)
    .MagFilter(TextureMagFilter::Nearest);
    geomBuffer.AttachTexture(
        Framebuffer::Target::Draw,
        FramebufferColorAttachment::_2, bTextures[GBufferTextureType::AlbedoSpecular], 0
    );
    openedColorBuffers.push_back(FramebufferColorAttachment::_2);
    // depth buffer
    gl.Bound(Texture::Target::_2D, bDepthTexture)
    .Image2D(
        0, PixelDataInternalFormat::DepthComponent32F, windowWith, windowHeight,
        0, PixelDataFormat::DepthComponent, PixelDataType::Float, nullptr
    ); // high precision float texture
    geomBuffer.AttachTexture(
        Framebuffer::Target::Draw,
        FramebufferAttachment::Depth, bDepthTexture, 0
    );
    gl.DrawBuffers(bTextures.size(), openedColorBuffers.data());

    if(!Framebuffer::IsComplete(Framebuffer::Target::Draw))
    {
        FramebufferStatus status = Framebuffer::Status(Framebuffer::Target::Draw);
        std::cout << "(DeferredHandler) Framebuffer Error:"
                  << (unsigned int)status;
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}

DeferredHandler::~DeferredHandler()
{
}

const std::vector<RawTexture::TextureType> & DeferredHandler::ProgramSamplers()
{
    static std::vector<RawTexture::TextureType> programSamplers;

    if(programSamplers.empty())
    {
        programSamplers =
        {
            RawTexture::Diffuse,
            RawTexture::Specular,
            RawTexture::Ambient
        };
    }

    return programSamplers;
}

void DeferredHandler::SetSamplerUniform(oglplus::Program &program,
                                        RawTexture::TextureType texType)
{
    if(texType == RawTexture::Ambient)
    {
        oglplus::UniformSampler(program, "ambientMap").Set(texType);
    }
    else if(texType == RawTexture::Specular)
    {
        oglplus::UniformSampler(program, "specularMap").Set(texType);
    }
    else if(texType == RawTexture::Diffuse)
    {
        oglplus::UniformSampler(program, "diffuseMap").Set(texType);
    }
}

void DeferredHandler::BindGBuffer(const oglplus::Framebuffer::Target
                                  &bindingMode)
{
    gl.Bind(bindingMode, geomBuffer);
}

void DeferredHandler::ReadGBuffer(const GBufferTextureType &gBufferTexType)
{
    gl.ReadBuffer(openedColorBuffers[gBufferTexType]);
}
