#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "main_menu.h"

#include "../renderers/voxelizer_renderer.h"

bool UIMainMenu::drawSceneLoader = true;
bool UIMainMenu::drawFramerate = false;
bool UIMainMenu::drawSceneCameras = false;
bool UIMainMenu::drawSceneLights = false;
bool UIMainMenu::drawFramebuffers = false;
bool UIMainMenu::drawShadowOptions = false;
bool UIMainMenu::drawVoxelizationOptions = false;
bool UIMainMenu::drawGIOptions = false;
bool UIMainMenu::drawSceneMaterials = false;

using namespace ImGui;

void UIMainMenu::Draw()
{
    if (BeginMainMenuBar())
    {
        if (BeginMenu("View"))
        {
            MenuItem("Scene Loader", nullptr, &drawSceneLoader);
            MenuItem("Show Framerate", nullptr, &drawFramerate);
            MenuItem("View Voxels", nullptr, &VoxelizerRenderer::ShowVoxels);
            MenuItem("View Framebuffers", nullptr, &drawFramebuffers);
            ImGui::EndMenu();
        }

        if (BeginMenu("Scene"))
        {
            MenuItem("Cameras", nullptr, &drawSceneCameras);
            MenuItem("Lights", nullptr, &drawSceneLights);
            MenuItem("Materials", nullptr, &drawSceneMaterials);
            ImGui::EndMenu();
        }

        if (BeginMenu("Options"))
        {
            MenuItem("Shadows", nullptr, &drawShadowOptions);
            MenuItem("Voxelization", nullptr, &drawVoxelizationOptions);
            MenuItem("Global Illumination", nullptr, &drawGIOptions);
            ImGui::EndMenu();
        }

        EndMainMenuBar();
    }
}

UIMainMenu::UIMainMenu()
{
}

UIMainMenu::~UIMainMenu()
{
}