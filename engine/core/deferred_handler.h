#pragma once

#include "../scene/texture.h"
#include "../types/transform_matrices.h"
#include "../scene/material.h"
#include "../types/program_base.h"

#include <oglplus/framebuffer.hpp>
#include <oglplus/context.hpp>
#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>
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

class GeometryProgram : public DeferredProgram
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

class LightingProgram : public DeferredProgram
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

class DeferredHandler
{
    public:
        GeometryProgram geometryProgram;
        LightingProgram lightingProgram;

        DeferredHandler(unsigned int windowWith, unsigned int windowHeight);
        virtual ~DeferredHandler();
        void BindGeometryBuffer(const oglplus::FramebufferTarget &bindingMode);
        void ReadGeometryBuffer(const GBufferTextureId
                                &bufferTexId);
        void ActivateBindTextureTargets();
        void RenderFullscreenQuad();

        /// <summary>
        /// Returns an array with all the geometry buffer textures
        /// </summary>
        /// <returns>Array of textures</returns>
        const std::array<oglplus::Texture, static_cast<size_t>(
            GBufferTextureId::TEXTURE_TYPE_MAX)> &BufferTextures() const;
    private:
        oglplus::Framebuffer geometryBuffer;
        std::array<oglplus::Texture, static_cast<size_t>(
            GBufferTextureId::TEXTURE_TYPE_MAX)> bufferTextures;
        std::vector<oglplus::Context::ColorBuffer> colorBuffers;
        // full screen quad
        oglplus::VertexArray fullscreenQuadVertexArray;
        oglplus::Buffer fullscreenQuadVertexBuffer;
        oglplus::Buffer fullscreenQuadElementBuffer;
        void LoadShaders();
        void SetupGBuffer(unsigned int windowWith,
                          unsigned int windowHeight);
        void CreateFullscreenQuad();
};