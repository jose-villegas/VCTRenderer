#pragma once

#include "scene_object.h"
#include "../types/bounding_box.h"

#include <vector>
#include <oglplus/buffer.hpp>
#include <oglplus/vertex_array.hpp>

class Material;
struct Vertex;

class Mesh : public SceneObject
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

        void FreeRawData();
};

class MeshDrawer : public Mesh
{
    protected:
        std::unique_ptr<oglplus::Buffer> oglArrayBuffer;
        std::unique_ptr<oglplus::Buffer> oglElementArrayBuffer;
        std::unique_ptr<oglplus::VertexArray> oglVertexArray;

        bool loaded;
        unsigned int indicesCount;
        unsigned int vertexCount;
    public:

        MeshDrawer();
        ~MeshDrawer();

        void Load();
        void BindArrayBuffer() const;
        void BindElementArrayBuffer() const;
        void BindVertexArrayObject() const;

        void DrawElements() const;
        bool IsLoaded() const { return loaded; }
    protected:
        // No copying or copy assignment allowed of this class or any derived class
        MeshDrawer(MeshDrawer const &) = delete;
        MeshDrawer &operator=(MeshDrawer const &) = delete;
};

