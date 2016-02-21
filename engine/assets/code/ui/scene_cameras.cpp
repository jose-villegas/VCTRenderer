#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_cameras.h"

#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/camera.h"
#include "../../../util/const_definitions.h"

#include "glm/detail/type_vec.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace ImGui;

void UISceneCameras::Draw()
{
    if (!UIMainMenu::drawSceneCameras) { return; }

    static auto &scene = Scene::Active();
    static auto &camera = Camera::Active();

    if (!scene || !camera) { return; }

    static auto &selected = *camera;
    static auto position = selected.transform.Position();
    static auto angles = selected.transform.Angles();
    // begin editor
    Begin("Cameras");
    Columns(2);

    for (int i = 0; i < scene->cameras.size(); i++)
    {
        auto &current = scene->cameras[i];
        PushID(i);

        // selected becomes the clicked selectable
        if (Selectable(current->name.c_str()))
        {
            selected = *current;
            position = selected.transform.Position();
            angles = selected.transform.Angles();
        }

        PopID();
    }

    NextColumn();

    if (DragFloat3("Position", value_ptr(position)))
    {
        selected.transform.Position(position);
    }

    if ( DragFloat3("Rotation", value_ptr(angles)))
    {
        selected.transform.Rotation(glm::quat(radians(angles)));
    }

    End();
}

UISceneCameras::UISceneCameras()
{
}

UISceneCameras::~UISceneCameras()
{
}
