#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_drawer_program.h"

void VoxelDrawerProgram::ExtractUniforms()
{
    // program owner
    sceneMinPoint.Assign(program);
    volumeDimension.Assign(program);
    matrices.modelViewProjection.Assign(program);
    voxelSize.Assign(program);
    // binding point
    sceneMinPoint.BindTo("sceneMinPoint");
    volumeDimension.BindTo("volumeDimension");
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
    voxelSize.BindTo("voxelSize");
}

VoxelDrawerProgram::~VoxelDrawerProgram()
{
}
