#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "framebuffer_textures.h"
#include "main_menu.h"

#include "../misc/geometry_buffer.h"
#include "../renderers/deferred_renderer.h"\

#include "../../../core/assets_manager.h"

#include <oglplus/texture.hpp>
#include "../renderers/shadow_map_renderer.h"

using namespace ImGui;

void DrawBufferTexture(const oglplus::Texture &tex, const std::string &name)
{
    using namespace oglplus;
    static auto size = ImVec2(160, 90);
    static auto uv1 = ImVec2(-1, 0);
    static auto uv2 = ImVec2(0, -1);
    auto texName = reinterpret_cast<void *>
                   (static_cast<intptr_t>(GetName(tex)));
    BeginGroup();
    Text(name.c_str());
    Image(texName, size, uv1, uv2);

    if (IsItemHovered())
    {
        BeginTooltip();
        Image(texName, ImVec2(160 * 4, 90 * 4), uv1, uv2);
        EndTooltip();
    }

    EndGroup();
}

void UIFramebuffers::Draw()
{
    if (!UIMainMenu::drawFramebuffers) { return; }

    static auto &gbuffer = static_cast<DeferredRenderer *>
                           (AssetsManager::Instance()->renderers
                            ["Deferred"].get())->GBuffer();
    static auto &shadow = static_cast<ShadowMapRenderer *>
                          (AssetsManager::Instance()->renderers
                           ["Shadowmapping"].get())->ShadowMap();
    // begin editor
    Begin("Geometry Buffer", &UIMainMenu::drawFramebuffers,
          ImGuiWindowFlags_AlwaysAutoResize);
    BeginGroup();
    Text("Geometry Buffer");
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Normal), "Normal");
    SameLine();
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Albedo), "Albedo");
    SameLine();
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Specular), "Specular");
    SameLine();
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Depth), "Depth");
    EndGroup();
    BeginGroup();
    Text("Shadow Mapping");
    DrawBufferTexture(shadow, "Depth");
    EndGroup();
    End();
}

UIFramebuffers::UIFramebuffers()
{
}


UIFramebuffers::~UIFramebuffers()
{
}
