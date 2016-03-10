#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shadow_map_renderer.h"

#include "../scene/light.h"

ShadowMapRenderer::ShadowMapRenderer(): shadowCaster(nullptr)
{
}

ShadowMapRenderer::~ShadowMapRenderer()
{
}

void ShadowMapRenderer::CreateFramebuffer(const unsigned size) const
{
    using namespace oglplus;
    shadowFramebuffer.Bind(FramebufferTarget::Draw);
    renderDepth.Bind(TextureTarget::_2D);
    renderDepth.Image2D(TextureTarget::_2D, 0,
                        PixelDataInternalFormat::DepthComponent24, size, size, 0,
                        PixelDataFormat::DepthComponent,
                        PixelDataType::Float,
                        nullptr);
    renderDepth.MagFilter(TextureTarget::_2D, TextureMagFilter::Nearest);
    renderDepth.MinFilter(TextureTarget::_2D, TextureMinFilter::Nearest);
    renderDepth.WrapS(TextureTarget::_2D, TextureWrap::ClampToEdge);
    renderDepth.WrapT(TextureTarget::_2D, TextureWrap::ClampToEdge);
    renderDepth.CompareFunc(TextureTarget::_2D, CompareFunction::Equal);
    renderDepth.CompareMode(TextureTarget::_2D,
                            TextureCompareMode::CompareRefToTexture);
    shadowFramebuffer.AttachTexture(FramebufferTarget::Draw,
                                    FramebufferAttachment::Depth,
                                    renderDepth, 0);

    // check if success building frame buffer
    if (!Framebuffer::IsComplete(FramebufferTarget::Draw))
    {
        auto status = Framebuffer::Status(FramebufferTarget::Draw);
        Framebuffer::HandleIncompleteError(FramebufferTarget::Draw, status);
    }

    Framebuffer::Bind(Framebuffer::Target::Draw, FramebufferName(0));
}
