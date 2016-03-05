#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxelization_program.h"

void VoxelizationProgram::ExtractUniforms()
{
    // program owner
    storeMode.Assign(program);
    diffuseMap.Assign(program);
    cellSize[0].Assign(program);
    cellSize[1].Assign(program);
    viewProjections[0].Assign(program);
    viewProjections[1].Assign(program);
    viewProjections[2].Assign(program);
    // binding point
    storeMode.BindTo("storeMode");
    diffuseMap.BindTo("diffuseMap");
    cellSize[0].BindTo("cellSize[0]");
    cellSize[1].BindTo("cellSize[1]");
    viewProjections[0].BindTo("viewProjections[0]");
    viewProjections[1].BindTo("viewProjections[1]");
    viewProjections[2].BindTo("viewProjections[2]");
}

VoxelizationProgram::~VoxelizationProgram()
{
}
