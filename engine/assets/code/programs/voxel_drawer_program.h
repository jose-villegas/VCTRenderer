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
        oglplus::Uniform<float> voxelSize;

        void ExtractUniforms() override;

        VoxelDrawerProgram() = default;
        virtual ~VoxelDrawerProgram();
};

