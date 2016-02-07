#pragma once
#include <GL/glew.h>
#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/context.hpp>

class GeometryBuffer
{
    public:
        /// <summary>
        /// Identifies the geometry buffer texture target
        /// </summary>
        enum RenderTargets
        {
            Position, // texture with half float rgb precision storing positions
            Normal, // texture with half float rgb precision storing normals
            Albedo, // texture with unsigned byte rgb precision storing albedo
            Specular, // texture with unsigned byte r precision storing specular
            Depth, // depth buffer for completeness
            TARGETS_MAX
        };
        oglplus::Texture &RenderTarget(RenderTargets renderTarget);
        void Bind(oglplus::FramebufferTarget target) const;
        void AttachTexture(RenderTargets renderTarget,
                           oglplus::FramebufferTarget target =
                               oglplus::FramebufferTarget::Draw, int level = 0);

        void DrawBuffers() const;
        void ActivateTextures();

        GeometryBuffer();
    private:
        oglplus::Framebuffer geometryBuffer;
        std::array<oglplus::Texture, TARGETS_MAX> bufferTextures;
        std::vector<oglplus::Context::ColorBuffer> colorBuffers;
};


