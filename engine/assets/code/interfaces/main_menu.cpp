#include <GL/glew.h>
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
bool UIMainMenu::drawSceneNodes = false;
bool UIMainMenu::showMetrics = false;

using namespace ImGui;

void UIMainMenu::Draw()
{
    static bool showAbout = false;

    if (BeginMainMenuBar())
    {
        if (BeginMenu("View"))
        {
            MenuItem("Scene Loader", nullptr, &drawSceneLoader);
            MenuItem("Show Framerate", nullptr, &drawFramerate);
            MenuItem("Show Metrics", nullptr, &showMetrics);
            MenuItem("View Voxels", nullptr, &VoxelizerRenderer::ShowVoxels);
            MenuItem("View Framebuffers", nullptr, &drawFramebuffers);
            EndMenu();
        }

        if (BeginMenu("Scene"))
        {
            MenuItem("Cameras", nullptr, &drawSceneCameras);
            MenuItem("Lights", nullptr, &drawSceneLights);
            MenuItem("Materials", nullptr, &drawSceneMaterials);
            MenuItem("Shapes", nullptr, &drawSceneNodes);
            EndMenu();
        }

        if (BeginMenu("Options"))
        {
            MenuItem("Shadows", nullptr, &drawShadowOptions);
            MenuItem("Voxelization", nullptr, &drawVoxelizationOptions);
            MenuItem("Global Illumination", nullptr, &drawGIOptions);
            MenuItem("About", nullptr, &showAbout);
            EndMenu();
        }

        EndMainMenuBar();
    }

    if(showAbout)
    {
        if (Begin("Name", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Text("Developer: Jose Villegas.");
            Text("Developer: villegasjose.gg@gmail.com");
        }

        End();
    }
}

UIMainMenu::UIMainMenu()
{
}

UIMainMenu::~UIMainMenu()
{
}