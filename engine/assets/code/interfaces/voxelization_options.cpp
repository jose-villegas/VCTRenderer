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

    if (Begin("Voxelization", &UIMainMenu::drawVoxelizationOptions,
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
            voxel.RevoxelizeScene();
        }

        static auto updateFrequency = 0;
        static auto revoxOnNeed = true;

        if(Checkbox("Update When Needed", &revoxOnNeed))
        {
            voxel.SetUpdateFrequency(-1);
        }

        if(!revoxOnNeed && InputInt("Update Frequency", &updateFrequency))
        {
            updateFrequency = glm::max(updateFrequency, 0);
            voxel.SetUpdateFrequency(updateFrequency);
        }

        Separator();
        static auto mipLevel = 0, direction = 0;
        static auto drawAlbedo = false;
        static auto drawNormals = false;
        static auto colors = glm::vec4(1.0f);
        auto maxLevel = log2(voxel.VolumeDimension()) - 1;

        if (DragFloat4("Color Channels", value_ptr(colors), 1.0f, 0.0f, 1.0f))
        {
            voxel.SetupDrawVoxels(mipLevel, drawAlbedo ? 8 : drawNormals
                                  ? 7 : direction, colors);
        }

        if(Checkbox("Draw Albedo", &drawAlbedo))
        {
            if (drawAlbedo && drawNormals) drawNormals = false;

            voxel.SetupDrawVoxels(mipLevel, drawAlbedo ? 8 : drawNormals
                                  ? 7 : direction, colors);
        }

        SameLine();

        if (Checkbox("Draw Normal (Visibility)", &drawNormals))
        {
            if (drawAlbedo && drawNormals) drawAlbedo = false;

            voxel.SetupDrawVoxels(mipLevel, drawNormals ? 7 : drawNormals
                                  ? 7 : direction, colors);
        }

        if(!drawAlbedo && !drawNormals)
        {
            if (SliderInt("Draw Mip Level", &mipLevel, 0, maxLevel))
            {
                voxel.SetupDrawVoxels(mipLevel, direction, colors);
            }

            if (SliderInt("Draw Mip Direction", &direction, 0, 5))
            {
                voxel.SetupDrawVoxels(mipLevel, direction, colors);
            }
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