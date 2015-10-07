#include "stdafx.h"
#include "engine_assets.h"
#include "engine_base.h"

EngineAssets::EngineAssets() : demoScenesLoaded(false)
{
    // available scenes for execution
    //sceneFilepaths.push_back("resources\\models\\crytek-sponza\\sponza.obj");
    sceneFilepaths.push_back("resources\\models\\dabrovic-sponza\\sponza.obj");
    //sceneFilepaths.push_back("resources\\models\\sibenik\\sibenik.obj");
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
}

void EngineAssets::LoadDefaultScenes()
{
    if(demoScenesLoaded) return;

    // reserve space for available scenes for use
    demoScenes.resize(sceneFilepaths.size());
    // import scenes raw data, in parallel
    tbb::parallel_for(size_t(0), sceneFilepaths.size(), [ = ](size_t i)
    {
        std::string cutFilePath = std::string(sceneFilepaths[i]);

        if(cutFilePath.size() > 30)
        {
            cutFilePath = cutFilePath.substr(cutFilePath.size() - 30, 30);
        }

        std::cout << "(EngineAssets) Processing File: (.." + cutFilePath + ")\n";
        // create and import scene from file path
        demoScenes[i] = std::unique_ptr<Scene>(new Scene());
        sceneImporter.Import(sceneFilepaths[i], *demoScenes[i]);
        // scene finally loaded
        std::cout << "(EngineAssets) Loaded Scene: (.." + cutFilePath + ")\n";
    });

    for(auto it = demoScenes.begin(); it != demoScenes.end(); ++it)
    {
        // upload to GPU meshes buffers
        for(auto mesh = (*it)->meshes.begin(); mesh != (*it)->meshes.end(); ++mesh)
        {
            (*mesh)->UploadToGPU();
        }

        // upload to GPU textures 2d
        for(auto tex = (*it)->textures.begin(); tex != (*it)->textures.end(); ++tex)
        {
            (*tex)->UploadToGPU();
        }
    }

    // finally loaded
    demoScenesLoaded = true;
}

Scene &EngineAssets::GetScene(const unsigned int index)
{
    return *demoScenes[index];
}

void EngineAssets::LoadAssets()
{
    LoadDefaultScenes();
}
