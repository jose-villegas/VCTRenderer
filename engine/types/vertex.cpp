#include <glm/gtx/norm.hpp>
#include <glm/gtx/orthonormalize.hpp>

#include "vertex.h"

Vertex::Vertex()
{
    position = normal = tangent = bitangent = uv = glm::vec3(0.0f);
}

void Vertex::Orthonormalize()
{
    normal = normalize(normal);
    // Gram-Schmidt orthonormalization
    this->tangent = orthonormalize(tangent, normal);

    // secure handedness
    if (dot(cross(normal, tangent), bitangent) < 0.0f)
    {
        tangent = tangent * -1.0f;
    }
}

