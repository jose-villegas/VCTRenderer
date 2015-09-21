#include "stdafx.h"
#include "renderer.h"
#include "engine_base.h"

Renderer::Renderer()
{
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
    // activate geometry pass shader program
    deferredHandler.UseGeometryPass();
    // play with camera position
    activeScene.cameras[engine->GetExecInfo().activeCamera]->position =
        glm::vec3(
            std::cos(glfwGetTime()) * 5.0f, 0.0f, std::sin(glfwGetTime()) * 5.0f
        );
    // update view and projection transform, default camera at [0] position
    transformMatrices.UpdateProjectionMatrix(
        activeScene.cameras[engine->GetExecInfo().activeCamera]->GetProjecctionMatrix()
    );
    transformMatrices.UpdateViewMatrix(
        activeScene.cameras[engine->GetExecInfo().activeCamera]->GetViewMatrix()
    );
    // draw whole scene hierachy tree from root node
    activeScene.rootNode.DrawRecursive();
}

TransformMatrices::TransformMatrices()
{
}

TransformMatrices::~TransformMatrices()
{
}

void TransformMatrices::UpdateModelMatrix(const glm::mat4x4 &rModel)
{
    this->matrices.model = std::move(rModel);
}

void TransformMatrices::UpdateViewMatrix(const glm::mat4x4 &rView)
{
    this->matrices.view = std::move(rView);
}

void TransformMatrices::UpdateProjectionMatrix(const glm::mat4x4 &rProjection)
{
    this->matrices.projection = std::move(rProjection);
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
    Uniform<glm::mat4x4>(program, "matrix.modelViewProjection")
    .Set(matrices.modelViewProjection);
}
