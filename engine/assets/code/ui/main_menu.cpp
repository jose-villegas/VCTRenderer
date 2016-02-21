#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "main_menu.h"

bool UIMainMenu::drawSceneLoader = true;
bool UIMainMenu::drawFramerate = false;
bool UIMainMenu::drawSceneCameras = false;

using namespace ImGui;

void UIMainMenu::Draw()
{
    if (BeginMainMenuBar())
    {
        if (BeginMenu("View"))
        {
            MenuItem("Show Framerate", nullptr, &drawFramerate);
            EndMenu();
        }

        if (BeginMenu("Scene"))
        {
            MenuItem("Cameras", nullptr, &drawSceneCameras);
            MenuItem("Lights");
            MenuItem("Hierachy");
            EndMenu();
        }

        if (BeginMenu("Window"))
        {
            MenuItem("Scene Loader", nullptr, &drawSceneLoader);
            EndMenu();
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