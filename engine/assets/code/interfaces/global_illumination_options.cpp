#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "global_illumination_options.h"

#include "main_menu.h"
#include "../core/assets_manager.h"
#include "../renderers/gi_deferred_renderer.h"
#include "../renderers/voxelizer_renderer.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>

using namespace ImGui;

void UIGlobalIllumination::Draw()
{
    if (!UIMainMenu::drawGIOptions) { return; }

    static auto &assets = AssetsManager::Instance();
    static auto &deferred = *static_cast<GIDeferredRenderer *>
                            (assets->renderers["Deferred"].get());

    if (Begin("Global Illumination", &UIMainMenu::drawGIOptions,
              ImGuiWindowFlags_AlwaysAutoResize))
    {
        static auto &voxel = *static_cast<VoxelizerRenderer *>
                             (assets->renderers["Voxelizer"].get());
        static auto maxTracingConeDistance = deferred.MaxTracingDistance();
        static auto bounceStrength = deferred.GlobalIlluminationStrength();
        static auto aoFalloff = deferred.AmbientOclussionFalloff();
        static auto aoAlpha = deferred.AmbientOclussionAlpha();
        static auto mode = 0;
        static auto modes = "Direct + Indirect + Occlusion\0Direct + Indirec" \
                            "t\0Direct\0Indirect\0Ambient Occlusion";
        static auto firstB = voxel.InjectFirstBounce();
        static auto conesS = voxel.TraceShadowCones();
        static auto checkB = deferred.CheckVolumeBoundaries();
        static auto samplingFact = deferred.SamplingFactor();
        static auto fCone = deferred.ConeShadowTolerance();
        static auto fConeAperture = glm::degrees(deferred.ConeShadowAperture());
        static auto shadwMethod = 0;

        if(SliderFloat("Maximum Trace Distance", &maxTracingConeDistance, 0.0f, 1.0f))
        {
            deferred.MaxTracingDistance(maxTracingConeDistance);
        }

        if(Checkbox("Check Scene Boundaries", &checkB))
        {
            deferred.CheckVolumeBoundaries(checkB);
        }

        if(SliderFloat("GI Strength", &bounceStrength, 0.0f, 32.0f))
        {
            deferred.GlobalIlluminationStrength(bounceStrength);
        }

        if (SliderFloat("Sampling Factor", &samplingFact, 0.1f, 2.5f))
        {
            deferred.SamplingFactor(glm::clamp(samplingFact, 0.1f, 2.5f));
        }

        if (Checkbox("Inject First Bounce", &firstB))
        {
            voxel.InjectFirstBounce(firstB);
            voxel.UpdateRadiance();
        }

        if (Checkbox("Inject Raytraced Shadows", &conesS))
        {
            voxel.TraceShadowCones(conesS);
            voxel.UpdateRadiance();
        }

        if(conesS)
        {
            Indent();
            static auto umbra = voxel.ShadowConeUmbra();

            if(SliderFloat("Tolerance", &umbra, 0.01f, 1.0f))
            {
                voxel.ShadowConeTolerance(umbra);
                voxel.UpdateRadiance();
            }

            Unindent();
        }

        if(Combo("Traced Shadow Cones", &shadwMethod,
                 "Trace Per Fragment\0Sample Shadow Volume\0", 2))
        {
            deferred.SampleVoxelShadowVolume(shadwMethod == 1);
        }

        if (shadwMethod == 0)
        {
            Indent();

            if (SliderFloat("Fragment Shadow Cone Tolerance",
                            &fCone, 0.0f, 1.0f))
            {
                deferred.ConeShadowTolerance(fCone);
            }

            if (SliderFloat("Fragment Shadow Cone Aperture",
                            &fConeAperture, 1.0f, 90.0f))
            {
                deferred.ConeShadowAperture(glm::radians(fConeAperture));
            }

            Unindent();
        }

        if(Combo("Render Mode", &mode, modes))
        {
            deferred.RenderMode(mode);
        }

        Text("Ambient Occlusion");
        Indent();

        if (InputFloat("Falloff", &aoFalloff, 10))
        {
            deferred.AmbientOclussionFalloff(aoFalloff);
        }

        if (SliderFloat("Alpha", &aoAlpha, 0.0f, 1.0f))
        {
            deferred.AmbientOclussionAlpha(aoAlpha);
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