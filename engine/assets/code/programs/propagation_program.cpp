#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "propagation_program.h"


void PropagationProgram::ExtractUniforms()
{
    maxTracingDistanceGlobal.Assign(program);
    maxTracingDistanceGlobal.BindTo("maxTracingDistanceGlobal");
}

PropagationProgram::PropagationProgram()
{
}


PropagationProgram::~PropagationProgram()
{
}
