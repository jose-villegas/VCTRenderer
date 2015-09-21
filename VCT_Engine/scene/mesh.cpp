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
    // create vertex buffer object and upload vertex data
    oglArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglArrayBuffer->Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, this->vertices);
    // setup vertex data interleaving
    size_t vertexSize = sizeof(Vertex);
    DataType dType = DataType::Float;
    // position
    VertexArrayAttrib(0).Pointer(3, dType, 0, vertexSize, (void*)0).Enable();
    // normals
    VertexArrayAttrib(1).Pointer(3, dType, 0, vertexSize, (void*)12).Enable();
    // uvs
    VertexArrayAttrib(2).Pointer(2, dType, 0, vertexSize, (void*)24).Enable();
    // tangents
    VertexArrayAttrib(3).Pointer(3, dType, 0, vertexSize, (void*)32).Enable();
    // bitangents
    VertexArrayAttrib(4).Pointer(3, dType, 0, vertexSize, (void*)44).Enable();
    // create element (indices) buffer object and upload data
    oglElementArrayBuffer = std::unique_ptr<Buffer>(new Buffer());
    oglElementArrayBuffer->Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices);
    // ogl attrib flags set
    gl.ClearDepth(1.0f);
    gl.Enable(Capability::DepthTest);
    gl.Enable(Capability::CullFace);
    gl.FrontFace(FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
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
    using namespace oglplus;
    (program | 0)	// positions
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const void*)0).Enable();
    (program | 1)	// normals
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const void*)12).Enable();
    (program | 2)	// uvs
    .Pointer(2, DataType::Float, false, sizeof(Vertex), (const void*)24).Enable();
    (program | 3)	// tangents
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const void*)32).Enable();
    (program | 4)	// bitangents
    .Pointer(3, DataType::Float, false, sizeof(Vertex), (const void*)44).Enable();
}

void OGLMesh::BufferSetup(oglplus::Program &program)
{
    (program | 0)	// positions
    .Setup<float>(3).Enable();
    (program | 1)	// normals
    .Setup<float>(3).Enable();
    (program | 2)	// uvs
    .Setup<float>(2).Enable();
    (program | 3)	// tangents
    .Setup<float>(3).Enable();
    (program | 4)	// bitangents
    .Setup<float>(3).Enable();
}