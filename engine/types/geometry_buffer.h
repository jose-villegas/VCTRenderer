#pragma once

#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/renderbuffer.hpp>
#include <oglplus/context.hpp>

/// <summary>
/// Describes the geometry buffer used in
/// deferred rendering. Holds all the render
/// target textures and the associated framebuffer.
/// </summary>
class GeometryBuffer
{
    public:
        /// <summary>
        /// Identifies the geometry buffer texture target
        /// </summary>
        enum RenderTargets
        {
            Normal, // texture with half float rgb precision storing normals
            Albedo, // texture with unsigned byte rgb precision storing albedo
            Specular, // texture with unsigned byte r precision storing specular
            Depth, // depth texture for completeness
        };
        void Bind(oglplus::FramebufferTarget target) const;
        void ActivateTextures() const;
        /// <summary>
        /// Returns the specified geometry buffer render texture.
        /// </summary>
        /// <param name="renderTarget">The render target.</param>
        /// <returns></returns>
        const oglplus::Texture &RenderTarget(RenderTargets renderTarget) const;
        /// <summary>
        /// Attaches the specified render texture to the geometry buffer
        /// framebuffer.
        /// </summary>
        /// <param name="renderTarget">The render target.</param>
        /// <param name="target">The target.</param>
        /// <param name="level">The level.</param>
        void AttachTexture(RenderTargets renderTarget,
                           oglplus::FramebufferTarget target, int level = 0);
        void DrawBuffers();

        GeometryBuffer();
        ~GeometryBuffer();
    private:
        oglplus::Framebuffer geometryBuffer;
        oglplus::Renderbuffer depthBuffer;
        std::array<oglplus::Texture, 4> bufferTextures;
        std::vector<oglplus::Context::ColorBuffer> colorBuffers;
};
