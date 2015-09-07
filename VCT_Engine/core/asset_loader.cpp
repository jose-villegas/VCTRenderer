#include "stdafx.h"
#include "asset_loader.h"
using namespace VCT_ENGINE;

Assets::Assets() : demoScenesLoaded(false)
{
}


Assets::~Assets()
{
}

void VCT_ENGINE::Assets::LoadDemoScenes()
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

void VCT_ENGINE::Assets::LoadShaders()
{
    using namespace oglplus;
    // load shaders source code
    Shader phongVert(ShaderType::Vertex,
                     GLSLSource::FromFile("resources\\shaders\\phong.vert"));
    Shader phongFrag(ShaderType::Fragment,
                     GLSLSource::FromFile("resources\\shaders\\phong.frag"));
    // compile vertex and fragment s code
    phongFrag.Compile(); phongVert.Compile();
    // create a new shader program and attach the shaders
    Program * phongShader = new Program();
    phongShader->AttachShader(phongVert);
    phongShader->AttachShader(phongFrag);
    // store reference in asset load class
    this->engineShaders["Phong"] = phongShader;
}

Scene * VCT_ENGINE::Assets::GetScene(
    const unsigned int index)
{
    return demoScenes[index].get();
}
