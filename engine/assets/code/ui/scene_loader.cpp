#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_loader.h"

#include "../../../scene/camera.h"
#include "../../../scene/scene.h"
#include "../../../core/engine_assets.h"

bool SceneName(void * data, int idx, const char ** out_text)
{
    auto items = static_cast<std::shared_ptr<Scene> *>(data);

    if (out_text)
    {
        *out_text = items[idx]->GetFilepath().c_str();
    }

    return true;
}

void UISceneLoader::Draw()
{
    static auto &assets = AssetsManager::Instance();
    static auto activeScene = -1;
    ImGui::SetNextWindowPos(ImVec2(3, 3));
    ImGui::Begin("Import Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize |
                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
    // active scene selector

    if (ImGui::Combo("Path", &activeScene, SceneName, assets->scenes.data(),
                     static_cast<int>(assets->scenes.size())))
    {
        assets->scenes[activeScene]->SetAsActive();
        assets->scenes[activeScene]->cameras.front()->SetAsActive();
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") && Scene::Active())
    {
        Scene::Active()->Load();
    }

    uiPos = ImGui::GetWindowPos();
    uiSize = ImGui::GetWindowSize();
    ImGui::End();
}

UISceneLoader::UISceneLoader()
{
}

UISceneLoader::~UISceneLoader()
{
}