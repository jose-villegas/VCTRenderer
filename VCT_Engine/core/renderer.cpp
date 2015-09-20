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
    deferredHandler.LoadDeferredShaders();
}

void Renderer::Render(Scene &activeScene)
{
    static std::shared_ptr<EngineBase> engine = EngineBase::Instance();
    // activate geometry pass shader program
    deferredHandler.UseGeometryPass();
    // update view transform, default camera at [0] position
    matrices.UpdateProjectionMatrix(
        activeScene.cameras[engine->GetExecInfo().activeCamera]->GetProjecctionMatrix()
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
    this->model = std::move(rModel);
}

void TransformMatrices::UpdateViewMatrix(const glm::mat4x4 &rView)
{
    this->view = std::move(rView);
}

void TransformMatrices::UpdateProjectionMatrix(const glm::mat4x4 &rProjection)
{
    this->projection = std::move(rProjection);
}
