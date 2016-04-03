#pragma once

#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

/// <summary>
/// Contains all necessary uniforms for the <see cref="DeferredHandler"/>'s
/// lighting program for light pass.
/// </summary>
/// <seealso cref="ProgramShader" /
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
        oglplus::Uniform<glm::mat4x4> inverseProjectionView;
        oglplus::Uniform<glm::mat4x4> lightViewProjection;

        oglplus::UniformSampler gDepth;
        oglplus::UniformSampler gNormal;
        oglplus::UniformSampler gAlbedo;
        oglplus::UniformSampler gSpecular;
        oglplus::UniformSampler shadowMap;

        std::vector<UniformLight> directionalLight;
        std::vector<UniformLight> pointLight;
        std::vector<UniformLight> spotLight;
        std::array<oglplus::Uniform<unsigned int>, 3> lightTypeCount;

        oglplus::Uniform<glm::vec2> exponents;
        oglplus::Uniform<float> lightBleedingReduction;

        oglplus::Uniform<glm::mat4x4> worldToVoxelTex;
        oglplus::Uniform<int> volumeDimension;
        oglplus::UniformSampler voxelTex;

        void ExtractUniforms() override;

        LightingProgram() = default;
        virtual ~LightingProgram();
        LightingProgram(LightingProgram const &r) = delete;
        LightingProgram &operator=(LightingProgram const &r) = delete;
};

