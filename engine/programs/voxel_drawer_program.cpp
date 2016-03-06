#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_drawer_program.h"

void VoxelDrawerProgram::ExtractUniforms()
{
    // program owner
    matrices.modelViewProjection.Assign(program);
    volumeDimension.Assign(program);
    voxelAlbedo.Assign(program);
    // binding point
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
    volumeDimension.BindTo("volumeDimension");
    voxelAlbedo.BindTo("voxelAlbedo");
}

VoxelDrawerProgram::~VoxelDrawerProgram()
{
}
