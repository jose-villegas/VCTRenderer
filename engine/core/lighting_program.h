#pragma once

#include "../scene/texture.h"
#include "../types/program_shader.h"
#include "../types/uniform_collection.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>
#include <oglplus/uniform_block.hpp>
#include <oglplus/buffer.hpp>

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
        void ExtractUniform(const oglplus::aux::ActiveUniformInfo &info) override;
    public:
        // fragment shader uniforms
        oglplus::UniformSampler gPosition;
        oglplus::UniformSampler gNormal;
        oglplus::UniformSampler gAlbedo;
        oglplus::UniformSampler gSpecular;

        oglplus::Uniform<glm::vec2> screenSize;
        oglplus::Uniform<glm::vec3> viewPosition;
        UniformLight directionalLight;

        oglplus::Uniform<float> ambientFactor;
        LightingProgram();
        virtual ~LightingProgram() {}

};

