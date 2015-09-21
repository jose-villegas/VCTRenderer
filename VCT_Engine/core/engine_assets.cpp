#include "stdafx.h"
#include "engine_assets.h"
#include "engine_base.h"

EngineAssets::EngineAssets() : demoScenesLoaded(false)
{
    // available scenes for execution
    //sceneFilepaths.push_back("resources\\models\\crytek-sponza\\sponza.obj");
    //sceneFilepaths.push_back("resources\\models\\dabrovic-sponza\\sponza.obj");
    //sceneFilepaths.push_back("resources\\models\\sibenik\\sibenik.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Empty-CO.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Empty-RG.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Empty-Squashed.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Empty-White.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Glossy.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Mirror.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Original.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Sphere.obj");
    sceneFilepaths.push_back("resources\\models\\cornell-box\\CornellBox-Water.obj");
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
        demoScenes[i] = std::unique_ptr<Scene>(new Scene());
        sceneImporter.Import(sceneFilepaths[i], *demoScenes[i]);
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
