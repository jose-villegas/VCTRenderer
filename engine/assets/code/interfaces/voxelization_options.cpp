#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxelization_options.h"

#include "main_menu.h"
#include "../../../core/assets_manager.h"
#include "../renderers/voxelizer_renderer.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>


using namespace ImGui;

void UIVoxelizationOptions::Draw()
{
    if (!UIMainMenu::drawVoxelizationOptions) { return; }

    static auto &assets = AssetsManager::Instance();
    static auto &voxel = *static_cast<VoxelizerRenderer *>
                         (assets->renderers["Voxelizer"].get());

    if (Begin("Voxelization", &UIMainMenu::drawShadowOptions,
              ImGuiWindowFlags_AlwaysAutoResize))
    {
        static auto texRes = 5;
        static std::vector<int> sizes =
        {
            8, 16, 32, 64, 128, 256, 512
        };
        static std::vector<const char *> sizesStr =
        {
            "8", "16", "32", "64", "128", "256", "512"
        };
        // shadow map quality
        BeginGroup();
        Text("Volume Resolution");

        if(Combo("Resolution: ", &texRes, sizesStr.data(), sizesStr.size()))
        {
            voxel.SetupVoxelVolumes(sizes[texRes]);
        }

        static auto updateFrequency = 5;

        if(InputInt("Update Frequency", &updateFrequency))
        {
            voxel.SetUpdateFrequency(updateFrequency);
        }

        if(Button("Voxelize Now"))
        {
            voxel.RevoxelizeScene();
        }

        EndGroup();
    }

    End();
}
UIVoxelizationOptions::UIVoxelizationOptions()
{
}
UIVoxelizationOptions::~UIVoxelizationOptions()
{
}