#pragma once
#include "scene\texture.h"

class DeferredHandler
{
    private:
        enum GBufferTextureType
        {
            Position,
            Normal,
            AlbedoSpecular,
            GBUFFER_TEXTURE_TYPE_MAX
        };
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

        void BindGBuffer(const oglplus::Framebuffer::Target &bindingMode);
        void ReadGBuffer(const GBufferTextureType &gBufferTexType);
};

