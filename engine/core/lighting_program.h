#pragma once

#include "../scene/texture.h"
#include "../types/program_shader.h"
#include "../types/uniform_collection.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>
#include <oglplus/uniform_block.hpp>
#include <oglplus/buffer.hpp>

enum class GBufferTextureId;

class LightingProgram : public ProgramShader
{
    public:
        // light pass uniforms
        struct LightUniform
        {
            oglplus::Uniform<float> angleInnerCone;
            oglplus::Uniform<float> angleOuterCone;
            oglplus::Uniform<glm::vec3> ambient;
            oglplus::Uniform<glm::vec3> diffuse;
            oglplus::Uniform<glm::vec3> specular;

            oglplus::Uniform<glm::vec3> position;
            oglplus::Uniform<glm::vec3> direction;

            struct AttenuationUniform
            {
                oglplus::Uniform<float> constant;
                oglplus::Uniform<float> linear;
                oglplus::Uniform<float> quadratic;
            } attenuation;

            oglplus::Uniform<unsigned int> lightType;
        };

        LightUniform directionalLight;
        std::vector<LightUniform> pointLight;
        std::vector<LightUniform> spotLight;
        oglplus::Uniform<glm::vec3> viewPosUniform;

        UniformCollection<oglplus::UniformSampler, GBufferTextureId> samplers;

    public:
        LightingProgram();
        virtual ~LightingProgram() {}

        void ExtractUniform(const oglplus::aux::ActiveUniformInfo &info) override;

        void SetUniform(GBufferTextureId tId, const int val);
        void SetUniform(const glm::vec3 &val, bool viewPosition = true);

        oglplus::UniformBlock uBlock;
        oglplus::Buffer ubo;
};

