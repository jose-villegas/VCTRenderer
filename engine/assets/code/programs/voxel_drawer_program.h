#pragma once
#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class VoxelDrawerProgram : public ProgramShader
{
    protected:
        struct UniformMatrices
        {
            oglplus::Uniform<glm::mat4> modelViewProjection;
        };
    public:
        UniformMatrices matrices;
        oglplus::Uniform<unsigned int> volumeDimension;
        std::array<oglplus::Uniform<glm::vec4>, 6> frustumPlanes;
        oglplus::Uniform<float> voxelSize;
        oglplus::Uniform<glm::vec3> worldMinPoint;

        void ExtractUniforms() override;

        VoxelDrawerProgram() = default;
        virtual ~VoxelDrawerProgram();
};

