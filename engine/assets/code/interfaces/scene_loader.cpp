#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene_loader.h"

#include "main_menu.h"
#include "../../../scene/camera.h"
#include "../../../scene/scene.h"
#include "../../../core/assets_manager.h"
#include "../../../scene/light.h"
#include "../renderers/shadow_map_renderer.h"
#include "../../../util/scene_importer.h"

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
    static Scene * reloaded = nullptr;
    SetNextWindowPosCenter();

    if (Begin("Load Scene", &UIMainMenu::drawSceneLoader))
    {
        static auto advancedImport = false;
        static unsigned int flag = aiProcessPreset_TargetRealtime_Fast;
        static unsigned int flags[26] =
        {
            aiProcess_CalcTangentSpace,
            aiProcess_JoinIdenticalVertices,
            aiProcess_MakeLeftHanded,
            aiProcess_Triangulate,
            aiProcess_RemoveComponent,
            aiProcess_GenNormals,
            aiProcess_GenSmoothNormals,
            aiProcess_SplitLargeMeshes,
            aiProcess_PreTransformVertices,
            aiProcess_LimitBoneWeights,
            aiProcess_ValidateDataStructure,
            aiProcess_ImproveCacheLocality,
            aiProcess_RemoveRedundantMaterials,
            aiProcess_FixInfacingNormals,
            aiProcess_SortByPType,
            aiProcess_FindDegenerates,
            aiProcess_FindInvalidData,
            aiProcess_GenUVCoords,
            aiProcess_TransformUVCoords,
            aiProcess_FindInstances,
            aiProcess_OptimizeMeshes,
            aiProcess_OptimizeGraph,
            aiProcess_FlipUVs ,
            aiProcess_FlipWindingOrder,
            aiProcess_SplitByBoneCount,
            aiProcess_Debone,
        };

        // active scene selector
        if (Combo("Path", &activeScene, SceneName, &assets->scenes,
                  static_cast<int>(assets->scenes.size())))
        {
            auto begin = assets->scenes.begin();
            advance(begin, activeScene);
            begin->second->SetAsActive();
        }

        if(scene) SameLine();

        if (reloaded) { reloaded->SetAsActive(); }

        if (scene && Button(scene->IsLoaded() ? "Reload" : "Load"))
        {
            auto scenePtr = scene.get();
            scenePtr->IsLoaded() ? scenePtr->CleanImport(flag) : scenePtr->Import(flag);
            scenePtr->Load();
            static auto &shadowRender = *static_cast<ShadowMapRenderer *>
                                        (assets->renderers["Shadowmapping"].get());

            // set first directional light as shadowmapping
            for(auto &l : scenePtr->lights)
            {
                if(l->Type() == Light::Directional)
                {
                    l->mode[0].set(0, 1);
                    shadowRender.Caster(l.get());
                }
            }

            if(shadowRender.Caster()) shadowRender.Caster()->RegisterChange();

            if (scenePtr->cameras.size() > 0) scenePtr->cameras.front()->SetAsActive();

            // to reset as active next frame
            reloaded = scenePtr;
        }

        static int choosen = 1;

        if (Combo("Importing Preset", &choosen,
                  "ToLeftHanded\0Fast\0Quality\0Max Quality", 4))
        {
            switch (choosen)
            {
                case 0: flag = aiProcess_ConvertToLeftHanded; break;

                case 1: flag = aiProcessPreset_TargetRealtime_Fast; break;

                case 2: flag = aiProcessPreset_TargetRealtime_Quality; break;

                case 3: flag = aiProcessPreset_TargetRealtime_MaxQuality; break;
            }
        }

        Separator();
        Checkbox("Show Advanced Import Settings", &advancedImport);

        if(advancedImport)
        {
            Separator();
            PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            Columns(2);

            for (auto i = 0; i < SceneImporter::FlagNames.size(); i++)
            {
                if (i > SceneImporter::FlagNames.size() / 2 - 1 &&
                        i <= SceneImporter::FlagNames.size() / 2)
                {
                    NextColumn();
                }

                if (CheckboxFlags(SceneImporter::FlagNames[i].c_str(),
                                  &flag, flags[i]))
                {
                }
            }

            PopStyleVar();
        }
    }

    End();
}

UISceneLoader::UISceneLoader()
{
}

UISceneLoader::~UISceneLoader()
{
}