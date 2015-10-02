#include "stdafx.h"
#include "renderer.h"
#include "engine_base.h"

Renderer::Renderer(const RenderWindow &rWindow) : deferredHandler(
        rWindow.Settings().width, rWindow.Settings().height)
{
    renderWindow = &rWindow;
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
}

void Renderer::Render(Scene &activeScene)
{
    using namespace oglplus;
    static std::shared_ptr<EngineBase> engine = EngineBase::Instance();
    // bind gbuffer for writing
    deferredHandler.BindGBuffer(FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // opengl flags
    gl.ClearDepth(1.0f);
    //gl.Disable(Capability::Blend);
    gl.Enable(Capability::DepthTest);
    gl.Enable(Capability::CullFace);
    gl.FrontFace(FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // activate geometry pass shader program
    deferredHandler.UseGeometryPass();
    // play with camera position
    activeScene.cameras[engine->GetExecInfo().activeCamera]->position =
        glm::vec3(0.0f, -12.5f, 0.0f);
    activeScene.cameras[engine->GetExecInfo().activeCamera]->lookAt =
        glm::vec3(
            std::sin(glfwGetTime()),
            -12.5f,
            std::cos(glfwGetTime())
        );
    activeScene.cameras[engine->GetExecInfo().activeCamera]->clipPlaneFar =
        10000.0f;
    // update view and projection transform, default camera at [0] position
    transformMatrices.UpdateProjectionMatrix(
        activeScene.cameras[engine->GetExecInfo().activeCamera]->GetProjecctionMatrix()
    );
    transformMatrices.UpdateViewMatrix(
        activeScene.cameras[engine->GetExecInfo().activeCamera]->GetViewMatrix()
    );
    // draw whole scene hierachy tree from root node
    activeScene.rootNode.DrawRecursive();
    // unbind geom buffer
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
}

TransformMatrices::TransformMatrices()
{
}

TransformMatrices::~TransformMatrices()
{
}

void TransformMatrices::UpdateModelMatrix(const glm::mat4x4 &rModel)
{
    this->matrices.model = rModel;
}

void TransformMatrices::UpdateViewMatrix(const glm::mat4x4 &rView)
{
    this->matrices.view = rView;
}

void TransformMatrices::UpdateProjectionMatrix(const glm::mat4x4 &rProjection)
{
    this->matrices.projection = rProjection;
}

void TransformMatrices::RecalculateMatrices()
{
    matrices.modelView = matrices.view * matrices.model;
    matrices.modelViewProjection = matrices.projection * matrices.modelView;
    matrices.normal = glm::inverseTranspose(matrices.modelView);
}

void TransformMatrices::SetUniforms(oglplus::Program &program)
{
    using namespace oglplus;
    // set actually used uniforms
    Uniform<glm::mat4x4>(program, "matrices.modelViewProjection")
    .Set(matrices.modelViewProjection);
    Uniform<glm::mat4x4>(program, "matrices.modelView")
    .Set(matrices.modelView);
    Uniform<glm::mat4x4>(program, "matrices.normal")
    .Set(matrices.normal);
}
