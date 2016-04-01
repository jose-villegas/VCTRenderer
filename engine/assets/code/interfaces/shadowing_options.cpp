#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shadowing_options.h"

#include "main_menu.h"
#include "../../../core/assets_manager.h"
#include "../renderers/shadow_map_renderer.h"

#include <string>


using namespace ImGui;

void UIShadowingOptions::Draw()
{
    if (!UIMainMenu::drawShadowOptions) { return; }

    static auto &assets = AssetsManager::Instance();
    static auto &shadowRender = *static_cast<ShadowMapRenderer *>
                                (assets->renderers["Shadowmapping"].get());

    if (Begin("Shadows", &UIMainMenu::drawShadowOptions,
              ImGuiWindowFlags_AlwaysAutoResize))
    {
        static auto selectedH = 6, selectedW = 6;
        static std::vector<int> sizes =
        {
            32, 64, 128, 256, 512,
            1024, 2048, 4096, 8192
        };
        static std::vector<const char *> sizesStr =
        {
            "32", "64", "128", "256", "512",
            "1024", "2048", "4096", "8192"
        };
        // shadow map quality
        BeginGroup();
        Text("Shadowmap Resolution");

        if(Combo("Height: ", &selectedH, sizesStr.data(), sizesStr.size()))
        {
            shadowRender.SetupFramebuffers(sizes[selectedW], sizes[selectedH]);
        }

        if(Combo("Width: ", &selectedW, sizesStr.data(), sizesStr.size()))
        {
            shadowRender.SetupFramebuffers(sizes[selectedW], sizes[selectedH]);
        }

        EndGroup();
        // shadow map blurring / filtering
        BeginGroup();
        static auto blurScale = 2.0f;
        static auto blurQuality = 1;
        static auto aniso = 8;

        if(SliderFloat("Blur Scale", &blurScale, 0.0f, 8.0f))
        {
            shadowRender.BlurScale(blurScale);
        }

        if (SliderInt("Blur Quality", &blurQuality, 1, 3))
        {
            shadowRender.BlurQuality(blurQuality);
        }

        if (SliderInt("Anisotropic Filtering", &aniso, 0, 16))
        {
            shadowRender.Anisotropy(aniso);
        }

        EndGroup();
    }

    End();
}
UIShadowingOptions::UIShadowingOptions()
{
}
UIShadowingOptions::~UIShadowingOptions()
{
}