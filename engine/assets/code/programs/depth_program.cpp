#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "depth_program.h"


void DepthProgram::ExtractUniforms()
{
    matrices.modelViewProjection.Assign(program);
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
}

DepthProgram::DepthProgram()
{
}


DepthProgram::~DepthProgram()
{
}
