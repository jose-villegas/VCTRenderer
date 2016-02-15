#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_cameras.h"

#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/camera.h"

using namespace ImGui;

void UISceneCameras::Draw()
{
    //if (!UIMainMenu::drawSceneCameras) { return; }
    static auto &scene = Scene::Active();
    static auto &camera = Camera::Active();

    if (!scene || !camera) { return; }

    Begin("Cameras");
    Columns(2);

    for (int i = 0; i < scene->cameras.size(); i++)
    {
        PushID(i);
        Text("Camera");
        PopID();
    }

    End();
}

UISceneCameras::UISceneCameras()
{
}

UISceneCameras::~UISceneCameras()
{
}
