#include "node.h"

#include <glm/gtx/transform.hpp>

#include "mesh.h"
#include "../core/engine_base.h"
#include "../core/deferred_renderer.h"
#include "camera.h"
#include <glm/gtc/matrix_inverse.inl>

Node::Node() : name("Default Node")
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
    for (auto &mesh : meshes)
    {
        if (!mesh->IsLoaded()) { return; }

        //if (meshes.size() > 1 && !Camera::Active()->InFrustum
        //        (mesh->boundaries.Transform(modelMatrix)))
        //{
        //    continue;
        //}
        EngineBase::Renderer().SetMaterialUniforms(mesh->material);
        mesh->BindVertexArrayObject();
        mesh->DrawElements();
    }
}

void Node::Draw()
{
    // recalculate model-dependent transform matrices
    ComputeMatrices(Camera::Active());
    // set this node as rendering active
    SetAsActive();

    if (!Camera::Active()->InFrustum(boundaries.Transform(modelMatrix)))
    {
        return;
    }

    EngineBase::Renderer().SetMatricesUniforms();
    // draw elements per mesh
    DrawMeshes();
}

void Node::DrawRecursive()
{
    // set this node as rendering active
    SetAsActive();

    if (!Camera::Active()->InFrustum(boundaries.Transform(modelMatrix)))
    {
        return;
    }

    // recalculate model-dependent transform matrices
    ComputeMatrices(Camera::Active());
    // set matrices uniform with updated matrices
    EngineBase::Renderer().SetMatricesUniforms();
    // draw elements per mesh
    DrawMeshes();

    for (auto &node : nodes)
    {
        node->DrawRecursive();
    }
}

void Node::DrawList()
{
    auto &camera = Camera::Active();
    auto &renderer = EngineBase::Renderer();

    // draw elements using draw list
    for (auto node : drawList)
    {
        // set this node as rendering active
        node->SetAsActive();
        //if (!camera->InFrustum(node->boundaries.Transform(node->modelMatrix)))
        //{
        //  continue;
        //}
        // recalculate model-dependent transform matrices
        node->ComputeMatrices(camera);
        // set matrices uniform with updated matrices
        renderer.SetMatricesUniforms();
        // draw node meshes
        node->DrawMeshes();
    }
}

void Node::ComputeMatrices(std::unique_ptr<Camera> &camera)
{
    modelViewMatrix = camera->ViewMatrix() * modelMatrix;
    normalMatrix = inverseTranspose(modelViewMatrix);
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
    this->position = position;
    ComputeModelMatrix();
}

void Node::Scaling(const glm::vec3 &scaling)
{
    this->scaling = scaling;
    ComputeModelMatrix();
}

void Node::Rotation(const glm::quat &rotation)
{
    this->rotation = rotation;
    ComputeModelMatrix();
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
