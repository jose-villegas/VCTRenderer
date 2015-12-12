#include "node.h"

#include <glm/gtx/transform.hpp>

#include "mesh.h"
#include "../core/engine_base.h"
#include "../core/deferred_renderer.h"
#include "material.h"
#include <iomanip>

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

void Node::RecalculateModelMatrix()
{
    modelMatrix = translate(position) * mat4_cast(rotation) * scale(scaling);
}

void Node::BuildDrawList(std::vector<DrawInfo> &base)
{
    for (auto &mesh : meshes)
    {
        base.push_back(DrawInfo(std::ref<Node>(*this), mesh));
    }

    for (auto &node : nodes)
    {
        node->BuildDrawList(base);
    }

    // order by material name
    auto sortByMaterial = [](DrawInfo & rhs, DrawInfo & lhs) -> bool
    {
        return rhs.second->material < lhs.second->material;
    };
    std::sort(base.begin(), base.end(), sortByMaterial);
}

void Node::DrawMeshes()
{
    static auto &renderer = EngineBase::Renderer();

    for (auto &mesh : meshes)
    {
        if (!mesh->OnGPUMemory()) { return; }

        if (DeferredRenderer::FrustumCulling)
        {
            if (!renderer.InFrustum(mesh->boundaries.Transform(modelMatrix)))
            {
                continue;
            }
        }

        renderer.SetMaterialUniforms(mesh->material);
        mesh->BindVertexArrayObject();
        mesh->DrawElements();
    }
}

void Node::Draw()
{
    static auto &renderer = EngineBase::Renderer();
    // update matrices with node model matrix
    renderer.Matrices().UpdateModelMatrix(modelMatrix);
    // recalculate model-dependent transform matrices
    renderer.Matrices().RecalculateMatrices();

    if (DeferredRenderer::FrustumCulling)
    {
        if (!renderer.InFrustum(boundaries.Transform(modelMatrix)))
        {
            return;
        }
    }

    renderer.SetMatricesUniforms();
    // draw elements per mesh
    DrawMeshes();
}

void Node::DrawRecursive()
{
    static auto &renderer = EngineBase::Renderer();
    // update with node model matrix
    renderer.Matrices().UpdateModelMatrix(modelMatrix);
    // recalculate model-dependent transform matrices
    renderer.Matrices().RecalculateMatrices();

    // frustum culling per node boundaries
    if (DeferredRenderer::FrustumCulling)
    {
        if (!renderer.InFrustum(boundaries.Transform(modelMatrix)))
        {
            return;
        }
    }

    // set matrices uniform with updated matrices
    renderer.SetMatricesUniforms();
    // draw elements per mesh
    DrawMeshes();

    for (auto &node : nodes)
    {
        node->DrawRecursive();
    }
}

void Node::DrawList()
{
    static auto &renderer = EngineBase::Renderer();
    // update with node model matrix
    renderer.Matrices().UpdateModelMatrix(modelMatrix);
    // recalculate model-dependent transform matrices
    renderer.Matrices().RecalculateMatrices();

    // frustum culling node boundaries
    if (DeferredRenderer::FrustumCulling)
    {
        if (!renderer.InFrustum(boundaries.Transform(modelMatrix)))
        {
            return;
        }
    }

    // set matrices uniform with updated matrices
    renderer.SetMatricesUniforms();

    // draw elements using draw list
    for (auto &drawInfo : drawList)
    {
        auto &mesh = drawInfo.second;

        if (!mesh->OnGPUMemory()) { return; }

        if (DeferredRenderer::FrustumCulling)
        {
            auto &model = drawInfo.first.get().modelMatrix;

            if (!renderer.InFrustum(mesh->boundaries.Transform(model)))
            {
                continue;
            }
        }

        renderer.SetMaterialUniforms(mesh->material);
        mesh->BindVertexArrayObject();
        mesh->DrawElements();
    }
}

void Node::Transform(const glm::vec3 &position, const glm::vec3 &scaling,
                     const glm::quat &rotation)
{
    this->position = position;
    this->scaling = scaling;
    this->rotation = rotation;
    RecalculateModelMatrix();
}

void Node::Position(const glm::vec3 &position)
{
    this->position = position;
    RecalculateModelMatrix();
}

void Node::Scaling(const glm::vec3 &scaling)
{
    this->scaling = scaling;
    RecalculateModelMatrix();
}

void Node::Rotation(const glm::quat &rotation)
{
    this->rotation = rotation;
    RecalculateModelMatrix();
}

void Node::BuildDrawList()
{
    drawList.clear();
    BuildDrawList(drawList);
}