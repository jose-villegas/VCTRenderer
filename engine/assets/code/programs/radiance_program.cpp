#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "radiance_program.h"


void InjectRadianceProgram::ExtractUniforms()
{
    shadowMap.Assign(program);
    exponents.Assign(program);
    lightBleedingReduction.Assign(program);
    matrices.model.Assign(program);
    lightViewProjection.Assign(program);
    shadowMap.BindTo("shadowMap");
    exponents.BindTo("exponents");
    lightBleedingReduction.BindTo("lightBleedingReduction");
    matrices.model.BindTo("matrices.model");
    lightViewProjection.BindTo("lightViewProjection");
}

InjectRadianceProgram::InjectRadianceProgram()
{
}


InjectRadianceProgram::~InjectRadianceProgram()
{
}
