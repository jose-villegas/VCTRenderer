#pragma once

#include "../types/base_object.h"
#include "../types/bounding_box.h"

#include <vector>
#include <oglplus/buffer.hpp>
#include <oglplus/vertex_array.hpp>

class Material;
struct Vertex;

class Mesh : public BaseObject
{
    public:
        // node boundaries
        BoundingBox boundaries;
        // mesh raw data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        // material used by mesh
        std::shared_ptr<Material> material;
        Mesh();
        ~Mesh();
};

class MeshDrawer : public Mesh
{
    public:
        MeshDrawer();
        ~MeshDrawer();
        // No copying or copy assignment allowed of this class
        MeshDrawer(MeshDrawer const &) = delete;
        MeshDrawer &operator=(MeshDrawer const &) = delete;
        void BindArrayBuffer() const;
        void BindElementArrayBuffer() const;
        void BindVertexArrayObject() const;
        bool IsLoaded() const;
        virtual void Load();
        virtual void DrawElements() const;
    protected:
        std::shared_ptr<oglplus::Buffer> vertexBuffer;
        std::shared_ptr<oglplus::Buffer> elementBuffer;
        std::shared_ptr<oglplus::VertexArray> vertexArray;
        bool loaded;
        unsigned int indicesCount;
        unsigned int vertexCount;
};

