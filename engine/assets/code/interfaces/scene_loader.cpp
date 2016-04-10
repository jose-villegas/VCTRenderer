#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "scene_loader.h"

#include "main_menu.h"
#include "../../../scene/camera.h"
#include "../../../scene/scene.h"
#include "../../../core/assets_manager.h"
#include "../../../scene/light.h"
#include "../renderers/shadow_map_renderer.h"

using namespace ImGui;

bool SceneName(void * data, int idx, const char ** out_text)
{
    auto items = static_cast<std::map<std::string,
         std::shared_ptr<Scene>> *>(data);

    if (out_text)
    {
        auto begin = items->begin();
        advance(begin, idx);
        *out_text = begin->first.c_str();
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
        PushItemWidth(200);

        if (Combo("Path", &activeScene, SceneName, &assets->scenes,
                  static_cast<int>(assets->scenes.size())))
        {
            auto begin = assets->scenes.begin();
            advance(begin, activeScene);
            begin->second->SetAsActive();
        }

        SameLine();

        if (Button("Load") && scene && !scene->IsLoaded())
        {
            scene->Import();
            scene->Load();
            static auto &shadowRender = *static_cast<ShadowMapRenderer *>
                                        (assets->renderers["Shadowmapping"].get());

            // set first directional light as shadowmapping
            for(auto &l : scene->lights)
            {
                if(l->Type() == Light::Directional)
                {
                    l->mode[0].set(0, 1);
                    shadowRender.Caster(l.get());
                }
            }
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