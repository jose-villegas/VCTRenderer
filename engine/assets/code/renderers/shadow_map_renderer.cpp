#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shadow_map_renderer.h"

#include "../scene/light.h"
#include "../../../scene/scene.h"

#include <oglplus/context.hpp>

void ShadowMapRenderer::Render()
{
    //static oglplus::Context gl;
    //static auto &scene = Scene::Active();
    //auto camera = Camera::Active().get();
    //if (!camera || !scene || !scene->IsLoaded())
    //{
    //    return;
    //}
    //SetAsActive();
    //lightView.SetAsActive();
    //// draw whole scene tree from root node
    //scene->rootNode->DrawList();
    //// recover original render camera
    //camera->SetAsActive();
}

ShadowMapRenderer::ShadowMapRenderer(RenderWindow &window) : Renderer(window),
    shadowCaster(nullptr)
{
    CreateFramebuffer(512, 512);
}

ShadowMapRenderer::~ShadowMapRenderer()
{
}

void ShadowMapRenderer::CreateFramebuffer(const unsigned &w,
        const unsigned &h) const
{
    using namespace oglplus;
    shadowFramebuffer.Bind(FramebufferTarget::Draw);
    renderDepth.Bind(TextureTarget::_2D);
    renderDepth.Image2D(TextureTarget::_2D, 0,
                        PixelDataInternalFormat::DepthComponent24, w, w, 0,
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
