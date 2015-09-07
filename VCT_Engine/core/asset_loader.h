#pragma once
#include "util\scene_importer.h"

namespace VCT_ENGINE
{
    class Assets
    {
        private:
            SceneImporter sceneImporter;
            std::unordered_map<std::string, oglplus::Program *>engineShaders;
            std::array<std::unique_ptr<Scene>, 3> demoScenes;
            std::thread importerThread[3];

            bool demoScenesLoaded;
        public:
            Assets();
            virtual ~Assets();
            void LoadDemoScenes();
            void LoadShaders();
            Scene * GetScene(const unsigned int index);
    };
}

