#pragma once
#include "scene\scene.h"
#include "deferred_handler.h"


class TransformMatrices
{
    public:
        TransformMatrices();
        virtual ~TransformMatrices();
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
        Renderer();
        virtual ~Renderer();

        void Initialize();
        // setups all objects for drawing frame
        void NewFrame();
        // calls drawing instructions
        void Draw(Scene &activeScene);
};
