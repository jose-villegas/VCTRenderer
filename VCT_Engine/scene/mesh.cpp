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
    this->oglVertexArray = std::unique_ptr<VertexArray>(new VertexArray());
    oglVertexArray->Bind();
    // create vertex buffer object and upload vertex data
    oglArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglArrayBuffer->Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, this->vertices);
    // setup vertex data interleaving
    // position
    VertexArrayAttrib(VertexAttribSlot(0))
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid*)0).Enable();
    // uvs
    VertexArrayAttrib(VertexAttribSlot(1))
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid*)12).Enable();
    // normals
    VertexArrayAttrib(VertexAttribSlot(2))
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid*)24).Enable();
    // tangents
    VertexArrayAttrib(VertexAttribSlot(3))
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid*)36).Enable();
    // bitangents
    VertexArrayAttrib(VertexAttribSlot(4))
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid*)48).Enable();
    // create element (indices) buffer object and upload data
    oglElementArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglElementArrayBuffer->Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices);
    // save number of faces and vertices for rendering
    this->indicesCount = (unsigned int)this->indices.size();
    this->vertexCount = (unsigned int)this->vertices.size();

    if(unloadFromRAM)
    {
        this->vertices.clear();
        this->indices.clear();
    }

    // unbind vao
    NoVertexArray().Bind();
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

void OGLMesh::BindVertexArrayObject()
{
    this->oglVertexArray->Bind();
}

void OGLMesh::DrawElements()
{
    gl.DrawElements(oglplus::PrimitiveType::Triangles, indicesCount,
                    oglplus::DataType::UnsignedInt);
}

void OGLMesh::BufferPointers(oglplus::Program &program)
{
    using namespace oglplus;
    (program | 0).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const GLvoid*)0).Enable();
    // uvs
    (program | 1).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const GLvoid*)12).Enable();
    // normals
    (program | 2).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const GLvoid*)24).Enable();
    // tangents
    (program | 3).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const GLvoid*)36).Enable();
    // bitangents
    (program | 4).Pointer(3, DataType::Float, false, sizeof(Vertex),
                          (const GLvoid*)48).Enable();
}

void OGLMesh::BufferSetup(oglplus::Program &program)
{
    (program | 0)	// positions
    .Setup<float>(3).Enable();
    (program | 1)	// normals
    .Setup<float>(3).Enable();
    (program | 2)	// uvs
    .Setup<float>(3).Enable();
    (program | 3)	// tangents
    .Setup<float>(3).Enable();
    (program | 4)	// bitangents
    .Setup<float>(3).Enable();
}