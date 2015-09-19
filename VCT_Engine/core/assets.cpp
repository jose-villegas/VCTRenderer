#include "stdafx.h"
#include "assets.h"
#include "base.h"
using namespace VCT_ENGINE;

Assets::Assets() : demoScenesLoaded(false)
{
    // available scenes for execution
    sceneFilepaths.push_back("resources\\models\\crytek-sponza\\sponza.obj");
    sceneFilepaths.push_back("resources\\models\\dabrovic-sponza\\sponza.obj");
    sceneFilepaths.push_back("resources\\models\\sibenik\\sibenik.obj");
}


Assets::~Assets()
{
}

void VCT_ENGINE::Assets::LoadDemoScenes()
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

void VCT_ENGINE::Assets::LoadDeferredShaders()
{
    using namespace oglplus;
    deferredShaders[0] = std::shared_ptr<Program>(new Program());
    // load shaders source code and compile
    VertexShader geomVert(
        GLSLSource::FromFile("resources\\shaders\\geometry_pass.vert"));
    geomVert.Compile();
    FragmentShader geomFrag(
        GLSLSource::FromFile("resources\\shaders\\geometry_pass.frag"));
    geomFrag.Compile();
    // create a new shader program and attach the shaders
    deferredShaders[0]->AttachShader(geomVert);
    deferredShaders[0]->AttachShader(geomFrag);
    // link attached shaders
    deferredShaders[0]->Link().Use();
}

Scene &VCT_ENGINE::Assets::GetScene(const unsigned int index)
{
    return *demoScenes[index];
}
