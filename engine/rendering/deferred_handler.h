#pragma once

#include <memory>

#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>

class GeometryBuffer;
class LightingProgram;
class GeometryProgram;

/// <summary>
/// DeferredHandler holds the deferred rendering programs and
/// the geometry buffer object. It also handles rendering the
/// full screen quad during the light pass stage.
/// </summary>
class DeferredHandler
{
    public:
        GeometryProgram &GeometryPass() const;
        LightingProgram &LightingPass() const;
        /// <summary>
        /// Renders a full screen quad, used in the light pass stage
        /// </summary>
        void DrawFullscreenQuad() const;
        /// <summary>
        /// Returns <see cref="geometryBuffer"/> which is the associated
        /// geometry buffer to this deferred path implementation.
        /// </summary>
        /// <returns></returns>
        static const GeometryBuffer &GBuffer();
        /// <summary>
        /// Setups the geometry buffer, initializes the render target
        /// textures and attaches these textures to the
        /// <see cref="GeometryBuffer.geometryBuffer"/>
        /// </summary>
        /// <param name="windowWith">The rendering window width.</param>
        /// <param name="windowHeight">The rendering window height.</param>
        static void SetupGeometryBuffer(unsigned int windowWith,
                                        unsigned int windowHeight);
        DeferredHandler();
        virtual ~DeferredHandler();
    private:
        /// <summary>
        /// The deferred rendering geometry pass program
        /// </summary>
        GeometryProgram * geometryProgram;
        /// <summary>
        /// The deferred rendering lighting pass program
        /// </summary>
        LightingProgram * lightingProgram;
        // full screen quad
        oglplus::VertexArray fsQuadVertexArray;
        oglplus::Buffer fsQuadVertexBuffer;
        oglplus::Buffer fsQuadElementBuffer;
        /// <summary>
        /// The geometry buffer which holds all the framebuffer
        /// with all the render target texture attachments
        /// </summary>
        static std::unique_ptr<GeometryBuffer> geometryBuffer;
        /// <summary>
        /// Loads the deferred rendering required shaders
        /// </summary>
        void LoadShaders();
        void CreateFullscreenQuad() const;
};