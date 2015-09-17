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
    // save number of faces and vertices for rendering
    this->indicesCount = this->indices.size();
    this->vertexCount = this->vertices.size();

    if(unloadFromRAM)
    {
        this->vertices.clear();
        this->indices.clear();
    }

    onGPUMemory = true;
}

void OGLMesh::UploadToGPU(oglplus::Program &program,
                          bool unloadFromRAM /*= true*/)
{
    if(oglElementArrayBuffer || oglArrayBuffer) return;

    // create buffers
    oglElementArrayBuffer = std::unique_ptr<oglplus::Buffer>(new oglplus::Buffer());
    oglArrayBuffer = std::unique_ptr<oglplus::Buffer>(new oglplus::Buffer());
    // upload indices
    oglElementArrayBuffer->Bind(oglplus::Buffer::Target::ElementArray);
    oglplus::Buffer::Data(oglplus::Buffer::Target::ElementArray, this->indices,
                          oglplus::BufferUsage::StaticDraw);
    oglArrayBuffer->Bind(oglplus::Buffer::Target::Array);
    // upload whole vertex data to buffer
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, this->vertices,
                          oglplus::BufferUsage::StaticDraw);
    /*                     setup vertex distribution                     */
    // positions
    (program | 0).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)0);
    // normals
    (program | 1).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)12);
    // uvs
    (program | 2).Pointer(2, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)24);
    // tangents
    (program | 3).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)32);
    // bitangents
    (program | 4).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)44);
    // save number of faces and vertices for rendering
    this->indicesCount = this->indices.size();
    this->vertexCount = this->vertices.size();

    if(unloadFromRAM)
    {
        this->vertices.clear();
        this->indices.clear();
    }

    onGPUMemory = true;
}

void OGLMesh::BindBuffers()
{
    this->oglArrayBuffer->Bind(oglplus::Buffer::Target::Array);
}

void OGLMesh::DrawElements()
{
    gl.DrawElements(oglplus::PrimitiveType::Triangles, indicesCount,
                    oglplus::DataType::UnsignedInt);
}

void OGLMesh::SetupBufferPointers(oglplus::Program &program)
{
    // positions
    (program | 0).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)0);
    // normals
    (program | 1).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)12);
    // uvs
    (program | 2).Pointer(2, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)24);
    // tangents
    (program | 3).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)32);
    // bitangents
    (program | 4).Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex),
                          (const void*)44);
}
