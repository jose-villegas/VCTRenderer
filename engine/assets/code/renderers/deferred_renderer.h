#pragma once

#include "../core/renderer.h"

#include <memory>
#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>

class LightingProgram;
class GeometryProgram;
class GeometryBuffer;
class Material;
class RenderWindow;

/// <summary>
/// The main renderer for the engine. Uses a deferred rendering path.
/// Handles geometry pass and light pass logic, creates the necessary
/// geometry buffer and handles uniform pass.
/// </summary>
/// <seealso cref="DeferredHandler" />
class DeferredRenderer : public Renderer
{
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="DeferredRenderer"/>
        /// class.
        /// </summary>
        /// <param name="rWindow">The rendering window.</param>
        explicit DeferredRenderer(RenderWindow &window);
        /// <summary>
        /// Finalizes an instance of the <see cref="DeferredRenderer"/> class.
        /// </summary>
        ~DeferredRenderer();
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
        /// Renders a frame using deferred rendering
        /// </summary>
        void Render() override;
        /// <summary>
        /// Sets the matrices uniforms during geometry pass.
        /// </summary>
        void SetMatricesUniforms(const Node &node) const override;
        /// <summary>
        /// Sets the material uniforms during geometry pass.
        /// </summary>
        /// <param name="mat">The mat.</param>
        void SetMaterialUniforms(const Material &material) const override;
    private:
        /// <summary>
        /// The geometry program shader.
        /// </summary>
        /// <returns></returns>
        static GeometryProgram &GeometryPass();
        /// <summary>
        /// The light pass program shader.
        /// </summary>
        /// <returns></returns>
        static LightingProgram &LightingPass();
        /// <summary>
        /// Sets the light pass uniforms.
        /// </summary>
        void SetLightPassUniforms() const;
        /// <summary>
        /// Setups the geometry buffer, initializes the render target
        /// textures and attaches these textures to the
        /// <see cref="GeometryBuffer.geometryBuffer"/>
        /// </summary>
        /// <param name="windowWith">The rendering window width.</param>
        /// <param name="windowHeight">The rendering window height.</param>
        static void SetupGeometryBuffer(unsigned int windowWidth,
                                        unsigned int windowHeight);
        /// <summary>
        /// The geometry buffer which holds all the framebuffer
        /// with all the render target texture attachments
        /// </summary>
        static std::unique_ptr<GeometryBuffer> geometryBuffer;
        /// <summary>
        /// The full screen quad vertex array object
        /// </summary>
        oglplus::VertexArray fsQuadVertexArray;
        /// <summary>
        /// The full screen quad vertex buffer
        /// </summary>
        oglplus::Buffer fsQuadVertexBuffer;
        /// <summary>
        /// The full screen quad element index buffer
        /// </summary>
        oglplus::Buffer fsQuadElementBuffer;

        void CreateFullscreenQuad() const;
        /// <summary>
        /// The view matrix changed, used to check during rendering
        /// so light pass uniforms position and directions get updated.
        /// </summary>
        bool viewMatrixChanged;
};
