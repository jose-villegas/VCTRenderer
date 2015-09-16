#pragma once
#include "util\scene_importer.h"

namespace VCT_ENGINE
{
    class Assets
    {
        private:
            // utility class to import scenes using assimp
            SceneImporter sceneImporter;
            // indicates when all threads have ended loading scenes
            bool demoScenesLoaded;
            // all scenes data used by the base engine are stored here
            std::vector<std::unique_ptr<Scene>> demoScenes;
            // all program shaders used by the base engine stored here
            std::unordered_map<std::string, oglplus::Program *>engineShaders;
            // stores locations of available scenes files
            std::vector<const char *> availableScenes;
        public:
            Assets();
            virtual ~Assets();
            void LoadDemoScenes();
            void LoadShaders();
            Scene * GetScene(const unsigned int index);

            const std::vector<const char *> &GetAvailableScenes() { return availableScenes; }
    };
}

