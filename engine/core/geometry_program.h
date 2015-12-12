#pragma once

#include "../scene/texture.h"
#include "../types/transform_matrices.h"
#include "../scene/material.h"
#include "../types/engine_shader_program.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

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

class GeometryProgram : public EngineShaderProgram
{
    protected:
        /// <summary>
        /// Geometry shader program samplers
        /// </summary>
        UniformCollection<oglplus::UniformSampler,
                          RawTexture::TextureType> samplers;
        /// <summary>
        /// Geometry shader transform matrices
        /// </summary>
        UniformCollection<oglplus::Uniform<glm::mat4x4>,
                          TransformMatrices::MatrixId> transformMatrices;
        /// <summary>
        /// Geometry shader material vec3 float properties uniforms
        /// </summary>
        UniformCollection<oglplus::Uniform<glm::vec3>,
                          OGLMaterial::Float3PropertyId> materialFloat3;
        /// <summary>
        /// Geometry shader material float properties uniforms
        /// </summary>
        UniformCollection<oglplus::Uniform<float>,
                          OGLMaterial::Float1PropertyId> materialFloat1;
        /// <summary>
        /// Geometry shader material unsigned int (uint) properties
        /// </summary>
        UniformCollection<oglplus::Uniform<unsigned int>,
                          OGLMaterial::UInt1PropertyId> materialUInt1;
    public:

        GeometryProgram();
        virtual ~GeometryProgram() {}

        void ExtractUniform(oglplus::SLDataType uType,
                            std::string uName) override;

        void SetUniform(TransformMatrices::MatrixId mId,
                        const glm::mat4x4 &val);
        void SetUniform(RawTexture::TextureType tId,
                        const int val);
        void SetUniform(OGLMaterial::Float3PropertyId mF3Id,
                        const glm::vec3 &val);
        void SetUniform(OGLMaterial::Float1PropertyId mF1Id,
                        const float val);
        void SetUniform(OGLMaterial::UInt1PropertyId mF1Id,
                        const unsigned int val);

        const std::vector<RawTexture::TextureType>
        &ActiveSamplers() const;
        const std::vector<TransformMatrices::MatrixId>
        &ActiveTransformMatrices() const;
        const std::vector<OGLMaterial::Float3PropertyId>
        &ActiveMaterialFloat3Properties() const;
        const std::vector<OGLMaterial::Float1PropertyId>
        &ActiveMaterialFloat1Properties() const;
        const std::vector<OGLMaterial::UInt1PropertyId>
        &ActiveMaterialUInt1Properties()const;
};

