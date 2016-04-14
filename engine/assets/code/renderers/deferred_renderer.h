#pragma once

#include "../core/renderer.h"
#include "../rendering/primitives/fullscreen_quad.h"

#include <oglplus/vertex_array.hpp>

#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/renderbuffer.hpp>

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
        const std::array<oglplus::Texture, 5> &BufferTextures() const;
        const float &MaxTracingDistance() const;
        void MaxTracingDistance(const float &val);
        const float &GlobalIlluminationStrength() const;
        void GlobalIlluminationStrength(const float &val);
        const float &AmbientOclussionFalloff() const;
        void AmbientOclussionFalloff(const float &val);
        const float &AmbientOclussionAlpha() const;
        void AmbientOclussionAlpha(const float &val);
        const unsigned int &RenderMode() const;
        void RenderMode(const unsigned int &mode);
    private:
        FullscreenQuad fsQuad;
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
        void SetupGeometryBuffer(unsigned int windowWidth, unsigned int windowHeight);

        oglplus::Framebuffer geometryBuffer;
        oglplus::Renderbuffer depthBuffer;
        std::array<oglplus::Texture, 5> bufferTextures;

        float maxTracingDistance;
        float globalIlluminationStrength;
        float ambientOcclusionFalloff;
        float ambientOcclusionAlpha;
        unsigned int renderMode;
};
