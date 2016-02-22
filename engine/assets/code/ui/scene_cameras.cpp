#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_cameras.h"

#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

using namespace ImGui;

void UISceneCameras::Draw()
{
    if (!UIMainMenu::drawSceneCameras) { return; }

    static auto scenesCamera = std::unordered_map<Scene *, int>();
    static auto scene = static_cast<Scene *>(nullptr);
    static auto camera = static_cast<Camera *>(nullptr);
    // control variables
    static auto selected = -1;
    static glm::vec3 position;
    static glm::vec3 angles;
    static glm::vec3 proj;
    static std::vector<char> name;

    // active scene changed
    if (scene != Scene::Active().get())
    {
        scene = Scene::Active().get();

        // this scene is not logged in the hash map
        if (scenesCamera.find(scene) == scenesCamera.end())
        {
            if (scene->cameras.size() > 0)
            {
                scene->cameras[0]->SetAsActive();
                scenesCamera[scene] = 0;
            }
        }
        else if (scene->cameras.size() > scenesCamera[scene])
        {
            scene->cameras[scenesCamera[scene]]->SetAsActive();
        }

        selected = -1;
        camera == nullptr;
    }

    // no active scene
    if (!scene) { return; }

    // begin editor
    Begin("Cameras", &UIMainMenu::drawSceneCameras,
          ImGuiWindowFlags_AlwaysAutoResize);
    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    Columns(2);

    if (Button("New Camera"))
    {
        scene->cameras.push_back(std::make_shared<Camera>());

        // just added a camera to a scene with no cameras
        // thus make it default as active
        if (scene->cameras.size() == 1)
        {
            scene->cameras[0]->SetAsActive();
        }
    }

    for (int i = 0; i < scene->cameras.size(); i++)
    {
        auto &current = scene->cameras[i];
        PushID(i);
        BeginGroup();

        // selected becomes the clicked selectable
        if (Selectable(current->name.c_str(), i == selected))
        {
            selected = i;
            camera = current.get();
            position = camera->transform.Position();
            angles = degrees(camera->transform.Angles());
            proj[0] = camera->HorizontalFoV();
            proj[1] = camera->ClipPlaneNear();
            proj[2] = camera->ClipPlaneFar();
            // copy name to a standard vector
            name.clear();
            copy(camera->name.begin(), camera->name.end(), back_inserter(name));
            name.push_back('\0');
        }

        if (current->IsActive())
        {
            SameLine();
            Bullet();
        }

        EndGroup();
        PopID();
    }

    NextColumn();

    if (selected >= 0 && camera != nullptr)
    {
        if (InputText("Name", name.data(), name.size()))
        {
            camera->name = std::string(name.data());
        }

        if (DragFloat3("Position", value_ptr(position), 0.1))
        {
            camera->transform.Position(position);
        }

        if (DragFloat3("Rotation", value_ptr(angles), 0.1))
        {
            camera->transform.Rotation(radians(angles));
        }

        if (SliderFloat("Field of View", &proj[0], 1.0f, 179.0f))
        {
            camera->HorizontalFoV(proj[0]);
        }

        Text("Clipping Planes");
        Indent();

        if (DragFloat("Near", &proj[1], 0.1f, 0.01f))
        {
            proj[1] = glm::max(proj[1], 0.01f);
            camera->ClipPlaneNear(proj[1]);
        }

        if (DragFloat("Far", &proj[2], 0.1f, 0.01f))
        {
            proj[2] = glm::max(proj[2], 0.01f);
            camera->ClipPlaneFar(proj[2]);
        }

        Unindent();

        if (Button("Set as Active"))
        {
            camera->SetAsActive();
            // save currently active camera index
            scenesCamera[scene] = selected;
        }

        SameLine();

        if (Button("Delete"))
        {
            // temporal shared ptr ref
            auto toDelete = scene->cameras[selected];
            // delete ref in scene cameras
            scene->cameras.erase(scene->cameras.begin() + selected);

            // in case the camera deleted was marked
            // as active, set another camera as active
            if (scene->cameras.size() > 0 && toDelete->IsActive())
            {
                scene->cameras[0]->SetAsActive();
                selected = 0;
            }
        }
    }
    else
    {
        Text("No Camera Selected");
    }

    PopStyleVar();
    ImGui::End();
}
UISceneCameras::UISceneCameras()
{
}
UISceneCameras::~UISceneCameras()
{
}