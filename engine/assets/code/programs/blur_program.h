#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class BlurProgram : public ProgramShader
{
    public:
        oglplus::Uniform<glm::vec2> blurDirection;
        oglplus::Uniform<int> blurType;
        oglplus::UniformSampler source;
        void ExtractUniforms() override;
        BlurProgram();
        ~BlurProgram();
};

