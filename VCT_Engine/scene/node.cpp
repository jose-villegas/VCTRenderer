#include "stdafx.h"
#include "node.h"
#include "core\engine_base.h"

Node::Node() : name("Default Node")
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scaling = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
}


Node::~Node()
{
}

glm::mat4 Node::GetTransformMatrix() const
{
    return glm::translate(position) *
           glm::mat4_cast(rotation) *
           glm::scale(scaling);
}

void Node::Draw()
{
    if(meshes.empty()) return;

    static Renderer *renderer = &EngineBase::Instance()->GetRenderer();
    renderer->transformMatrices.UpdateModelMatrix(GetTransformMatrix());
    renderer->transformMatrices.RecalculateMatrices();
    renderer->transformMatrices.SetUniforms(
        renderer->GetDeferredHandler().GetGeometryPass()
    );

    for(auto it = this->meshes.begin(); it != this->meshes.end(); ++it)
    {
        if(!(*it)->OnGPUMemory()) continue;

        (*it)->BindVertexArrayObject();
        (*it)->DrawElements();
    }
}

void Node::DrawRecursive()
{
    this->Draw();

    // draw descendants
    for(auto it = this->nodes.begin(); it != this->nodes.end(); ++it)
    {
        (*it).DrawRecursive();
    }
}
