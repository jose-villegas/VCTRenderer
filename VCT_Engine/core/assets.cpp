#include "stdafx.h"
#include "assets.h"
#include "base.h"
using namespace VCT_ENGINE;

Assets::Assets() : demoScenesLoaded(false)
{
    // available scenes for execution
    availableScenes.push_back("resources\\models\\crytek-sponza\\sponza.obj");
    availableScenes.push_back("resources\\models\\dabrovic-sponza\\sponza.obj");
    availableScenes.push_back("resources\\models\\sibenik\\sibenik.obj");
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

    // import scenes, in parallel
    tbb::parallel_for(size_t(0), availableScenes.size(),
                      [ = ](size_t i)
    {
        sceneImporter.Import(availableScenes[i], *demoScenes[i]);
    });
    // finally loaded
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
    // link attached shaders
    phongShader->Link();
    // store reference in asset load class
    this->engineShaders["Phong"] = phongShader;
}

Scene * VCT_ENGINE::Assets::GetScene(
    const unsigned int index)
{
    return demoScenes[index].get();
}
