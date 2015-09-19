#pragma once
#include "util\scene_importer.h"

class EngineAssets
{
    private:
        // utility class to import scenes using assimp
        SceneImporter sceneImporter;
        // indicates when all threads have ended loading scenes
        bool demoScenesLoaded;
        // all scenes data used by the base engine are stored here
        std::vector<std::shared_ptr<Scene>> demoScenes;
        // stores locations of available scenes files
        std::vector<const char *> sceneFilepaths;
        // loaders
        void LoadDefaultScenes();
    public:
        EngineAssets();
        virtual ~EngineAssets();

        Scene &GetScene(const unsigned int index);
        const std::vector<const char *> &GetAvailableScenes() { return sceneFilepaths; }
        unsigned int GetNumberOfAvailableScenes() { return (unsigned int)sceneFilepaths.size(); }
        void LoadAssets();
};


