#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shadow_map_renderer.h"

#include "../scene/light.h"
#include "../programs/depth_program.h"
#include "../../../scene/scene.h"
#include "../../../core/assets_manager.h"

#include <oglplus/context.hpp>
#include "../../../3rdparty/imgui/imgui.h"

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
    // activate geometry pass shader program
    CurrentProgram<DepthProgram>(DepthShader());
    gl.Viewport(shadowMapSize.x, shadowMapSize.y);
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl.Clear().DepthBuffer();
    // fix light camera with light shadow caster
    lightView.transform.Position(shadowCaster->Position());
    lightView.transform.Forward(shadowCaster->Direction());
    // draw whole scene tree from root node
    scene->rootNode->DrawList();
    // recover original render camera
    camera->SetAsActive();
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
    //test
    static auto size = ImVec2(258, 258);
    static auto uv1 = ImVec2(-1, 0);
    static auto uv2 = ImVec2(0, -1);
    auto texName = reinterpret_cast<void *>
                   (static_cast<intptr_t>(GetName(renderDepth)));
    ImGui::Image(texName, size, uv1, uv2);
}

void ShadowMapRenderer::Caster(const Light * caster)
{
    shadowCaster = caster;
}

ShadowMapRenderer::ShadowMapRenderer(RenderWindow &window) : Renderer(window),
    shadowCaster(nullptr)
{
    CreateFramebuffer(512, 512);
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
    shadowMapSize = glm::uvec2(w, h);
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
