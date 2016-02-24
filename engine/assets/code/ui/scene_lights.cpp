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
    static glm::vec3 intensities;
    static glm::vec2 cone;
    static Light::LightType type;
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
            intensities = light->Intensity();
            type = light->Type();
            cone[0] = light->AngleInnerCone();
            cone[1] = light->AngleOuterCone();
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

        if (type != Light::Directional)
        {
            if (DragFloat3("Position", value_ptr(position), 0.1f))
            {
                light->transform.Position(position);
            }
        }

        if (type != Light::Point)
        {
            if (DragFloat3("Rotation", value_ptr(angles), 0.1f))
            {
                light->transform.Rotation(radians(angles));
            }
        }

        if (Combo("Type", reinterpret_cast<int *>(&type), "Directional\0Point\0Spot"))
        {
            light->TypeCollection(type);
        }

        if (type == Light::Spot)
        {
            Text("Cone Angle");
            Indent();

            if (SliderFloat("Oute", &cone[1], 1.0f, 179.0f))
            {
                light->AngleOuterCone(cone[1]);
                cone[0] = glm::min(cone[0], cone[1]);
            }

            if (SliderFloat("Inner", &cone[0], 1.0f, cone[1]))
            {
                light->AngleInnerCone(cone[0]);
            }

            Unindent();
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

        if (SliderFloat3("Intensities", value_ptr(intensities), 0.0f, 8.0f))
        {
            light->Intensity(intensities);
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
