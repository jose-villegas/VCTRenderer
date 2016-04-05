#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class InjectRadianceProgram : public ProgramShader
{
    protected:
        struct UniformLight
        {
            oglplus::Uniform<glm::vec3> diffuse;
            oglplus::Uniform<glm::vec3> direction;
        };
    public:
        oglplus::Uniform<glm::mat4x4> lightViewProjection;
        oglplus::UniformSampler shadowMap;
        UniformLight directionalLight;
        oglplus::Uniform<glm::vec2> exponents;
        oglplus::Uniform<float> lightBleedingReduction;
        oglplus::Uniform<float> voxelSize;
        oglplus::Uniform<glm::vec3> worldMinPoint;
        oglplus::Uniform<unsigned int> shadowMapping;
        void ExtractUniforms() override;
        InjectRadianceProgram();
        ~InjectRadianceProgram();
};

