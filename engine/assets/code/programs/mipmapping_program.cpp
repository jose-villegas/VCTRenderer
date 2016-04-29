#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mipmapping_program.h"


void MipmappingBaseProgram::ExtractUniforms()
{
    mipDimension.Assign(program);
    mipDimension.BindTo("mipDimension");
}

MipmappingBaseProgram::MipmappingBaseProgram()
{
}

MipmappingBaseProgram::~MipmappingBaseProgram()
{
}

void MipmappingVolumeProgram::ExtractUniforms()
{
    mipDimension.Assign(program);
    mipDimension.BindTo("mipDimension");
    mipLevel.Assign(program);
    mipLevel.BindTo("mipLevel");
}

MipmappingVolumeProgram::MipmappingVolumeProgram()
{
}

MipmappingVolumeProgram::~MipmappingVolumeProgram()
{
}
