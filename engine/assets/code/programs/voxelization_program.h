#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class VoxelizationProgram : public ProgramShader
{
    protected:
        struct UniformMaterial
        {
            oglplus::Uniform<glm::vec3> diffuse;
        };
        struct UniformMatrices
        {
            oglplus::Uniform<glm::mat4> model;
            oglplus::Uniform<glm::mat4> normal;
        };
        struct UniformLight
        {
            oglplus::Uniform<float> angleInnerCone;
            oglplus::Uniform<float> angleOuterCone;

            oglplus::Uniform<glm::vec3> diffuse;

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
        UniformMatrices matrices;
        UniformMaterial material;
        oglplus::UniformSampler diffuseMap;
        oglplus::Uniform<unsigned int> volumeDimension;

        std::array<oglplus::Uniform<glm::mat4>, 3> viewProjections;
        oglplus::Uniform<glm::mat4x4> lightViewProjection;
        oglplus::UniformSampler shadowMap;

        std::vector<UniformLight> directionalLight;
        std::vector<UniformLight> pointLight;
        std::vector<UniformLight> spotLight;
        std::array<oglplus::Uniform<unsigned int>, 3> lightTypeCount;


        void ExtractUniforms() override;

        VoxelizationProgram() = default;
        virtual ~VoxelizationProgram();
};

