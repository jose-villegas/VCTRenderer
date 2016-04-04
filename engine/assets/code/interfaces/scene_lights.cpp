#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_lights.h"

#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/light.h"
#include "../../../core/assets_manager.h"
#include "../renderers/shadow_map_renderer.h"

#include <glm/gtc/type_ptr.hpp>

using namespace ImGui;

void UISceneLights::Draw()
{
    static auto &assets = AssetsManager::Instance();
    static auto shadowing = static_cast<ShadowMapRenderer *>
                            (assets->renderers["Shadowmapping"].get());
    static auto scene = static_cast<Scene *>(nullptr);
    static auto light = static_cast<Light *>(nullptr);
    static auto casters = std::map<Scene *, Light *>();
    // control variables
    static auto selected = -1;
    static glm::vec3 position;
    static glm::vec3 angles;
    static glm::vec3 color[3];
    static glm::vec3 attenuation;
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

        // no shadow caster assigned
        if(casters.find(scene) == casters.end())
        {
            for (auto &l : scene->lights)
            {
                if (l->Type() == Light::Directional)
                {
                    // store shadow caster
                    casters[scene] = l.get();
                    // assign to shadow renderer
                    shadowing->Caster(casters[scene]);
                    break;
                }
            }
        }
        else if(casters[scene] != nullptr)
        {
            shadowing->Caster(casters[scene]);
        }
    }

    if (!scene) { return; }

    if (!UIMainMenu::drawSceneLights) { return; }

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
            intensities = light->Intensities();
            type = light->Type();
            cone[0] = glm::degrees(light->AngleInnerCone());
            cone[1] = glm::degrees(light->AngleOuterCone());
            attenuation[0] = light->attenuation.Constant();
            attenuation[1] = light->attenuation.Linear();
            attenuation[2] = light->attenuation.Quadratic();
            // copy name to a standard vector
            name.clear();
            copy(light->name.begin(), light->name.end(), back_inserter(name));
            name.push_back('\0');
        }

        EndGroup();
        PopID();
    }

    NextColumn();

    if (selected >= 0 && light != nullptr)
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

            if (SliderFloat("Outer", &cone[1], 1.0f, 179.0f))
            {
                light->AngleOuterCone(glm::radians(cone[1]));
                cone[0] = glm::min(cone[0], cone[1]);
            }

            if (SliderFloat("Inner", &cone[0], 1.0f, cone[1]))
            {
                light->AngleInnerCone(glm::radians(cone[0]));
            }

            Unindent();
        }

        // attenuation controls
        if (type != Light::Directional)
        {
            Text("Attenuation");
            Indent();

            if (DragFloat("Constant", &attenuation[0], 0.01f))
            {
                attenuation[0] = glm::max(0.0f, attenuation[0]);
                light->attenuation.Constant(attenuation[0]);
            }

            if (DragFloat("Linear", &attenuation[1], 0.01f))
            {
                attenuation[1] = glm::max(0.0f, attenuation[1]);
                light->attenuation.Linear(attenuation[1]);
            }

            if (DragFloat("Quadratic", &attenuation[2], 0.01f))
            {
                attenuation[2] = glm::max(0.0f, attenuation[2]);
                light->attenuation.Quadratic(attenuation[2]);
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

        if (SliderFloat3("Intensities", value_ptr(intensities), 0.0f, 16.0f))
        {
            light->Intensities(intensities);
        }

        Unindent();

        if (Button("Delete"))
        {
            // stored as shadow caster also
            if (light == casters[scene])
            {
                casters[scene] = nullptr;
                shadowing->Caster(nullptr);
            }

            // delete ref in scene lights
            scene->lights.erase(scene->lights.begin() + selected);
            light = nullptr;
        }

        if (type == Light::Directional)
        {
            SameLine();

            if (Button("Cast Shadow"))
            {
                casters[scene] = light;
                shadowing->Caster(light);
            }
        }
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
