#include "engine_assets.h"
#include "../scene/texture.h"
#include <iostream>
#include <tbb/tbb.h>
#include "../scene/camera.h"
#include "../scene/mesh.h"
#include "../scene/scene.h"

EngineAssets::EngineAssets() : scenesAlreadyLoaded(false)
{
    // available scenes for execution
    //sceneFilepaths.push_back("resources\\models\\crytek-sponza\\sponza.obj");
    //sceneFilepaths.push_back("resources\\models\\dabrovic-sponza\\sponza.obj");
    sceneFilepaths.push_back("resources\\models\\sibenik\\sibenik.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-empty-co.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-empty-rg.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-empty-squashed.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-empty-white.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-glossy.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-mirror.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-original.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-sphere.obj");
    //sceneFilepaths.push_back("resources\\models\\cornell-box\\cornellbox-water.obj");
}


EngineAssets::~EngineAssets()
{
    // early owner-ship release for static scope (no gl context)
    delete OGLTexture2D::GetDefaultTexture().release();
    delete OGLTexture2D::GetDefaultNormalTexture().release();
    delete OGLTexture2D::GetErrorTexture().release();
}

std::vector<std::shared_ptr<Scene>> &EngineAssets::Scenes()
{
    return scenes;
}

std::vector<const char *> &EngineAssets::SceneNames()
{
    return sceneNames;
}

void EngineAssets::LoadScenes()
{
    if (scenesAlreadyLoaded) { return; }

    // reserve space for available scenes for use
    scenes.clear();
    scenes.resize(sceneFilepaths.size());
    // import scenes raw data, in parallel
    tbb::parallel_for(size_t(0), sceneFilepaths.size(), [ = ](size_t i)
    {
        std::string cutFilePath = std::string(sceneFilepaths[i]);

        if (cutFilePath.size() > 30)
        {
            cutFilePath = cutFilePath.substr(cutFilePath.size() - 30, 30);
        }

        std::cout << "(EngineAssets) Processing File: (.." + cutFilePath + ")\n";
        // create and import scene from file path
        scenes[i] = std::make_unique<Scene>();
        sceneImporter.Import(sceneFilepaths[i], *scenes[i]);
        // scene finally loaded
        std::cout << "(EngineAssets) Loaded Scene: (.." + cutFilePath + ")\n";
    });

    for (auto it = scenes.begin(); it != scenes.end(); ++it)
    {
        // upload to GPU meshes buffers
        for (auto mesh = (*it)->meshes.begin(); mesh != (*it)->meshes.end(); ++mesh)
        {
            (*mesh)->UploadToGPU();
        }

        // upload to GPU 2d textures
        for (auto tex = (*it)->textures.begin(); tex != (*it)->textures.end(); ++tex)
        {
            (*tex)->UploadToGPU();
        }

        // store root node name as scene name
        sceneNames.push_back((*it)->rootNode.name.c_str());
    }

    // by default set first scene as active scene
    scenes.front()->SetAsActive();
    scenes.front()->cameras.front()->SetAsActive();
    // finally loaded
    scenesAlreadyLoaded = true;
}
