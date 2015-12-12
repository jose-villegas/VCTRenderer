#pragma once

#include "../scene/texture.h"
#include "../types/engine_shader_program.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

enum class GBufferTextureId;

class LightingProgram : public EngineShaderProgram
{
    protected:
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

        void ExtractUniform(oglplus::SLDataType uType,
                            std::string uName) override;

        void SetUniform(GBufferTextureId tId, const int val);
        void SetUniform(const glm::vec3 &val, bool viewPosition = true);
};

