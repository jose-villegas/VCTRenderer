#pragma once

#include "../core/renderer.h"
#include "../rendering/primitives/fullscreen_quad.h"

#include <oglplus/vertex_array.hpp>

#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/renderbuffer.hpp>
#include <glm/vec2.hpp>

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
class GIRenderer : public Renderer
{
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="DeferredRenderer"/>
        /// class.
        /// </summary>
        /// <param name="rWindow">The rendering window.</param>
        explicit GIRenderer(RenderWindow &window);
        /// <summary>
        /// Finalizes an instance of the <see cref="DeferredRenderer"/> class.
        /// </summary>
        ~GIRenderer();
        /// <summary>
        /// Renders a frame using deferred rendering
        /// </summary>
        void Render() override;

        const float &MaxTracingDistance() const;
        void MaxTracingDistance(const float &val);
        const float &GlobalIlluminationStrength() const;
        void GlobalIlluminationStrength(const float &val);
        const float &AmbientOclussionFalloff() const;
        void AmbientOclussionFalloff(const float &val);
        const float &AmbientOclussionAlpha() const;
        void AmbientOclussionAlpha(const float &val);
        oglplus::Texture &IndirectLightPass();
        void GlobalIlluminationScale(const float &val);
    private:
        FullscreenQuad fsQuad;
        /// <summary>
        /// Setups the geometry buffer, initializes the render target
        /// textures and attaches these textures to the
        /// <see cref="GeometryBuffer.geometryBuffer"/>
        /// </summary>
        /// <param name="windowWith">The rendering window width.</param>
        /// <param name="windowHeight">The rendering window height.</param>
        void SetupFramebuffer(unsigned int width, unsigned int height);

        oglplus::Framebuffer giFramebuffer;
        oglplus::Texture indirectPass;

        glm::uvec2 renderSize;

        float maxTracingDistance;
        float globalIlluminationStrength;
        float ambientOcclusionFalloff;
        float ambientOcclusionAlpha;
};
