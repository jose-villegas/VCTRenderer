#pragma once
#include "scene\scene.h"
#include "deferred_handler.h"
#include "render_window.h"
#include "types\transform_matrices.h"
#include "types\frustum.h"

class DeferredRenderer
{
    public:
        static bool FrustumCulling;

        TransformMatrices transformMatrices;
        Frustum viewFrustum;

        DeferredRenderer(const RenderWindow &rWindow);
        virtual ~DeferredRenderer();

        void Initialize();
        // calls drawing instructions
        void Render();
        DeferredHandler &GetDeferredHandler() { return deferredHandler; }
        // program uniform setters
        void SetMatricesUniforms();
        void SetMaterialUniforms(const std::shared_ptr<OGLMaterial> &mat);
        void SetLightPassUniforms();
    private:
        const RenderWindow * renderWindow;
        DeferredHandler deferredHandler;
};
