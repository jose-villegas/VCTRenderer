#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "geometrybuffer_textures.h"
#include "main_menu.h"
#include "../../../rendering/deferred_renderer.h"
#include "../../../types/geometry_buffer.h"

#include <oglplus/texture.hpp>

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
        auto width = tex.Width(TextureTarget::_2D, 0);
        auto height = tex.Height(TextureTarget::_2D, 0);
        BeginTooltip();
        Image(texName, ImVec2(width / 2, height / 2), uv1, uv2);
        EndTooltip();
    }

    EndGroup();
}

void UIGeometryBuffer::Draw()
{
    if (!UIMainMenu::drawGeometryBuffer) { return; }

    auto &gbuffer = DeferredRenderer::GBuffer();
    // begin editor
    Begin("Geometry Buffer", &UIMainMenu::drawGeometryBuffer,
          ImGuiWindowFlags_AlwaysAutoResize);
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Normal), "Normal");
    SameLine();
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Albedo), "Albedo");
    SameLine();
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Specular), "Specular");
    SameLine();
    DrawBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Depth), "Depth");
    End();
}

UIGeometryBuffer::UIGeometryBuffer()
{
}


UIGeometryBuffer::~UIGeometryBuffer()
{
}
