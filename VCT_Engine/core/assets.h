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
            std::array<std::unique_ptr<Scene>, 3> demoScenes;
            // raw data importing is made using multiple threads
            std::thread importerThread[3];
            // all program shaders used by the base engine stored here
            std::unordered_map<std::string, oglplus::Program *>engineShaders;
            // stores names of available scenes
            std::vector<char *> availableScenes;
        public:
            Assets();
            virtual ~Assets();
            void LoadDemoScenes();
            void LoadShaders();
            Scene * GetScene(const unsigned int index);

            const std::vector<char *> &GetAvailableScenes() { return availableScenes; }
    };
}

