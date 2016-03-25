#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxelization_program.h"

void VoxelizationProgram::ExtractUniforms()
{
    // program owner
    matrices.model.Assign(program);
    material.diffuse.Assign(program);
    diffuseMap.Assign(program);
    volumeDimension.Assign(program);
    viewProjections[0].Assign(program);
    viewProjections[1].Assign(program);
    viewProjections[2].Assign(program);
    // binding point
    matrices.model.BindTo("matrices.model");
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
