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
    public:
        UniformMatrices matrices;
        UniformMaterial material;
        oglplus::UniformSampler diffuseMap;
        oglplus::Uniform<unsigned int> volumeDimension;

        std::array<oglplus::Uniform<glm::mat4>, 3> viewProjections;
        oglplus::Uniform<glm::mat4> worldToVoxelTex;

        void ExtractUniforms() override;

        VoxelizationProgram() = default;
        virtual ~VoxelizationProgram();
};

