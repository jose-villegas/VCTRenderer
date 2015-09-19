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

void Renderer::NewFrame()
{
    // activate geometry pass shader program
    deferredHandler.UseGeometryPass();
}

void Renderer::Draw(Scene &activeScene)
{
    activeScene.rootNode.DrawRecursive();
}

TransformMatrices::TransformMatrices()
{
}

TransformMatrices::~TransformMatrices()
{
}
