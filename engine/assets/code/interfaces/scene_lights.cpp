#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_lights.h"

#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/light.h"
#include "../../../core/assets_manager.h"
#include "../renderers/shadow_map_renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include "../renderers/voxelizer_renderer.h"

using namespace ImGui;

void UISceneLights::Draw()
{
    static auto &assets = AssetsManager::Instance();
    static auto shadowing = static_cast<ShadowMapRenderer *>
                            (assets->renderers["Shadowmapping"].get());
    static auto &voxel = *static_cast<VoxelizerRenderer *>
                         (assets->renderers["Voxelizer"].get());
    static auto scene = static_cast<Scene *>(nullptr);
    static auto light = static_cast<Light *>(nullptr);
    static auto shadowingMethod = int(0);
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
    }

    if (!scene) { return; }

    if (!UIMainMenu::drawSceneLights) { return; }

    // begin editor
    Begin("Lights", &UIMainMenu::drawSceneLights);
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
            shadowingMethod = light->mode[0].to_ulong();
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
                light->Position(position);
            }
        }

        if (type != Light::Point)
        {
            if (DragFloat3("Rotation", value_ptr(angles), 0.1f))
            {
                light->Rotation(radians(angles));
            }
        }

        if (Combo("Type", reinterpret_cast<int *>(&type), "Directional\0Point\0Spot"))
        {
            light->TypeCollection(type);
            light->RegisterChange();
        }

        if (type == Light::Spot)
        {
            Text("Cone Angle");
            Indent();

            if (SliderFloat("Outer", &cone[1], 1.0f, 179.0f))
            {
                light->AngleOuterCone(glm::radians(cone[1]));
                light->RegisterChange();
                cone[0] = glm::min(cone[0], cone[1]);
            }

            if (SliderFloat("Inner", &cone[0], 1.0f, cone[1]))
            {
                light->AngleInnerCone(glm::radians(cone[0]));
                light->RegisterChange();
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
                light->RegisterChange();
            }

            if (DragFloat("Linear", &attenuation[1], 0.01f))
            {
                attenuation[1] = glm::max(0.0f, attenuation[1]);
                light->attenuation.Linear(attenuation[1]);
                light->RegisterChange();
            }

            if (DragFloat("Quadratic", &attenuation[2], 0.01f))
            {
                attenuation[2] = glm::max(0.0f, attenuation[2]);
                light->attenuation.Quadratic(attenuation[2]);
                light->RegisterChange();
            }

            Unindent();
        }

        Text("Color");
        Indent();

        if (ColorEdit3("Ambient", value_ptr(color[0])))
        {
            light->Ambient(color[0]);
            light->RegisterChange();
        }

        if (ColorEdit3("Diffuse", value_ptr(color[1])))
        {
            light->Diffuse(color[1]);
            light->RegisterChange();
        }

        if (ColorEdit3("Specular", value_ptr(color[2])))
        {
            light->Specular(color[2]);
            light->RegisterChange();
        }

        if (SliderFloat3("Intensities", value_ptr(intensities), 0.0f, 16.0f))
        {
            light->Intensities(intensities);
            light->RegisterChange();
        }

        Unindent();

        if (RadioButton("No Shadows", &shadowingMethod, 0))
        {
            light->mode[0].reset();
            light->RegisterChange();

            if (light == shadowing->Caster()) shadowing->Caster(nullptr);
        }

        if (light->Type() == Light::Directional &&
                RadioButton("Shadow Mapping", &shadowingMethod, 1))
        {
            shadowing->Caster(light);
            light->mode[0].reset();
            light->mode[0].set(0, 1);
            light->RegisterChange();

            // there can be only one with shadow mapping
            for (auto &l : scene->lights)
            {
                if (l.get() != light && l->mode[0][0])
                {
                    l->mode[0].reset();
                }
            }
        }

        if (RadioButton("Trace Shadow Cones", &shadowingMethod, 2))
        {
            light->mode[0].reset();
            light->mode[0].set(1, 1);
            light->RegisterChange();

            if (light == shadowing->Caster()) shadowing->Caster(nullptr);
        }

        if (Button("Delete"))
        {
            // method 1 indicates this light is used for shadow mapping
            if(light->mode[0][0])
            {
                // no shadowing from this light source
                light->mode[0].reset();
                shadowing->Caster(nullptr);
            }

            light = nullptr;
            // delete from scene
            scene->lights.erase(scene->lights.begin() + selected);
            voxel.UpdateRadiance();
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
