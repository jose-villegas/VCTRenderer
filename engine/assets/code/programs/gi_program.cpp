#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gi_program.h"

void GIProgram::ExtractUniforms()

{
    using namespace oglplus;
    // assign program
    inverseProjectionView.Assign(program);
    gDepth.Assign(program);
    gNormal.Assign(program);
    gAlbedo.Assign(program);
    gSpecular.Assign(program);
    // bind to uniform name
    inverseProjectionView.BindTo("inverseProjectionView");
    gDepth.BindTo("gDepth");
    gNormal.BindTo("gNormal");
    gAlbedo.BindTo("gAlbedo");
    gSpecular.BindTo("gSpecular");
    // voxel volume setup
    volumeDimension.Assign(program);
    volumeDimension.BindTo("volumeDimension");
    voxelTex.Assign(program);
    voxelTex.BindTo("voxelTex");
    voxelTexMipmap.Assign(program);
    voxelTexMipmap.BindTo("voxelTexMipmap");
    // from world to voxel space
    worldMinPoint.Assign(program);
    worldMinPoint.BindTo("worldMinPoint");
    voxelScale.Assign(program);
    voxelScale.BindTo("voxelScale");
    // gi options
    maxTracingDistanceGlobal.Assign(program);
    maxTracingDistanceGlobal.BindTo("maxTracingDistanceGlobal");
    bounceStrength.Assign(program);
    bounceStrength.BindTo("bounceStrength");
    aoFalloff.Assign(program);
    aoFalloff.BindTo("aoFalloff");
    aoAlpha.Assign(program);
    aoAlpha.BindTo("aoAlpha");
    enableAo.Assign(program);
    enableAo.BindTo("enableAo");
    cameraPosition.Assign(program);
    cameraPosition.BindTo("cameraPosition");
}

void FinalCompositionProgram::ExtractUniforms()
{
    directLight.Assign(program);
    indirectLight.Assign(program);
    directLight.BindTo("directLight");
    indirectLight.BindTo("indirectLight");
}
