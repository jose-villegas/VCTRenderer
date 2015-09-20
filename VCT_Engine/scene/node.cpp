#include "stdafx.h"
#include "node.h"
#include "core\engine_base.h"

Node::Node() : name("Default Node")
{
}


Node::~Node()
{
}

glm::mat4 & Node::GetTransformMatrix() const
{
    return glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(
               scaling);
}

void Node::Draw()
{
    if(meshes.empty()) return;

    // node model transform, position * rotation * scale
    EngineBase::Instance()->GetRenderer().matrices
    .UpdateModelMatrix(GetTransformMatrix());

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
