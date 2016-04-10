#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "voxel_drawer_program.h"

void VoxelDrawerProgram::ExtractUniforms()
{
    // program owner
    direction.Assign(program);
    volumeDimension.Assign(program);
    matrices.modelViewProjection.Assign(program);
    // binding point
    direction.BindTo("direction");
    volumeDimension.BindTo("volumeDimension");
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
}

VoxelDrawerProgram::~VoxelDrawerProgram()
{
}
