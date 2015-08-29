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
    if(oglElementArrayBuffer || oglArrayBuffer) return;

    using namespace oglplus;
    // create buffers
    oglElementArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    // upload indices
    oglElementArrayBuffer->Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices,
                 BufferUsage::StaticDraw);
    oglArrayBuffer->Bind(Buffer::Target::Array);
    // upload whole vertex data to buffer
    Buffer::Data(Buffer::Target::Array, this->vertices, BufferUsage::StaticDraw);
    /*                     setup vertex distribution                     */
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

    if(unloadFromRAM)
    {
        this->vertices.clear();
        this->indices.clear();
    }
}

void OGLMesh::UploadToGPU(oglplus::Program &program,
                          bool unloadFromRAM /*= true*/)
{
    if(oglElementArrayBuffer || oglArrayBuffer) return;

    using namespace oglplus;
    // create buffers
    oglElementArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    // upload indices
    oglElementArrayBuffer->Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices,
                 BufferUsage::StaticDraw);
    oglArrayBuffer->Bind(Buffer::Target::Array);
    // upload whole vertex data to buffer
    Buffer::Data(Buffer::Target::Array, this->vertices, BufferUsage::StaticDraw);
    /*                     setup vertex distribution                     */
    // positions
    (program | 0).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const void*)0);
    // normals
    (program | 1).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const void*)12);
    // uvs
    (program | 2).Pointer(2, DataType::Float, false, sizeof(Vertex),
                          (const void*)24);
    // tangents
    (program | 3).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const void*)32);
    // bitangents
    (program | 4).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const void*)44);

    if(unloadFromRAM)
    {
        this->vertices.clear();
        this->indices.clear();
    }
}
