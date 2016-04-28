#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "propagation_program.h"


void PropagationProgram::ExtractUniforms()
{
    maxTracingDistanceGlobal.Assign(program);
    maxTracingDistanceGlobal.BindTo("maxTracingDistanceGlobal");
    volumeDimension.Assign(program);
    volumeDimension.BindTo("volumeDimension");
    checkBoundaries.Assign(program);
    checkBoundaries.BindTo("checkBoundaries");
}

PropagationProgram::PropagationProgram()
{
}


PropagationProgram::~PropagationProgram()
{
}
