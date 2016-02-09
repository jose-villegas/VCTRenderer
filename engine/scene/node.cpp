#include "node.h"

#include <tbb/tbb.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "mesh.h"
#include "camera.h"
#include "../core/engine_base.h"
#include "../core/deferred_renderer.h"

Node::Node() : name("Default Node"), outsideFrustum(true)
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scaling = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    GetModelMatrix(); // initially as invalid
}


Node::~Node()
{
}

glm::mat4x4 Node::GetModelMatrix() const
{
    return modelMatrix;
}

void Node::ComputeModelMatrix()
{
    modelMatrix = translate(position) * mat4_cast(rotation) * scale(scaling);
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
    modelViewMatrix = camera->ViewMatrix() * modelMatrix;
    normalMatrix = modelViewMatrix;
    modelViewProjectionMatrix = camera->ProjectionMatrix() * modelViewMatrix;
}

void Node::Transform(const glm::vec3 &position, const glm::vec3 &scaling,
                     const glm::quat &rotation)
{
    this->position = position;
    this->scaling = scaling;
    this->rotation = rotation;
    ComputeModelMatrix();
}

void Node::Position(const glm::vec3 &position)
{
    if (position != this->position)
    {
        this->position = position;
        ComputeModelMatrix();
    }
}

void Node::Scaling(const glm::vec3 &scaling)
{
    if (scaling != this->scaling)
    {
        this->scaling = scaling;
        ComputeModelMatrix();
        UpdateBoundaries();
    }
}

void Node::UpdateBoundaries()
{
    boundaries.Transform(modelMatrix);

    for (auto &mesh : meshes)
    {
        mesh->boundaries.Transform(modelMatrix);
    }
}

void Node::PoolDrawList()
{
    static const auto &camera = Camera::Active();

    if (!camera->ParametersChanged()) { return; }

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

void Node::Rotation(const glm::quat &rotation)
{
    if (rotation != this->rotation)
    {
        this->rotation = rotation;
        ComputeModelMatrix();
        boundaries.Transform(modelMatrix);
    }
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
