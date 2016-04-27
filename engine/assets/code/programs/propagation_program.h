#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class PropagationProgram : public ProgramShader
{
    public:
        oglplus::Uniform<float> maxTracingDistanceGlobal;
        oglplus::Uniform<int> volumeDimension;
        oglplus::Uniform<unsigned int>  checkBoundaries;
        void ExtractUniforms() override;
        PropagationProgram();
        ~PropagationProgram();
};

