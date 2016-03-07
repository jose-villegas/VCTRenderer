#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_drawer_program.h"

void VoxelDrawerProgram::ExtractUniforms()
{
    // program owner
    volumeDimension.Assign(program);
    matrices.viewProjection.Assign(program);
    voxelSize.Assign(program);
    voxelAlbedo.Assign(program);
    voxelGridMove.Assign(program);
    // binding point
    volumeDimension.BindTo("volumeDimension");
    matrices.viewProjection.BindTo("matrices.viewProjection");
    voxelSize.BindTo("voxelSize");
    voxelAlbedo.BindTo("voxelAlbedo");
    voxelGridMove.BindTo("voxelGridMove");
}

VoxelDrawerProgram::~VoxelDrawerProgram()
{
}
