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
    // create vertex buffer object and upload vertex data
    oglArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglArrayBuffer->Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, this->vertices, BufferUsage::StaticDraw);
    // create element (indices) buffer object and upload data
    oglElementArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglElementArrayBuffer->Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices,
                 BufferUsage::StaticDraw);
    // save number of faces and vertices for rendering
    this->indicesCount = (unsigned int)this->indices.size();
    this->vertexCount = (unsigned int)this->vertices.size();

    if(unloadFromRAM)
    {
        this->vertices.clear();
        this->indices.clear();
    }

    onGPUMemory = true;
}

void OGLMesh::BindArrayBuffer()
{
    this->oglArrayBuffer->Bind(oglplus::Buffer::Target::Array);
}

void OGLMesh::BindElementArrayBuffer()
{
    this->oglElementArrayBuffer->Bind(oglplus::Buffer::Target::ElementArray);
}

void OGLMesh::DrawElements()
{
    gl.DrawElements(oglplus::PrimitiveType::Triangles, indicesCount,
                    oglplus::DataType::UnsignedInt);
}

void OGLMesh::BufferPointers(oglplus::Program &program)
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

void OGLMesh::BufferSetup(oglplus::Program &program)
{
    // positions
    (program | 0).Setup<float>(3).Enable();
    // normals
    (program | 1).Setup<float>(3).Enable();
    // uvs
    (program | 2).Setup<float>(2).Enable();
    // tangents
    (program | 3).Setup<float>(3).Enable();
    // bitangents
    (program | 4).Setup<float>(3).Enable();
}