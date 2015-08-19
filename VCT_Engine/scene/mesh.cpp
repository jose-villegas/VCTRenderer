#include "stdafx.h"
#include "mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Vertex::Orthogonalize()
{
    // Gram-Schmidt orthogonalize
    this->tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));

    // Calculate handedness
    if(glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f)
    {
        tangent = tangent * -1.0f;
    }
}
