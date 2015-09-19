#include "stdafx.h"
#include "deferred_handler.h"


DeferredHandler::DeferredHandler()
{
}


DeferredHandler::~DeferredHandler()
{
}

void DeferredHandler::LoadDeferredShaders()
{
    using namespace oglplus;
    // load shaders source code and compile
    VertexShader geomVert(
        GLSLSource::FromFile("resources\\shaders\\geometry_pass.vert"));
    geomVert.Compile();
    FragmentShader geomFrag(
        GLSLSource::FromFile("resources\\shaders\\geometry_pass.frag"));
    geomFrag.Compile();
    // create a new shader program and attach the shaders
    geometryPass.AttachShader(geomVert);
    geometryPass.AttachShader(geomFrag);
    // link attached shaders
    geometryPass.Link().Use();
}
