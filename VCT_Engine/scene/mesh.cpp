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

void OGLMesh::UploadToGPU(bool unloadFromRAM /*= true*/)
{
    using namespace oglplus;
    oglArrayBuffer.Bind(Buffer::Target::Array);
    // upload whole vertex data to buffer
    Buffer::Data(Buffer::Target::Array, this->vertices, BufferUsage::StaticDraw);
    /* setup vertex distribution */
    // positions
    VertexArrayAttrib(0).Pointer(3, DataType::Float, false, sizeof(Vertex),
                                 (const void*)0);
    // normals
    VertexArrayAttrib(1).Pointer(3, DataType::Float, false, sizeof(Vertex),
                                 (const void*)12);
    // uvs
    VertexArrayAttrib(2).Pointer(2, DataType::Float, false, sizeof(Vertex),
                                 (const void*)24);
    // tangents
    VertexArrayAttrib(3).Pointer(3, DataType::Float, false, sizeof(Vertex),
                                 (const void*)32);
    // bitangents
    VertexArrayAttrib(4).Pointer(3, DataType::Float, false, sizeof(Vertex),
                                 (const void*)44);
    oglElementArrayBuffer.Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices,
                 BufferUsage::StaticDraw);
}
