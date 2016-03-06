#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class VoxelDrawerProgram : public ProgramShader
{
    public:
        void ExtractUniforms() override;

        VoxelDrawerProgram() = default;
        virtual ~VoxelDrawerProgram();
};

