#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>

class MipmappingBaseProgram : public ProgramShader
{
    public:
        void ExtractUniforms() override;
        MipmappingBaseProgram();
        ~MipmappingBaseProgram();
};

class MipmappingVolumeProgram : public ProgramShader
{
    public:
        oglplus::Uniform<int> mipLevel;
        oglplus::Uniform<int> mipDimension;
        void ExtractUniforms() override;
        MipmappingVolumeProgram();
        ~MipmappingVolumeProgram();
};