#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_loader.h"

#include "main_menu.h"
#include "../../../scene/camera.h"
#include "../../../scene/scene.h"
#include "../../../core/assets_manager.h"

using namespace ImGui;

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
    if (!UIMainMenu::drawSceneLoader) { return; }

    static auto &assets = AssetsManager::Instance();
    static auto &scene = Scene::Active();
    static auto activeScene = -1;
    SetNextWindowPosCenter();

    if (Begin("Load Scene", &UIMainMenu::drawSceneLoader, ImGuiWindowFlags_NoMove |
              ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        // active scene selector
        PushItemWidth(350);

        if (Combo("Path", &activeScene, SceneName, assets->scenes.data(),
                  static_cast<int>(assets->scenes.size())))
        {
            assets->scenes[activeScene]->SetAsActive();
        }

        SameLine();

        if (Button("Load") && scene)
        {
            scene->Import();
            scene->Load();
        }

        PopItemWidth();
    }

    End();
}

UISceneLoader::UISceneLoader()
{
}

UISceneLoader::~UISceneLoader()
{
}