#include "mesh.h"

#include "../types/vertex.h"

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



OGLMesh::OGLMesh() : onGPUMemory(false)
{
}

OGLMesh::~OGLMesh()
{
}

void OGLMesh::UploadToGPU(bool unloadFromRAM /*= true*/)
{
    if (oglElementArrayBuffer || oglArrayBuffer) { return; }

    using namespace oglplus;
    this->oglVertexArray = std::unique_ptr<VertexArray>(new VertexArray());
    oglVertexArray->Bind();
    // create vertex buffer object and upload vertex data
    oglArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglArrayBuffer->Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, this->vertices);
    // setup vertex data interleaving
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)0);
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)12);
    VertexArrayAttrib(VertexAttribSlot(2)).Enable() // normals
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)24);
    VertexArrayAttrib(VertexAttribSlot(3)).Enable() // tangents
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)36);
    VertexArrayAttrib(VertexAttribSlot(4)).Enable() // bitangents
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)48);
    // create element (indices) buffer object and upload data
    oglElementArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglElementArrayBuffer->Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices);
    // save number of faces and vertices for rendering
    this->indicesCount = (unsigned int)this->indices.size();
    this->vertexCount = (unsigned int)this->vertices.size();

    if (unloadFromRAM)
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
    static oglplus::Context gl;
    gl.DrawElements(oglplus::PrimitiveType::Triangles, indicesCount,
                    oglplus::DataType::UnsignedInt);
}

void OGLMesh::BufferPointers(oglplus::Program &program)
{
    using namespace oglplus;
    (program | 0).Enable()
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)0);
    // uvs
    (program | 1).Enable()
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)12);
    // normals
    (program | 2).Enable()
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)24);
    // tangents
    (program | 3).Enable()
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)36);
    // bitangents
    (program | 4).Enable()
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const GLvoid *)48);
}

void OGLMesh::BufferSetup(oglplus::Program &program)
{
    (program | 0)   // positions
    .Setup<float>(3).Enable();
    (program | 1)   // normals
    .Setup<float>(3).Enable();
    (program | 2)   // uvs
    .Setup<float>(3).Enable();
    (program | 3)   // tangents
    .Setup<float>(3).Enable();
    (program | 4)   // bitangents
    .Setup<float>(3).Enable();
}