#pragma once
#include "scene\texture.h"

class DeferredHandler
{
    public:
        enum GBufferTextureType
        {
            Position,
            Normal,
            Albedo,
            Specular,
            GBUFFER_TEXTURE_TYPE_MAX
        };
    private:
        static oglplus::Context gl;
        // deferred shader programs
        oglplus::Program geometryPass;
        oglplus::Program lightPass;
        oglplus::FragmentShader fsGeometryPass;
        oglplus::FragmentShader fsLightPass;
        oglplus::VertexShader vLightPass;
        oglplus::VertexShader vsGeometryPass;

        // geometry buffer
        oglplus::Framebuffer geomBuffer;
        oglplus::Texture bDepthTexture;
        std::vector<oglplus::Context::ColorBuffer> openedColorBuffers;
        std::array<oglplus::Texture, GBUFFER_TEXTURE_TYPE_MAX> bTextures;

        // full screen quad
        oglplus::VertexArray fsQuadVertexArray;
        oglplus::Buffer fsQuadVertexBuffer;
        oglplus::Buffer fsQuadElementBuffer;

        void LoadShaders();
        void InitializeGBuffer(unsigned int windowWith, unsigned int windowHeight);
        void CreateFullscreenQuad();
    public:
        DeferredHandler(unsigned int windowWith, unsigned int windowHeight);

        virtual ~DeferredHandler();

        void UseGeometryPass() const { geometryPass.Use(); }
        void UseLightPass() const { lightPass.Use(); }

        oglplus::Program &GetGeometryPass() { return geometryPass; }
        oglplus::Program &GetLightPass() { return lightPass; }
        static const std::vector<RawTexture::TextureType> &ProgramSamplers();
        static void SetSamplerUniform(oglplus::Program &program,
                                      RawTexture::TextureType texType);

        void BindGBuffer(const oglplus::FramebufferTarget &bindingMode);
        void ReadGBuffer(const GBufferTextureType &gBufferTexType);
        // returns texture handlers to gbuffer color buffers
        const std::array<oglplus::Texture, GBUFFER_TEXTURE_TYPE_MAX>
        &GetGBufferTextures() const { return bTextures; }
};

