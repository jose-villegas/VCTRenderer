#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class InjectRadianceProgram : public ProgramShader
{
    protected:
    public:
        void ExtractUniforms() override;
        InjectRadianceProgram();
        ~InjectRadianceProgram();
};

