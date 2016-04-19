#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "propagation_program.h"


void PropagationProgram::ExtractUniforms()
{
    maxTracingDistanceGlobal.Assign(program);
    maxTracingDistanceGlobal.BindTo("maxTracingDistanceGlobal");
    volumeDimension.Assign(program);
    volumeDimension.BindTo("volumeDimension");
    voxelScale.Assign(program);
    voxelScale.BindTo("voxelScale");
    voxelSize.Assign(program);
    voxelSize.BindTo("voxelSize");
    worldMinPoint.Assign(program);
    worldMinPoint.BindTo("worldMinPoint");
}

PropagationProgram::PropagationProgram()
{
}


PropagationProgram::~PropagationProgram()
{
}
