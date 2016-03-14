#include "node.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "camera.h"
#include "../core/renderer.h"

Node::Node()
{
    name = "Default Node";
    ComputeMatrices();
    UpdateBoundaries();
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
    static const auto &renderer = Renderer::Active();

    for (auto &mesh : meshes)
    {
        if (!mesh->IsLoaded()) { return; }

        renderer->SetMaterialUniforms(*mesh->material);
        mesh->BindVertexArrayObject();
        mesh->DrawElements();
    }
}

void Node::DrawList()
{
    static const auto &renderer = Renderer::Active();;
    static const auto &camera = Camera::Active();

    // draw elements using draw list
    for (auto &node : drawList)
    {
        // no need to check or update boundaries and frustum planes.
        if (Renderer::UseFrustumCulling)
        {
            node->UpdateBoundaries();

            if (!camera->InFrustum(node->boundaries))
            {
                continue;
            }
        }

        // calculate model dependant matrices
        node->ComputeMatrices();
        // set matrices uniform with updated matrices
        renderer->SetMatricesUniforms(*node);
        // draw node meshes
        node->DrawMeshes();
    }
}

void Node::ComputeMatrices()
{
    static const auto &camera = Camera::Active();
    modelViewMatrix = camera->ViewMatrix() * transform.Matrix();
    modelViewProjectionMatrix = camera->ProjectionMatrix() * modelViewMatrix;
}

void Node::UpdateBoundaries()
{
    auto &model = transform.Matrix();
    boundaries.Transform(model);

    for (auto &mesh : meshes)
    {
        mesh->boundaries.Transform(model);
    }
}

void Node::BuildDrawList()
{
    drawList.clear();
    BuildDrawList(drawList);
}

const glm::mat4x4 &Node::ModeView() const
{
    return modelViewMatrix;
}

const glm::mat4x4 &Node::ModelViewProjection() const
{
    return modelViewProjectionMatrix;
}
