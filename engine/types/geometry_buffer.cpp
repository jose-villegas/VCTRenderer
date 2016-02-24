#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "geometry_buffer.h"

oglplus::Texture &GeometryBuffer::RenderTarget(RenderTargets renderTarget)
{
    return bufferTextures[renderTarget];
}

void GeometryBuffer::Bind(oglplus::FramebufferTarget target) const
{
    geometryBuffer.Bind(target);
}

void GeometryBuffer::AttachTexture(RenderTargets renderTarget,
                                   oglplus::FramebufferTarget target, int level)
{
    if (renderTarget != Depth)
    {
        auto attachment = oglplus::FramebufferColorAttachment::_0;
        attachment = oglplus::FramebufferColorAttachment(
                         static_cast<unsigned int>(attachment) +
                         static_cast<unsigned int>(renderTarget));
        // finally attach texture
        geometryBuffer.AttachTexture(target, attachment,
                                     bufferTextures[renderTarget], level);
        // link color attachment
        colorBuffers.push_back(attachment);
    }
    else  // depth case
    {
        geometryBuffer.AttachTexture(target, oglplus::FramebufferAttachment::Depth,
                                     bufferTextures[renderTarget], level);
    }
}

void GeometryBuffer::DrawBuffers()
{
    oglplus::Context().DrawBuffers(colorBuffers);
    colorBuffers.clear();
}

void GeometryBuffer::ActivateTextures()
{
    using namespace oglplus;
    // bind and active position buffer texture
    Texture::Active(static_cast<int>(Position));
    bufferTextures[static_cast<int>(Position)]
    .Bind(TextureTarget::_2D);
    // bind and active normal buffer texture
    Texture::Active(static_cast<int>(Normal));
    bufferTextures[static_cast<int>(Normal)]
    .Bind(TextureTarget::_2D);
    // bind and active albedo buffer texture
    Texture::Active(static_cast<int>(Albedo));
    bufferTextures[static_cast<int>(Albedo)]
    .Bind(TextureTarget::_2D);
    // bind and active specular buffer texture
    Texture::Active(static_cast<int>(Specular));
    bufferTextures[static_cast<int>(Specular)]
    .Bind(TextureTarget::_2D);
}

GeometryBuffer::GeometryBuffer()
{
}

GeometryBuffer::~GeometryBuffer()
{
}
