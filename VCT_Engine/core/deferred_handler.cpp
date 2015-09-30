#include "stdafx.h"
#include "deferred_handler.h"


DeferredHandler::DeferredHandler(unsigned int windowWith,
                                 unsigned int windowHeight)
{
    LoadShaders();
    InitializeGBuffer(windowWith, windowHeight);
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
    geomBuffer.Bind(Framebuffer::Target::Draw);
    std::vector<Context::ColorBuffer> drawBuffers;

    // create geometry buffer textures
    for(unsigned int i = 0; i < bTextures.size(); i++)
    {
        gl.Bound(Texture::Target::_2D, bTextures[i])
        .Image2D(
            0, PixelDataInternalFormat::RGB32F, windowWith, windowHeight,
            0, PixelDataFormat::RGB, PixelDataType::Float, nullptr
        );
        geomBuffer.AttachTexture(
            Framebuffer::Target::Draw,
            (FramebufferAttachment)
            ((int)FramebufferAttachment::Color + i),
            bTextures[i], 0
        );
        // draw buffers open
        drawBuffers.push_back(
            (FramebufferColorAttachment)
            ((int)FramebufferColorAttachment::_0 + i)
        );
    }

    // depth buffer
    gl.Bound(Texture::Target::_2D, bDepthTexture)
    .Image2D(
        0, PixelDataInternalFormat::DepthComponent32F, windowWith, windowHeight,
        0, PixelDataFormat::DepthComponent, PixelDataType::Float, nullptr
    );
    geomBuffer.AttachTexture(
        Framebuffer::Target::Draw,
        FramebufferAttachment::Depth, bDepthTexture, 0
    );
    gl.DrawBuffers(bTextures.size(), drawBuffers.data());

    if(Framebuffer::IsComplete(Framebuffer::Target::Draw))
    {
        std::cout << "(DeferredHandler) Framebuffer Error:" << geomBuffer.Status;
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}

DeferredHandler::~DeferredHandler()
{
}

void DeferredHandler::BindGBuffer(oglplus::Framebuffer::Target &bindingMode)
{
    gl.Bind(bindingMode, geomBuffer);
}
