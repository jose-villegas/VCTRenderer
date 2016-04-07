#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class PropagationProgram : public ProgramShader
{
    public:
        void ExtractUniforms() override;
        PropagationProgram();
        ~PropagationProgram();
};

