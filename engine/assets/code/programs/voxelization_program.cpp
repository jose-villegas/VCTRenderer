#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxelization_program.h"

void VoxelizationProgram::ExtractUniforms()
{
    // program owner
    shadowMap.Assign(program);
    lightViewProjection.Assign(program);
    matrices.model.Assign(program);
    matrices.normal.Assign(program);
    material.diffuse.Assign(program);
    diffuseMap.Assign(program);
    volumeDimension.Assign(program);
    viewProjections[0].Assign(program);
    viewProjections[1].Assign(program);
    viewProjections[2].Assign(program);
    // binding point
    shadowMap.BindTo("shadowMap");
    lightViewProjection.BindTo("lightViewProjection");
    matrices.model.BindTo("matrices.model");
    matrices.normal.BindTo("matrices.normal");
    material.diffuse.BindTo("material.diffuse");
    diffuseMap.BindTo("diffuseMap");
    volumeDimension.BindTo("volumeDimension");
    viewProjections[0].BindTo("viewProjections[0]");
    viewProjections[1].BindTo("viewProjections[1]");
    viewProjections[2].BindTo("viewProjections[2]");
}

VoxelizationProgram::~VoxelizationProgram()
{
}
