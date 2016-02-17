#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_cameras.h"

#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/camera.h"

#include "glm/detail/type_vec.hpp"
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ImGui;

glm::quat LookRotation(const glm::vec3 &lookAt, const glm::vec3 &up)
{
    auto forward = normalize(lookAt);
    forward = orthonormalize(forward, up);
    auto right = cross(up, forward);
    glm::quat ret;
    ret.w = sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
    auto w4_recip = 1.0f / (4.0f * ret.w);
    ret.x = (up.z - forward.y) * w4_recip;
    ret.y = (forward.x - right.z) * w4_recip;
    ret.z = (right.y - up.x) * w4_recip;
    return ret;
}

void UISceneCameras::Draw()
{
    ////if (!UIMainMenu::drawSceneCameras) { return; }
    //static auto &scene = Scene::Active();
    //static auto &camera = Camera::Active();
    //if (!scene || !camera) { return; }
    //static auto &selected = *camera;
    //static auto position = selected.Position();
    //static auto lookat = selected.LookAt();
    //static auto rotation = LookRotation(lookat, selected.Up());
    //static auto angles = eulerAngles(rotation);
    //// begin editor
    //Begin("Cameras");
    //Columns(2);
    //for (int i = 0; i < scene->cameras.size(); i++)
    //{
    //    auto &current = scene->cameras[i];
    //    PushID(i);
    //    // selected becomes the clicked selectable
    //    if (Selectable(current->name.c_str()))
    //    {
    //        selected = *current;
    //        lookat = selected.LookAt();
    //        position = selected.Position();
    //        rotation = LookRotation(lookat, selected.Up());
    //        angles = eulerAngles(rotation);
    //    }
    //    PopID();
    //}
    //NextColumn();
    //DragFloat3("Position", value_ptr(position));
    //DragFloat3("Rotation", value_ptr(angles));
    //selected.Position(position);
    //selected.LookAt(position + lookat * glm::quat(radians(angles)));
    //End();
}

UISceneCameras::UISceneCameras()
{
}

UISceneCameras::~UISceneCameras()
{
}
