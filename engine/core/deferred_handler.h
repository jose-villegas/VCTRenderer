#pragma once

#include "../scene/texture.h"
#include "geometry_buffer.h"

#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>

class LightingProgram;
class GeometryProgram;

/// <summary>
/// DeferredHandler holds the deferred rendering programs and
/// the geometry buffer object. It also handles rendering the
/// full screen quad during the light pass stage.
/// </summary>
class DeferredHandler
{
    protected:
        /// <summary>
        /// The geometry buffer which holds all the framebuffer
        /// with all the render target texture attachments
        /// </summary>
        GeometryBuffer geometryBuffer;
        /// <summary>
        /// The deferred rendering geometry pass program
        /// </summary>
        std::unique_ptr<GeometryProgram> geometryProgram;
        /// <summary>
        /// The deferred rendering lighting pass program
        /// </summary>
        std::unique_ptr<LightingProgram> lightingProgram;
        /// <summary>
        /// Renders a full screen quad, used in the light pass stage
        /// </summary>
        void RenderFullscreenQuad() const;
        /// <summary>
        /// Initializes a new instance of the <see cref="DeferredHandler"/> class.
        /// At construction the geometry buffer is set up with the window height
        /// and width for the texture's resolution.
        /// </summary>
        /// <param name="windowWith">The rendering window with.</param>
        /// <param name="windowHeight">The rendering window height.</param>
        DeferredHandler(unsigned int windowWith, unsigned int windowHeight);
        virtual ~DeferredHandler();
    private:
        // full screen quad
        oglplus::VertexArray fullscreenQuadVertexArray;
        oglplus::Buffer fullscreenQuadVertexBuffer;
        oglplus::Buffer fullscreenQuadElementBuffer;

        /// <summary>
        /// Loads the deferred rendering required shaders
        /// </summary>
        void LoadShaders();
        /// <summary>
        /// Setups the geometry buffer, initializes the render target
        /// textures and attaches these textures to the
        /// <see cref="GeometryProgram.framebuffer">
        /// </summary>
        /// <param name="windowWith">The rendering window width.</param>
        /// <param name="windowHeight">The rendering window height.</param>
        void SetupGeometryBuffer(unsigned int windowWith, unsigned int windowHeight);
        void CreateFullscreenQuad() const;
    public:
        const GeometryBuffer &GeometryBuffer() const { return geometryBuffer; }
};