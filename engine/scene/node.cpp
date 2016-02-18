#include "node.h"

#include <tbb/tbb.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "mesh.h"
#include "camera.h"
#include "../core/engine_base.h"
#include "../rendering/deferred_renderer.h"

Node::Node() : outsideFrustum(true)
{
    name = "Default Node";
}


Node::~Node()
{
}

void Node::BuildDrawList(std::vector<Node *> &base)
{
    base.push_back(this);

    for (auto &node : nodes)
    {
        node->BuildDrawList(base);
    }
}

void Node::DrawMeshes()
{
    static const auto &renderer = EngineBase::Renderer();

    for (auto &mesh : meshes)
    {
        if (!mesh->IsLoaded()) { return; }

        if (meshes.size() > 1 && !Camera::Active()->InFrustum(mesh->boundaries))
        {
            continue;
        }

        renderer.SetMaterialUniforms(mesh->material);
        mesh->BindVertexArrayObject();
        mesh->DrawElements();
    }
}

void Node::Draw()
{
    static const auto &camera = Camera::Active();
    static const auto &renderer = EngineBase::Renderer();

    if (transform.changed) { UpdateBoundaries(); }

    if (!camera->InFrustum(boundaries))
    {
        return;
    }

    // set this node as rendering active
    SetAsActive();
    // recalculate model-dependent transform matrices
    ComputeMatrices();
    // set matrices uniform with updated matrices
    renderer.SetMatricesUniforms();
    // draw elements per mesh
    DrawMeshes();
}

void Node::DrawRecursive()
{
    Draw();

    for (auto &node : nodes)
    {
        node->DrawRecursive();
    }
}

void Node::DrawList()
{
    static const auto &renderer = EngineBase::Renderer();
    // refresh all nodes with proper data
    PoolDrawList();

    // we can conclude that if the root node
    // is culled all of its childs are also
    if (drawList[0]->outsideFrustum) { return; }

    // draw elements using draw list
    for (auto node : drawList)
    {
        if (node->outsideFrustum) { continue; }

        // set this node as rendering active
        node->SetAsActive();
        // set matrices uniform with updated matrices
        renderer.SetMatricesUniforms();
        // draw node meshes
        node->DrawMeshes();
    }
}

void Node::ComputeMatrices()
{
    static const auto &camera = Camera::Active();
    modelViewMatrix = camera->ViewMatrix() * transform.ToMatrix();
    normalMatrix = modelViewMatrix;
    modelViewProjectionMatrix = camera->ProjectionMatrix() * modelViewMatrix;
    // since we already obtained the model matrix the changed flag is no
    // longer necessary. This is in case UpdateBoundaries didn't get called
    transform.changed = false;
}

void Node::UpdateBoundaries()
{
    auto &model = transform.ToMatrix();
    boundaries.Transform(model);
    // since we already obtained the model matrix
    // the changed flag is no longer necessary
    transform.changed = false;

    for (auto &mesh : meshes)
    {
        mesh->boundaries.Transform(model);
    }
}

void Node::PoolDrawList()
{
    static const auto &camera = Camera::Active();

    if (!camera->ParametersChanged() && !transform.changed) { return; }

    if (transform.changed) { UpdateBoundaries(); }

    // cull root node to compute frustum planes and
    // avoid thread collisions on frustum planes
    if (camera->InFrustum(boundaries))
    {
        outsideFrustum = false;
        ComputeMatrices();
    }
    // we can conclude that if the root node
    // is culled all of its childs are also
    else
    {
        outsideFrustum = true;
        return;
    }

    // update all frustum culling statuses and model-depedent matrices
    // starts from 1 since first node is root already checked.
    tbb::parallel_for(size_t(1), drawList.size(), [ = ](size_t i)
    {
        auto &node = drawList[i];

        if (node->transform.changed) { node->UpdateBoundaries(); }

        if (camera->InFrustum(node->boundaries))
        {
            node->outsideFrustum = false;
            node->ComputeMatrices();
        }
        else
        {
            node->outsideFrustum = true;
        }
    });
}

void Node::BuildDrawList()
{
    drawList.clear();
    BuildDrawList(drawList);
}

const glm::mat4x4 &Node::NormalMatrix() const
{
    return normalMatrix;
}

const glm::mat4x4 &Node::ModelViewMatrix() const
{
    return modelViewMatrix;
}

const glm::mat4x4 &Node::ModelViewProjectionMatrix() const
{
    return modelViewProjectionMatrix;
}
