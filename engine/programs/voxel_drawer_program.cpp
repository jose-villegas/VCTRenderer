#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_drawer_program.h"

void VoxelDrawerProgram::ExtractUniforms()
{
    // program owner
    matrices.viewProjection.Assign(program);
    halfVoxelSize.Assign(program);
    voxelAlbedo.Assign(program);
    // binding point
    matrices.viewProjection.BindTo("matrices.viewProjection");
    halfVoxelSize.BindTo("halfVoxelSize");
    voxelAlbedo.BindTo("voxelAlbedo");
}

VoxelDrawerProgram::~VoxelDrawerProgram()
{
}
