#include "node.h"

#include <tbb/tbb.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "camera.h"
#include "../core/engine_base.h"
#include "../rendering/deferred_renderer.h"

bool Node::loopCameraModified = true;

Node::Node() : outsideFrustum(false)
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
    // checks if we need to redo InFrustum checks per nodes
    loopCameraModified = camera->ParametersChanged();

    // draw elements using draw list
    for (auto &node : drawList)
    {
        // no need to check or update boundaries and frustum planes.
        if (Renderer::UseFrustumCulling)
        {
            if (node->transform.changed) { node->UpdateBoundaries(); }

            // only two scenarios require updating outsideFrustum both when
            // either the camera or the node's transform have been modified
            if (loopCameraModified || node->transform.changed)
            {
                node->outsideFrustum = !camera->InFrustum(node->boundaries);
            }

            if (node->outsideFrustum) { continue; }
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

    // no need to update the model dependant matrices if the node's
    // transform or the camera matrices haven't been modified
    if (!loopCameraModified && !transform.changed) { return; }

    // if changed was true the transformation matrix has been already
    // updated in UpdateBoundaries. We set it to false to avoid
    // recalculating the transformation matrix.
    // if frustum culling is disabled, UpdateBoundaries isn't called
    // thus it is not updating the transform matrix
    if (transform.changed && Renderer::UseFrustumCulling)
    {
        transform.changed = false;
    }

    modelViewMatrix = camera->ViewMatrix() * transform.Matrix();
    modelViewProjectionMatrix = camera->ProjectionMatrix() * modelViewMatrix;

    // without frustum culling UpdateBoundaries didn't update the
    // transformation matrix,  it happens here instead, after use set
    // changed to true to avoid recalculating it unless it changes again.
    if (!Renderer::UseFrustumCulling && transform.changed)
    {
        transform.changed = false;
    }
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
