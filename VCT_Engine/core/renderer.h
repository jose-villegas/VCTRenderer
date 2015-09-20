#pragma once
#include "scene\scene.h"
#include "deferred_handler.h"


class TransformMatrices
{
    public:
        TransformMatrices();
        virtual ~TransformMatrices();

        void UpdateModelMatrix(const glm::mat4x4 &rModel);
        void UpdateViewMatrix(const glm::mat4x4 &rView);
        void UpdateProjectionMatrix(const glm::mat4x4 &rProjection);
    private:
        glm::mat4x4 modelView;
        glm::mat4x4 modelViewProjection;
        glm::mat4x4 model;
        glm::mat4x4 view;
        glm::mat4x4 projection;
        glm::mat4x4 normal;
};


class Renderer
{
    private:
        DeferredHandler deferredHandler;
    public:
        TransformMatrices matrices;

        Renderer();
        virtual ~Renderer();

        void Initialize();
        // calls drawing instructions
        void Render(Scene &activeScene);
};
