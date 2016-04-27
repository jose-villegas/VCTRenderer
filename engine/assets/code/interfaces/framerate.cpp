#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "framerate.h"

#include "main_menu.h"
#include <array>

using namespace ImGui;

void UIFramerate::Draw()
{
    if (!UIMainMenu::drawFramerate) { return; }

    static auto openFPSWindow = false;
    static auto position = ImVec2(io->DisplaySize.x - 80 - 3,
                                  io->DisplaySize.y - 50 - 3);
    SetNextWindowPos(position);

    if (Begin("Performance Window", nullptr, ImGuiWindowFlags_NoTitleBar |
              ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        Text("Framerate");
        Separator();
        Text("%.3f", io->Framerate);
    }

    position = GetWindowPos();
    End();
}

UIFramerate::UIFramerate()
{
}

UIFramerate::~UIFramerate()
{
}

void UIMetrics::Draw()
{
    if (!UIMainMenu::showMetrics) { return; }

    static auto pause = false;
    static std::array<float, 8> values;

    if(!pause)
    {
        values[0] = ClearStatic.value;
        values[1] = VoxelizationStatic.value;
        values[2] = ClearDynamic.value;
        values[3] = VoxelizationDynamic.value;
        values[4] = RadianceInjection.value;
        values[5] = MipmappingRadiance.value;
        values[6] = BounceInjection.value;
        values[7] = MipmappingBounce.value;
    }

    if(Begin("Performance Analysis"))
    {
        if(Button("Reset"))
        {
            ClearStatic.Reset();
            VoxelizationStatic.Reset();
            ClearDynamic.Reset();
            VoxelizationDynamic.Reset();
            RadianceInjection.Reset();
            MipmappingRadiance.Reset();
            BounceInjection.Reset();
            MipmappingBounce.Reset();
        }

        SameLine();
        Checkbox("Pause", &pause);
        Text("Clear Static Voxels: %f", values[0]);
        Text("Static Voxelization: %f", values[1]);
        Text("Clear Dynamic Voxels: %f", values[2]);
        Text("Dynamic Voxelization: %f", values[3]);
        Text("Radiance Injection: %f", values[4]);
        Text("Mipmapping Radiance: %f", values[5]);
        Text("Inject First Bounce: %f", values[6]);
        Text("Mipmapping Radiance (Bounce): %f", values[7]);
    }

    End();
}

MovingAverage UIMetrics::ClearStatic;
MovingAverage UIMetrics::VoxelizationStatic;
MovingAverage UIMetrics::ClearDynamic;
MovingAverage UIMetrics::VoxelizationDynamic;
MovingAverage UIMetrics::RadianceInjection;
MovingAverage UIMetrics::MipmappingRadiance;
MovingAverage UIMetrics::BounceInjection;
MovingAverage UIMetrics::MipmappingBounce;