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
    this->oglVertexArray = std::unique_ptr<VertexArray>(new VertexArray());
    oglVertexArray->Bind();
    // ogl attrib flags set
    gl.ClearDepth(1.0f);
    gl.Enable(Capability::DepthTest);
    gl.Enable(Capability::CullFace);
    gl.FrontFace(FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // create vertex buffer object and upload vertex data
    oglArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglArrayBuffer->Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, this->vertices, BufferUsage::StaticDraw);
    // setup vertex data interleaving
    VertexArrayAttrib(0)	// position
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)0);
    VertexArrayAttrib(1)     // normals
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)12);
    VertexArrayAttrib(2)    // uvs
    .Pointer(2, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)24);
    VertexArrayAttrib(3)    // tangents
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)32);
    VertexArrayAttrib(4)    // bitangents
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)44);
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
    (program | 0)    // positions
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)0);
    (program | 1)    // normals
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)12);
    (program | 2)    // uvs
    .Pointer(2, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)24);
    (program | 3)    // tangents
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)32);
    (program | 4)    // bitangents
    .Pointer(3, oglplus::DataType::Float, false, sizeof(Vertex), (const void*)44);
}

void OGLMesh::BufferSetup(oglplus::Program &program)
{
    (program | 0)    // positions
    .Setup<float>(3).Enable();
    (program | 1)    // normals
    .Setup<float>(3).Enable();
    (program | 2)    // uvs
    .Setup<float>(2).Enable();
    (program | 3)    // tangents
    .Setup<float>(3).Enable();
    (program | 4)    // bitangents
    .Setup<float>(3).Enable();
}