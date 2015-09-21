#include "stdafx.h"
#include "deferred_handler.h"


DeferredHandler::DeferredHandler()
{
    using namespace oglplus;
    // load shaders source code and compile
    std::ifstream vsGeomPassFile("resources\\shaders\\geometry_pass.vert");
    std::string vsGeomPassSource(
        (std::istreambuf_iterator<char>(vsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    vsGeometryPass.Source(vsGeomPassSource.c_str());
    vsGeometryPass.Compile();
    // fragment shader
    std::ifstream fsGeomPassFile("resources\\shaders\\geometry_pass.frag");
    std::string fsGeomPassSource(
        (std::istreambuf_iterator<char>(fsGeomPassFile)),
        std::istreambuf_iterator<char>()
    );
    fsGeometryPass.Source(fsGeomPassSource.c_str());
    fsGeometryPass.Compile();
    // create a new shader program and attach the shaders
    geometryPass.AttachShader(vsGeometryPass);
    geometryPass.AttachShader(fsGeometryPass);
    // link attached shaders
    geometryPass.Link().Use();
}


DeferredHandler::~DeferredHandler()
{
}