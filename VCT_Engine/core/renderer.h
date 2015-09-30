#pragma once
#include "scene\scene.h"
#include "deferred_handler.h"
#include "interface\render_window.h"

struct Matrices
{
    glm::mat4x4 modelView;
    glm::mat4x4 modelViewProjection;
    glm::mat4x4 model;
    glm::mat4x4 view;
    glm::mat4x4 projection;
    glm::mat4x4 normal;
};

class TransformMatrices
{
    public:
        TransformMatrices();
        virtual ~TransformMatrices();

        void UpdateModelMatrix(const glm::mat4x4 &rModel);
        void UpdateViewMatrix(const glm::mat4x4 &rView);
        void UpdateProjectionMatrix(const glm::mat4x4 &rProjection);

        void RecalculateMatrices();

        void SetUniforms(oglplus::Program &program);
    public:
        Matrices matrices;

        oglplus::Uniform<glm::mat4x4> uModelView;
        oglplus::Uniform<glm::mat4x4> uModelViewProjection;
        oglplus::Uniform<glm::mat4x4> uModel;
        oglplus::Uniform<glm::mat4x4> uView;
        oglplus::Uniform<glm::mat4x4> uProjection;
        oglplus::Uniform<glm::mat4x4> uNormal;
};

class Renderer
{
    private:
        static oglplus::Context gl;
        const RenderWindow * renderWindow;
        DeferredHandler deferredHandler;
    public:
        TransformMatrices transformMatrices;

        Renderer(const RenderWindow &rWindow);
        virtual ~Renderer();

        void Initialize();
        // calls drawing instructions
        void Render(Scene &activeScene);
        DeferredHandler &GetDeferredHandler() { return deferredHandler; }
};
