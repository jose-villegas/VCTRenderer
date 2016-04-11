#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_materials.h"
#include "main_menu.h"
#include "../../../scene/scene.h"
#include "../../../scene/material.h"
#include <glm/gtc/type_ptr.hpp>

using namespace ImGui;

void UISceneMaterials::Draw()
{
    if (!UIMainMenu::drawSceneMaterials) { return; }

    static auto scene = static_cast<Scene *>(nullptr);
    static auto material = static_cast<Material *>(nullptr);
    // control variables
    static auto selected = -1;
    static glm::vec3 ambient;
    static glm::vec3 specular;
    static glm::vec3 diffuse;
    static float shininess;
    static std::vector<char> name;

    // active scene changed
    if (scene != Scene::Active().get())
    {
        scene = Scene::Active().get();
        selected = -1;
        material = nullptr;
    }

    // no active scene
    if (!scene) { return; }

    // begin editor
    Begin("Materials", &UIMainMenu::drawSceneMaterials);
    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    Columns(2);

    for (auto i = 0; i < scene->materials.size(); i++)
    {
        auto &current = scene->materials[i];
        PushID(i);
        BeginGroup();

        // selected becomes the clicked selectable
        if (Selectable(current->name.c_str(), i == selected))
        {
            selected = i;
            material = current.get();
            ambient = material->Ambient();
            specular = material->Specular();
            diffuse = material->Diffuse();
            shininess = 1.0f - material->Shininess();
            // copy name to a standard vector
            name.clear();
            copy(material->name.begin(), material->name.end(), back_inserter(name));
            name.push_back('\0');
        }

        EndGroup();
        PopID();
    }

    NextColumn();

    if (selected >= 0 && material != nullptr)
    {
        if(InputText("Name", name.data(), name.size()))
        {
            material->name = std::string(name.data());
        }

        if(ColorEdit3("Ambient", value_ptr(ambient)))
        {
            material->Ambient(ambient);
        }

        if (ColorEdit3("Diffuse", value_ptr(diffuse)))
        {
            material->Diffuse(diffuse);
        }

        if (ColorEdit3("Specular", value_ptr(specular)))
        {
            material->Specular(specular);
        }

        if (DragFloat("Shininess", &shininess, 0.001f, 0.0f, 1.0f))
        {
            material->Shininess(1.0f - shininess);
        }
    }
    else
    {
        Text("No Material Selected");
    }

    PopStyleVar();
    End();
}

void UISceneNodes::Draw()
{
    if (!UIMainMenu::drawSceneNodes) { return; }

    static auto scene = static_cast<Scene *>(nullptr);
    static auto node = static_cast<Node *>(nullptr);
    // control variables
    static auto selected = -1;
    static glm::vec3 position;
    static glm::vec3 rotation;
    static glm::vec3 scale;
    static std::vector<char> name;

    // active scene changed
    if (scene != Scene::Active().get())
    {
        scene = Scene::Active().get();
        selected = -1;
        node = nullptr;
    }

    // no active scene
    if (!scene) { return; }

    // begin editor
    Begin("Nodes", &UIMainMenu::drawSceneNodes);
    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    Columns(2);

    for (auto i = 0; i < scene->rootNode->DrawListNodes().size(); i++)
    {
        auto &current = scene->rootNode->DrawListNodes()[i];
        PushID(i);
        BeginGroup();

        // selected becomes the clicked selectable
        if (Selectable(current->name.c_str(), i == selected))
        {
            selected = i;
            node = current;
            position = node->Position();
            rotation = degrees(node->Angles());
            scale = node->Scale();
            // copy name to a standard vector
            name.clear();
            copy(node->name.begin(), node->name.end(), back_inserter(name));
            name.push_back('\0');
        }

        EndGroup();
        PopID();
    }

    NextColumn();

    if (selected >= 0 && node != nullptr)
    {
        if (InputText("Name", name.data(), name.size()))
        {
            node->name = std::string(name.data());
        }

        if (DragFloat3("Position", value_ptr(position)))
        {
            node->Position(position);
        }

        if (DragFloat3("Rotation", value_ptr(rotation)))
        {
            node->Rotation(radians(rotation));
        }

        if (DragFloat3("Scale", value_ptr(scale)))
        {
            node->Scale(scale);
        }
    }
    else
    {
        Text("No Node Selected");
    }

    PopStyleVar();
    End();
}
