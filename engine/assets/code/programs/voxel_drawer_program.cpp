#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_drawer_program.h"

void VoxelDrawerProgram::ExtractUniforms()
{
    // program owner
    volumeDimension.Assign(program);
    matrices.modelViewProjection.Assign(program);
    // binding point
    volumeDimension.BindTo("volumeDimension");
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");

    for(int i = 0; i < frustumPlanes.size(); i++)
    {
        frustumPlanes[i].Assign(program);
        frustumPlanes[i].BindTo("frustumPlanes[" + std::to_string(i) + "]");
    }

    voxelSize.Assign(program);
    voxelSize.BindTo("voxelSize");
    worldMinPoint.Assign(program);
    worldMinPoint.BindTo("worldMinPoint");
}

VoxelDrawerProgram::~VoxelDrawerProgram()
{
}
