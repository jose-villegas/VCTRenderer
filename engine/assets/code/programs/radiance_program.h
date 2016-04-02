#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class InjectRadianceProgram : public ProgramShader
{
    protected:
        struct UniformMatrices
        {
            oglplus::Uniform<glm::mat4> model;
        };

        struct UniformLight
        {
            oglplus::Uniform<glm::vec3> diffuse;
            oglplus::Uniform<glm::vec3> direction;
        };
    public:
        oglplus::Uniform<glm::mat4x4> lightViewProjection;
        oglplus::UniformSampler shadowMap;
        UniformMatrices matrices;
        UniformLight directionalLight;
        oglplus::Uniform<glm::vec2> exponents;
        oglplus::Uniform<float> lightBleedingReduction;
        void ExtractUniforms() override;
        InjectRadianceProgram();
        ~InjectRadianceProgram();
};

