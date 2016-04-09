#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shadowing_options.h"

#include "main_menu.h"
#include "../../../core/assets_manager.h"
#include "../renderers/shadow_map_renderer.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>


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
        static auto selectedH = 5, selectedW = 5;
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
        static std::vector<const char *> filtersStr =
        {
            "None", "Bilinear", "Trilinear"
        };
        // shadow map quality
        Text("Shadowmap Resolution");

        if(Combo("Height: ", &selectedH, sizesStr.data(), sizesStr.size()))
        {
            shadowRender.SetupFramebuffers(sizes[selectedW], sizes[selectedH]);
        }

        if(Combo("Width: ", &selectedW, sizesStr.data(), sizesStr.size()))
        {
            shadowRender.SetupFramebuffers(sizes[selectedW], sizes[selectedH]);
        }

        // shadow map blurring / filtering
        static auto blurScale = 0.5f;
        static auto blurQuality = 1;
        static auto filtering = 1;
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

        if (Combo("Filtering", &filtering, filtersStr.data(), filtersStr.size()))
        {
            shadowRender.Filtering(filtering);
        }

        // vsm and evsm setup
        static auto exponents = glm::vec2(0);
        static auto reduction = 0.0f;
        exponents = shadowRender.Exponents();
        reduction = shadowRender.LightBleedingReduction();

        if(SliderFloat("Light Bleeding Reduction", &reduction, 0.0f, 1.0f))
        {
            shadowRender.LightBleedingReduction(reduction);
        }

        if(DragFloat2("Exponents", value_ptr(exponents)))
        {
            shadowRender.Exponents(exponents);
        }
    }

    End();
}
UIShadowingOptions::UIShadowingOptions()
{
}
UIShadowingOptions::~UIShadowingOptions()
{
}