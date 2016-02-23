#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_lights.h"

#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/light.h"

#include <glm/gtc/type_ptr.hpp>

using namespace ImGui;

void UISceneLights::Draw()
{
    if (!UIMainMenu::drawSceneLights) { return; }

    static auto scene = static_cast<Scene *>(nullptr);
    static auto light = static_cast<Light *>(nullptr);
    // control variables
    static auto selected = -1;
    static glm::vec3 position;
    static glm::vec3 angles;
    static glm::vec3 color[3];
    static std::vector<char> name;

    // active scene changed
    if (scene != Scene::Active().get())
    {
        scene = Scene::Active().get();
        light = nullptr;
        selected = -1;
    }

    if (!scene) { return; }

    // begin editor
    Begin("Lights", &UIMainMenu::drawSceneLights,
          ImGuiWindowFlags_AlwaysAutoResize);
    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    Columns(2);

    if (Button("New Light"))
    {
        scene->lights.push_back(std::make_shared<Light>());
    }

    for (auto i = 0; i < scene->lights.size(); i++)
    {
        auto &current = scene->lights[i];
        PushID(i);
        BeginGroup();

        // selected becomes the clicked selectable
        if (Selectable(current->name.c_str(), i == selected))
        {
            selected = i;
            light = current.get();
            position = light->transform.Position();
            angles = degrees(light->transform.Angles());
            color[0] = light->Ambient();
            color[1] = light->Diffuse();
            color[2] = light->Specular();
            // copy name to a standard vector
            name.clear();
            copy(light->name.begin(), light->name.end(), back_inserter(name));
            name.push_back('\0');
        }

        EndGroup();
        PopID();
    }

    NextColumn();

    if (selected >= 0)
    {
        if (InputText("Name", name.data(), name.size()))
        {
            light->name = std::string(name.data());
        }

        if (DragFloat3("Position", value_ptr(position), 0.1f))
        {
            light->transform.Position(position);
        }

        if (DragFloat3("Rotation", value_ptr(angles), 0.1f))
        {
            light->transform.Rotation(radians(angles));
        }

        Text("Color");
        Indent();

        if (ColorEdit3("Ambient", value_ptr(color[0])))
        {
            light->Ambient(color[0]);
        }

        if (ColorEdit3("Diffuse", value_ptr(color[1])))
        {
            light->Diffuse(color[1]);
        }

        if (ColorEdit3("Specular", value_ptr(color[2])))
        {
            light->Specular(color[2]);
        }

        Unindent();
    }
    else
    {
        Text("No Light Selected");
    }

    PopStyleVar();
    End();
}

UISceneLights::UISceneLights()
{
}

UISceneLights::~UISceneLights()
{
}
