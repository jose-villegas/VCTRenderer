#pragma once
#include "../../../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class GIProgram : public ProgramShader
{
    public:
        void ExtractUniforms() override;
        oglplus::Uniform<glm::mat4x4> inverseProjectionView;

        oglplus::UniformSampler gDepth;
        oglplus::UniformSampler gNormal;
        oglplus::UniformSampler gAlbedo;
        oglplus::UniformSampler gSpecular;

        oglplus::Uniform<glm::vec3> cameraPosition;

        oglplus::Uniform<int> volumeDimension;
        oglplus::UniformSampler voxelTex;
        oglplus::UniformSampler voxelTexMipmap;
        oglplus::Uniform<float> voxelScale;
        oglplus::Uniform<glm::vec3> worldMinPoint;

        oglplus::Uniform<float> maxTracingDistanceGlobal;
        oglplus::Uniform<float> bounceStrength;
        oglplus::Uniform<float> aoFalloff;
        oglplus::Uniform<float> aoAlpha;
        oglplus::Uniform<unsigned int> enableAo;
};

class FinalCompositionProgram : public ProgramShader
{
    public:
        void ExtractUniforms() override;

        oglplus::UniformSampler directLight;
        oglplus::UniformSampler indirectLight;
};
