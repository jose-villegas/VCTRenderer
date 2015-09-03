#pragma once
#include "util\scene_importer.h"
#include "util\shader_program.h"

namespace VCT_ENGINE
{
    class AssetLoader
    {
        private:
            SceneImporter sceneImporter;
            std::vector<OGLShaderProgram *> engineShaders;
            std::array<std::unique_ptr<Scene>, 3> demoScenes;
            std::thread importerThread[3];

            bool demoScenesLoaded;
        public:
            AssetLoader();
            virtual ~AssetLoader();

            void LoadDemoScenes();
            void LoadShaders();

            Scene * GetScene(const unsigned int index);
    };
}

