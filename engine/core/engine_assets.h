#pragma once
#include "util\scene_importer.h"

class EngineAssets
{
    public:
        EngineAssets();
        virtual ~EngineAssets();

        std::vector<const char *> &SceneNames();
        std::vector<std::shared_ptr<Scene>> &Scenes();
        void LoadScenes();
    private:
        // prevents loading scenes twice
        bool scenesAlreadyLoaded;
        // utility class to import scenes using assimp
        SceneImporter sceneImporter;
        // all scenes data used by the base engine are stored here
        std::vector<std::shared_ptr<Scene>> scenes;
        // stores locations of available scenes files
        std::vector<const char *> sceneFilepaths;
        std::vector<const char *> sceneNames;
};


