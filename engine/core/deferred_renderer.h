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

        explicit DeferredRenderer(const RenderWindow &rWindow);
        virtual ~DeferredRenderer();

        void Initialize();
        // calls drawing instructions
        void Render();
        DeferredHandler &GetDeferredHandler();
        // program uniform setters
        void SetMatricesUniforms();
        void SetMaterialUniforms(std::shared_ptr<OGLMaterial> &mat);
        void SetLightPassUniforms();
        // frustum culling
        bool InFrustum(const BoundingVolume &volume);
        TransformMatrices &Matrices();
    private:
        DeferredHandler deferredHandler;
        Frustum viewFrustum;
        TransformMatrices transformMatrices;
};
