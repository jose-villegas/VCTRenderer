#pragma once

class DeferredHandler
{
    private:
        enum GBufferTextureType
        {
            Position,
            Diffuse,
            Normal,
            TexCoord,
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

        oglplus::Framebuffer geomBuffer;
        oglplus::Texture bDepthTexture;
        std::array<oglplus::Texture, GBUFFER_TEXTURE_TYPE_MAX> bTextures;

        void LoadShaders();
        void InitializeGBuffer(unsigned int windowWith, unsigned int windowHeight);
    public:
        DeferredHandler(unsigned int windowWith, unsigned int windowHeight);

        virtual ~DeferredHandler();

        void UseGeometryPass() const { geometryPass.Use(); }
        void UseLightPass() const { lightPass.Use(); }

        oglplus::Program &GetGeometryPass() { return geometryPass; }
        oglplus::Program &GetLightPass() { return lightPass; }

        void BindGBuffer(oglplus::Framebuffer::Target &bindingMode);
};

