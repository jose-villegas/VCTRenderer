#include "stdafx.h"
#include "mesh.h"


Mesh::Mesh() : name("Default Mesh")
{
}


Mesh::~Mesh()
{
}

void Mesh::FreeRawData()
{
    this->vertices.clear();
    this->indices.clear();
}

void Vertex::Orthonormalize()
{
    // Gram-Schmidt orthonormalization
    this->tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));

    // Calculate handedness
    if(glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f)
    {
        tangent = tangent * -1.0f;
    }
}

OGLMesh::OGLMesh() : onGPUMemory(false)
{
}

OGLMesh::~OGLMesh()
{
}
