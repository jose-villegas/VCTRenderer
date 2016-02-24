#pragma once

#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class LightingProgram : public ProgramShader
{
    protected:
        // light pass uniforms
        struct UniformLight
        {
            oglplus::Uniform<float> angleInnerCone;
            oglplus::Uniform<float> angleOuterCone;

            oglplus::Uniform<glm::vec3> ambient;
            oglplus::Uniform<glm::vec3> diffuse;
            oglplus::Uniform<glm::vec3> specular;

            oglplus::Uniform<glm::vec3> position;
            oglplus::Uniform<glm::vec3> direction;

            struct UniformAttenuation
            {
                oglplus::Uniform<float> constant;
                oglplus::Uniform<float> linear;
                oglplus::Uniform<float> quadratic;
            };

            UniformAttenuation attenuation;
        };
    public:
        // fragment shader uniforms
        oglplus::UniformSampler gPosition;
        oglplus::UniformSampler gNormal;
        oglplus::UniformSampler gAlbedo;
        oglplus::UniformSampler gSpecular;

        std::vector<UniformLight> directionalLight;

        void ExtractUniforms() override;

        using ProgramShader::ProgramShader;
        ~LightingProgram() override;
        LightingProgram(LightingProgram const &r) = delete;
        LightingProgram &operator=(LightingProgram const &r) = delete;

};

