#include "stdafx.h"
#include "vertex.h"

Vertex::Vertex()
{
    position = normal = tangent = bitangent = uv = glm::vec3(0.0f);
}

void Vertex::Orthonormalize()
{
    normal = glm::normalize(normal);
    // Gram-Schmidt orthonormalization
    this->tangent = glm::orthonormalize(tangent, normal);

    // secure handedness
    if(glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f)
    {
        tangent = tangent * -1.0f;
    }
}

