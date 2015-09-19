#include "stdafx.h"
#include "node.h"
#include "core\base.h"
using namespace VCT_ENGINE;

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
    for(auto it = this->meshes.begin(); it != this->meshes.end(); ++it)
    {
        if(!(*it)->OnGPUMemory()) continue;

        (*it)->BindVertexArray();
        (*it)->BindArrayBuffer();
        (*it)->BufferPointers(Base::Instance()->GetAssets().GetGeometryPassShader());
        (*it)->BindElementArrayBuffer();
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
