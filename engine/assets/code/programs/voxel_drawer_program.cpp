#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_drawer_program.h"

void VoxelDrawerProgram::ExtractUniforms()
{
    // program owner
    volumeDimension.Assign(program);
    matrices.modelViewProjection.Assign(program);
    voxelSize.Assign(program);
    // binding point
    volumeDimension.BindTo("volumeDimension");
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
    voxelSize.BindTo("voxelSize");
}

VoxelDrawerProgram::~VoxelDrawerProgram()
{
}
