#include "stdafx.h"
#include "node.h"

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
}
