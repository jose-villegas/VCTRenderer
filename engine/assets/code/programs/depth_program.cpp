#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "depth_program.h"


void DepthProgram::ExtractUniforms()
{
    matrices.modelViewProjection.Assign(program);
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
    exponents.Assign(program);
    exponents.BindTo("exponents");
    diffuseMap.Assign(program);
    alphaCutoff.Assign(program);
    diffuseMap.BindTo("diffuseMap");
    alphaCutoff.BindTo("alphaCutoff");
}

DepthProgram::DepthProgram()
{
}


DepthProgram::~DepthProgram()
{
}
