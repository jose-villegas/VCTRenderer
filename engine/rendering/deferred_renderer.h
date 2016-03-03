#pragma once

#include <memory>
#include "../core/renderer.h"
#include "deferred_handler.h"

class Material;
class RenderWindow;

/// <summary>
/// The main renderer for the engine.
/// Uses a deferred rendering path.
/// </summary>
/// <seealso cref="DeferredHandler" />
class DeferredRenderer : public Renderer, public DeferredHandler
{
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="DeferredRenderer"/> class.
        /// </summary>
        /// <param name="rWindow">The rendering window.</param>
        explicit DeferredRenderer(RenderWindow &window);
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
        /// <summary>
        /// Sets the light pass uniforms.
        /// </summary>
        void SetLightPassUniforms() const;
    private:
        bool viewMatrixChanged;
};
