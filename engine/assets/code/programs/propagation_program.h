#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class PropagationProgram : public ProgramShader
{
    public:
        oglplus::Uniform<float> maxTracingDistanceGlobal;
        oglplus::Uniform<int> volumeDimension;
        oglplus::Uniform<float> voxelSize;
        oglplus::Uniform<glm::vec3> worldMinPoint;
        oglplus::Uniform<float> voxelScale;
        void ExtractUniforms() override;
        PropagationProgram();
        ~PropagationProgram();
};

