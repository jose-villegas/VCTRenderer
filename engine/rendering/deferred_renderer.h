#pragma once

#include "deferred_handler.h"

class Material;
class RenderWindow;

class DeferredRenderer : public DeferredHandler
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
        void Render() const;
        /// <summary>
        /// Sets the matrices uniforms during geometry pass.
        /// </summary>
        void SetMatricesUniforms() const;
        /// <summary>
        /// Sets the material uniforms during geometry pass.
        /// </summary>
        /// <param name="mat">The mat.</param>
        void SetMaterialUniforms(std::shared_ptr<Material> &mat) const;
        /// <summary>
        /// Sets the light pass uniforms.
        /// </summary>
        void SetLightPassUniforms() const;
};
