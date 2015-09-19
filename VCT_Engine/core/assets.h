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
            std::vector<std::shared_ptr<Scene>> demoScenes;
            // all program shaders used by the base engine stored here
            std::array<std::shared_ptr<oglplus::Program>, 2> deferredShaders;
            // stores locations of available scenes files
            std::vector<const char *> sceneFilepaths;
        public:
            Assets();
            virtual ~Assets();
            void LoadDemoScenes();
            void LoadDeferredShaders();
            Scene &GetScene(const unsigned int index);

            const std::vector<const char *> &GetAvailableScenes() { return sceneFilepaths; }
            unsigned int GetNumberOfAvailableScenes() { return (unsigned int)sceneFilepaths.size(); }

            oglplus::Program &GetGeometryPassShader() { return *deferredShaders[0]; }
            oglplus::Program &GetLightPassShader() { return *deferredShaders[1]; }
    };
}

