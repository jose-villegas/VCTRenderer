#pragma once

#include "../scene/texture.h"
#include "../types/transform_matrices.h"
#include "../scene/material.h"
#include "../types/program_shader.h"
#include "../types/uniform_collection.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>
#include <oglplus/uniform_block.hpp>


/// <summary>
/// Identifies the geometry buffer texture target
/// </summary>
enum class GBufferTextureId
{
    Position, // texture with half float rgb precision storing positions
    Normal, // texture with half float rgb precision storing normals
    Albedo, // texture with unsigned byte rgb precision storing albedo
    Specular, // texture with unsigned byte r precision storing specular
    Depth, // depth buffer for completeness
    TEXTURE_TYPE_MAX
};

class GeometryProgram : public ProgramShader
{
    protected:
        void ExtractUniform(const oglplus::aux::ActiveUniformInfo &info) override;
    public:
        // fragment shader uniforms
        oglplus::Uniform<glm::vec3> materialDiffuse;
        oglplus::Uniform<glm::vec3> materialSpecular;
        oglplus::Uniform<unsigned int> materialUseNormalsMap;
        oglplus::UniformSampler diffuseMap;
        oglplus::UniformSampler specularMap;
        oglplus::UniformSampler normalsMap;
        oglplus::Uniform<float> alphaCutoff;
        // vertex shader uniforms
        oglplus::Uniform<glm::mat4> matricesNormal;
        oglplus::Uniform<glm::mat4> matricesModelView;
        oglplus::Uniform<glm::mat4> matricesModelViewProjection;

        GeometryProgram();
        virtual ~GeometryProgram() {}
};
