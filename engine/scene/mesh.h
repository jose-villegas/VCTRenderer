#pragma once

#include <string>
#include <vector>

// opengl and context creation headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// ogl headers c++ wrapper on opengl
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/opt/smart_enums.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/buffer_usage.hpp>
#include <oglplus/glsl_source.hpp>
#include <oglplus/glsl_string.hpp>
#include <oglplus/object/array.hpp>
#include <oglplus/object/group.hpp>
#include <oglplus/interop/glm.hpp>

#include "../types/bounding_box.h"

class OGLMaterial;
struct Vertex;

class Mesh
{
    public:
        // node boundaries
        BoundingBox boundaries;
        // mesh identifier
        std::string name;
        // mesh raw data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        // material used by mesh
        std::shared_ptr<OGLMaterial> material;

        Mesh();
        ~Mesh();

        void FreeRawData();
};

class OGLMesh : public Mesh
{
    protected:
        std::unique_ptr<oglplus::Buffer> oglArrayBuffer;
        std::unique_ptr<oglplus::Buffer> oglElementArrayBuffer;
        std::unique_ptr<oglplus::VertexArray> oglVertexArray;

        bool loaded;
        unsigned int indicesCount;
        unsigned int vertexCount;
    public:

        OGLMesh();
        ~OGLMesh();

        void Load();
        void BindArrayBuffer() const;
        void BindElementArrayBuffer() const;
        void BindVertexArrayObject() const;

        void DrawElements() const;
        bool IsLoaded() const { return loaded; }
    protected:
        // No copying or copy assignment allowed of this class or any derived class
        OGLMesh(OGLMesh const &) = delete;
        OGLMesh &operator=(OGLMesh const &) = delete;
};

