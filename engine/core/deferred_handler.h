#pragma once
#include "scene\texture.h"
#include "types\transform_matrices.h"
#include "scene\material.h"
#include "scene\light.h"

/// <summary>
/// Base class for each program shader used in deferred rendering
/// </summary>
class DeferredProgram
{
    protected:
        ~DeferredProgram() {}
        friend class DeferredHandler;
        /// <summary>
        /// The fragment shader
        /// </summary>
        oglplus::FragmentShader fragmentShader;
        /// <summary>
        /// The vertex shader
        /// </summary>
        oglplus::VertexShader vertexShader;
        /// <summary>
        /// The program shader
        /// </summary>
        oglplus::Program program;
        /// <summary>
        /// Meant to be implemented in inheriting classes
        /// to extract uniform info such as binding location
        /// type, etc
        /// </summary>
        /// <param name="uType">Type of the uniform.</param>
        /// <param name="uName">Name of the uniform.</param>
        virtual void ExtractUniform(oglplus::SLDataType uType,
                                    std::string uName) = 0;

        template<typename T1, typename T2>
        class UniformCollection
        {
            public:
                const std::vector<T2> &Actives() const;
                void Resize(const size_t idCount);
                void Save(T2 id, T1 uniform);
                bool Has(const T2 &id) const;

                T1 &operator[](const T2 &id);
            private:
                std::vector<std::pair<T2, T1> *> links;
                std::vector<T2> actives;
                std::vector<T1> uniforms;
        };

    public:
        /// <summary>
        /// Identifies the geometry buffer texture target
        /// </summary>
        enum GBufferTextureId
        {
            Position, // texture with half float rgb precision storing positions
            Normal, // texture with half float rgb precision storing normals
            Albedo, // texture with unsigned byte rgb precision storing albedo
            Specular, // texture with unsigned byte r precision storing specular
            TEXTURE_TYPE_MAX
        };
        DeferredProgram() {};
        void ExtractActiveUniforms();
};

template<typename T1, typename T2>
void DeferredProgram::UniformCollection<T1, T2>::Resize(const size_t idCount)
{
    links.clear();
    links.resize(idCount);
}

template<typename T1, typename T2>
void DeferredProgram::UniformCollection<T1, T2>::Save(T2 id, T1 uniform)
{
    if (id < 0 && id >= links.size()) { return; }

    actives.push_back(std::move(id));
    uniforms.push_back(std::move(uniform));
    links[id] = new std::pair<T2, T1>(actives.back(), uniforms.back());;
}

template<typename T1, typename T2>
bool DeferredProgram::UniformCollection<T1, T2>::Has(const T2 &id) const
{
    return links[id] != nullptr;
}

template<typename T1, typename T2>
const std::vector<T2> &DeferredProgram::UniformCollection<T1, T2>::Actives()
const
{
    return actives;
}

template<typename T1, typename T2>
T1 &DeferredProgram::UniformCollection<T1, T2>::operator[](const T2 &id)
{
    return links[id]->second;
}

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

        // deferred shader programs handlers
        GeometryProgram geometryPass;
        LightingProgram lightPass;
    private:
        static oglplus::Context gl;
        // geometry buffer
        oglplus::Framebuffer geomBuffer;
        oglplus::Texture bDepthTexture;
        std::vector<oglplus::Context::ColorBuffer> openedColorBuffers;
        std::array<oglplus::Texture,
            static_cast<size_t>(DeferredProgram::TEXTURE_TYPE_MAX)> bTextures;
        // full screen quad
        oglplus::VertexArray fsQuadVertexArray;
        oglplus::Buffer fsQuadVertexBuffer;
        oglplus::Buffer fsQuadElementBuffer;

        void LoadShaders();
        void InitializeGBuffer(unsigned int windowWith,
                               unsigned int windowHeight);
        void CreateFullscreenQuad();
    public:
        DeferredHandler(unsigned int windowWith, unsigned int windowHeight);
        virtual ~DeferredHandler();

        void UseGeometryPass() const
        {
            geometryPass.program.Use();
        }

        void UseLightPass() const
        {
            lightPass.program.Use();
        }

        void BindGBuffer(const oglplus::FramebufferTarget &bindingMode);
        void ReadGBuffer(const DeferredProgram::GBufferTextureId &gBufferTexType);
        // binds and sets as active all gbuffer textures ids
        void ActivateGBufferTextures();
        void SetLightPassUniforms(const glm::vec3 &viewPosition,
                                  const std::vector<std::shared_ptr<Light>>
                                  &lights);
        // returns texture handlers to gbuffer color buffers
        const oglplus::Texture
        &GetGBufferTexture(DeferredProgram::GBufferTextureId tID) const;

        void RenderFSQuad();
};
