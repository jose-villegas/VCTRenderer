#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxelization_program.h"

#include "../../../scene/light.h"

void VoxelizationProgram::ExtractUniforms()
{
    // program owner
    matrices.model.Assign(program);
    matrices.normal.Assign(program);
    material.diffuse.Assign(program);
    material.emissive.Assign(program);
    diffuseMap.Assign(program);
    volumeDimension.Assign(program);
    viewProjections[0].Assign(program);
    viewProjections[1].Assign(program);
    viewProjections[2].Assign(program);
    viewProjectionsI[0].Assign(program);
    viewProjectionsI[1].Assign(program);
    viewProjectionsI[2].Assign(program);
    // binding point
    matrices.model.BindTo("matrices.model");
    matrices.normal.BindTo("matrices.normal");
    material.diffuse.BindTo("material.diffuse");
    material.emissive.BindTo("material.emissive");
    diffuseMap.BindTo("diffuseMap");
    volumeDimension.BindTo("volumeDimension");
    viewProjections[0].BindTo("viewProjections[0]");
    viewProjections[1].BindTo("viewProjections[1]");
    viewProjections[2].BindTo("viewProjections[2]");
    viewProjectionsI[0].BindTo("viewProjectionsI[0]");
    viewProjectionsI[1].BindTo("viewProjectionsI[1]");
    viewProjectionsI[2].BindTo("viewProjectionsI[2]");
    // from world to voxel space
    worldMinPoint.Assign(program);
    worldMinPoint.BindTo("worldMinPoint");
    voxelScale.Assign(program);
    voxelScale.BindTo("voxelScale");
};

VoxelizationProgram::~VoxelizationProgram()
{
}
