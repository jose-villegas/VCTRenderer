#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class ClearDynamicProgram : public ProgramShader
{
    public:
        void ExtractUniforms() override;
        ClearDynamicProgram();
        ~ClearDynamicProgram();
};

