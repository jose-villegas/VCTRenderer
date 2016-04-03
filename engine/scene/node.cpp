#include "node.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "camera.h"
#include "../core/renderer.h"

#include <glm/detail/func_matrix.hpp>

void Node::UpdateTransformMatrix()
{
    Transform::UpdateTransformMatrix();
    // per transform change update inversetranpose
    inverseTransposeModel = inverse(transpose(Matrix()));
    // update boundaries with transform
    TransformBoundaries();
}

Node::Node()
{
    name = "Default Node";
    TransformBoundaries();
    inverseTransposeModel = inverse(transpose(Matrix()));
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
            if (!camera->InFrustum(node->boundaries))
            {
                continue;
            }
        }

        // set matrices uniform with updated matrices
        renderer->SetMatricesUniforms(*node);
        // draw node meshes
        node->DrawMeshes();
    }
}

void Node::BuildDrawList()
{
    drawList.clear();
    BuildDrawList(drawList);
}

const glm::mat4 Node::InverseTranspose() const
{
    return inverseTransposeModel;
}

void Node::TransformBoundaries()
{
    boundaries.Transform(transform.Matrix());

    for (auto &mesh : meshes)
    {
        mesh->boundaries.Transform(transform.Matrix());
    }
}
