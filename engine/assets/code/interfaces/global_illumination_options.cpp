#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "global_illumination_options.h"

#include "main_menu.h"
#include "../core/assets_manager.h"
#include "../renderers/voxelizer_renderer.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "../renderers/global_illumination_renderer.h"

using namespace ImGui;

void UIGlobalIllumination::Draw()
{
    if (!UIMainMenu::drawGIOptions) { return; }

    static auto &assets = AssetsManager::Instance();
    static auto &gi = *static_cast<GIRenderer *>
                      (assets->renderers["GlobalIllumination"].get());

    if (Begin("Global Illumination", &UIMainMenu::drawGIOptions,
              ImGuiWindowFlags_AlwaysAutoResize))
    {
        static auto maxTracingConeDistance = gi.MaxTracingDistance();
        static auto bounceStrength = gi.GlobalIlluminationStrength();
        static auto aoFalloff = gi.AmbientOclussionFalloff();
        static auto aoAlpha = gi.AmbientOclussionAlpha();
        static auto scale = 0.5f;
        static auto mode = 0;
        static auto modes = "Direct + Indirect + Occlusion\0Direct + Indirec" \
                            "t\0Direct\0Indirect\0Ambient Occlusion";
        static auto firstB = false;
        static auto &voxel = *static_cast<VoxelizerRenderer *>
                             (assets->renderers["Voxelizer"].get());

        if(SliderFloat("Maximum Trace Distance", &maxTracingConeDistance, 0.0f, 1.0f))
        {
            gi.MaxTracingDistance(maxTracingConeDistance);
        }

        if(SliderFloat("GI Strength", &bounceStrength, 0.0f, 32.0f))
        {
            gi.GlobalIlluminationStrength(bounceStrength);
        }

        if(SliderFloat("GI Texture Scale", &scale, 0.0f, 1.0f))
        {
            gi.GlobalIlluminationScale(scale);
        }

        if (Checkbox("Inject First Bounce", &firstB))
        {
            voxel.InjectFirstBounce(firstB);
        }

        /*        if(Combo("Render Mode", &mode, modes))
                {
                    deferred.RenderMode(mode);
                }*/
        Text("Ambient Occlusion");
        Indent();

        if (InputFloat("Falloff", &aoFalloff, 10))
        {
            gi.AmbientOclussionFalloff(aoFalloff);
        }

        if (SliderFloat("Alpha", &aoAlpha, 0.0f, 1.0f))
        {
            gi.AmbientOclussionAlpha(aoAlpha);
        }

        Unindent();
    }

    End();
}
UIGlobalIllumination::UIGlobalIllumination()
{
}
UIGlobalIllumination::~UIGlobalIllumination()
{
}