#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "framebuffer_textures.h"
#include "main_menu.h"

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

    static auto &gbuffer = static_cast<GIDeferredRenderer *>
                           (AssetsManager::Instance()->renderers
                            ["Deferred"].get())->BufferTextures();
    static auto &shadow = static_cast<ShadowMapRenderer *>
                          (AssetsManager::Instance()->renderers
                           ["Shadowmapping"].get())->ShadowMap();
    // begin editor
    Begin("Geometry Buffer", &UIMainMenu::drawFramebuffers,
          ImGuiWindowFlags_AlwaysAutoResize);
    BeginGroup();
    Text("Geometry Buffer");
    DrawBufferTexture(gbuffer[0], "Normal");
    SameLine();
    DrawBufferTexture(gbuffer[1], "Albedo");
    SameLine();
    DrawBufferTexture(gbuffer[2], "Specular");
    SameLine();
    DrawBufferTexture(gbuffer[3], "Emissive");
    SameLine();
    DrawBufferTexture(gbuffer[4], "Depth");
    EndGroup();
    BeginGroup();
    Text("Shadow Mapping");
    DrawBufferTexture(shadow, "EVSM4");
    EndGroup();
    End();
}

UIFramebuffers::UIFramebuffers()
{
}


UIFramebuffers::~UIFramebuffers()
{
}