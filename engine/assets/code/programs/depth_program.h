#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class DepthProgram : public ProgramShader
{
    protected:
        struct UniformMatrices
        {
            oglplus::Uniform<glm::mat4> modelViewProjection;
        };
    public:
        UniformMatrices matrices;
        oglplus::Uniform<glm::vec2> exponents;
        oglplus::UniformSampler diffuseMap;
        oglplus::Uniform<float> alphaCutoff;

        void ExtractUniforms() override;
        DepthProgram();
        ~DepthProgram();
};

