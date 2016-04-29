#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <glm/vec3.hpp>

class MipmappingBaseProgram : public ProgramShader
{
    public:
        oglplus::Uniform<int> mipDimension;
        void ExtractUniforms() override;
        MipmappingBaseProgram();
        ~MipmappingBaseProgram();
};

class MipmappingVolumeProgram : public ProgramShader
{
    public:
        oglplus::Uniform<glm::vec3> mipDimension;
        oglplus::Uniform<int> mipLevel;
        void ExtractUniforms() override;
        MipmappingVolumeProgram();
        ~MipmappingVolumeProgram();
};