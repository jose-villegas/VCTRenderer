#include "stdafx.h"
#include "node.h"
#include "core\engine_base.h"
#include <mutex>

Node::Node() : name("Default Node")
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scaling = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    transformChanged = true;
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
    if(transformChanged)
    {
        transformChanged = false;
        modelMatrix = glm::translate(position) *
                      glm::mat4_cast(rotation) *
                      glm::scale(scaling);
    }
}

void Node::DrawMeshes()
{
    static Renderer *renderer = &EngineBase::Instance()->GetRenderer();

    for(auto it = this->meshes.begin(); it != this->meshes.end(); ++it)
    {
        // frustum culling per mesh boundaries
        if(Renderer::FrustumCulling && this->meshes.size() > 1)
        {
            if(!renderer->viewFrustum.BoxInFrustum
               ((*it)->boundaries * renderer->transformMatrices
                .GetModel())) continue;
        }

        if(!(*it)->OnGPUMemory()) continue;

        (*it)->material->SetMaterialUniforms();
        (*it)->BindVertexArrayObject();
        (*it)->DrawElements();
    }
}

void Node::Draw()
{
    static Renderer *renderer = &EngineBase::Instance()->GetRenderer();
    // update matrices with node model matrix
    RecalculateModelMatrix();
    renderer->transformMatrices.UpdateModelMatrix(modelMatrix);
    renderer->transformMatrices.RecalculateMatrices();

    // frustum culling
    if(Renderer::FrustumCulling)
    {
        if(!renderer->viewFrustum.BoxInFrustum
           (boundaries * renderer->transformMatrices
            .GetModel())) return;
    }

    renderer->SetMatricesUniforms();
    // draw elements per mesh
    DrawMeshes();
}

void Node::DrawRecursive()
{
    static Renderer *renderer = &EngineBase::Instance()->GetRenderer();
    // update matrices with node model matrix
    RecalculateModelMatrix();
    // update with node model matrix
    renderer->transformMatrices.UpdateModelMatrix(modelMatrix);
    // recalculate model-dependant transform matrices
    renderer->transformMatrices.RecalculateMatrices();

    // frustum culling per node boundaries
    if(Renderer::FrustumCulling)
    {
        if(!renderer->viewFrustum.BoxInFrustum
           (boundaries * renderer->transformMatrices
            .GetModel())) return;
    }

    // set matrices uniform with updated matrices
    renderer->SetMatricesUniforms();
    // draw elements per mesh
    DrawMeshes();

    // draw descendants
    for(auto it = this->nodes.begin(); it != this->nodes.end(); ++it)
    {
        (*it).DrawRecursive();
    }
}

void Node::Transform(const glm::vec3 &position, const glm::vec3 &scaling,
                     const glm::quat &rotation)
{
    this->position = position;
    this->scaling = scaling;
    this->rotation = rotation;
    transformChanged = true;
}

void Node::Position(const glm::vec3 &position)
{
    this->position = position;
    transformChanged = true;
}

void Node::Scaling(const glm::vec3 &scaling)
{
    this->scaling = scaling;
    transformChanged = true;
}

void Node::Rotation(const glm::quat &rotation)
{
    this->rotation = rotation;
    transformChanged = true;
}
