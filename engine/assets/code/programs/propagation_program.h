#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class PropagationProgram : public ProgramShader
{
    public:
        oglplus::Uniform<float> maxTracingDistanceGlobal;
        oglplus::Uniform<int> volumeDimension;

        void ExtractUniforms() override;
        PropagationProgram();
        ~PropagationProgram();
};

