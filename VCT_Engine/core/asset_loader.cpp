#include "stdafx.h"
#include "asset_loader.h"
using namespace VCT_ENGINE;

AssetLoader::AssetLoader() : demoScenesLoaded(false)
{
}


AssetLoader::~AssetLoader()
{
}

void VCT_ENGINE::AssetLoader::LoadDemoScenes()
{
    if(demoScenesLoaded) return;

    for(unsigned int i = 0; i < 3; i++)
    {
        this->demoScenes[i] = std::unique_ptr<Scene>(new Scene());
    }

    importerThread[0] = std::thread(&SceneImporter::Import, SceneImporter(),
                                    "resources\\models\\crytek-sponza\\sponza.obj", std::ref(*this->demoScenes[0]));
    importerThread[1] = std::thread(&SceneImporter::Import, SceneImporter(),
                                    "resources\\models\\dabrovic-sponza\\sponza.obj",
                                    std::ref(*this->demoScenes[1]));
    importerThread[2] = std::thread(&SceneImporter::Import, SceneImporter(),
                                    "resources\\models\\sibenik\\sibenik.obj", std::ref(*this->demoScenes[2]));
    // import scenes
    demoScenesLoaded = true;
}

void VCT_ENGINE::AssetLoader::LoadShaders()
{
}

Scene * VCT_ENGINE::AssetLoader::GetScene(
    const unsigned int index)
{
    return demoScenes[index].get();
}
