#pragma once

#include <memory>
#include "../core/renderer.h"

class DeferredHandler;
class Material;
class RenderWindow;

/// <summary>
/// The main renderer for the engine.
/// Uses a deferred rendering path.
/// </summary>
/// <seealso cref="DeferredHandler" />
class DeferredRenderer : Renderer
{
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="DeferredRenderer"/> class.
        /// </summary>
        /// <param name="rWindow">The rendering window.</param>
        explicit DeferredRenderer(const RenderWindow &rWindow);
        virtual ~DeferredRenderer();

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
        std::unique_ptr<DeferredHandler> handler;
        bool viewMatrixChanged;
};
