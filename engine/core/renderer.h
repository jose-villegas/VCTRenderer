#pragma once
#include "scene\scene.h"
#include "deferred_handler.h"
#include "interface\render_window.h"
#include "types\transform_matrices.h"
#include "types\frustum.h"

class Renderer
{
    private:
        static oglplus::Context gl;
        const RenderWindow * renderWindow;
        DeferredHandler deferredHandler;
    public:
        static bool FrustumCulling;

        TransformMatrices transformMatrices;
        Frustum viewFrustum;

        Renderer(const RenderWindow &rWindow);
        virtual ~Renderer();

        void Initialize();
        // calls drawing instructions
        void Render(Scene &activeScene, Camera &activeCamera);
        DeferredHandler &GetDeferredHandler() { return deferredHandler; }
        // program uniform setters
        void SetMatricesUniforms();
        void SetMaterialUniforms(const OGLMaterial &mat);
};
