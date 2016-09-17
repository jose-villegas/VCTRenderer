#pragma once

#include "../types/vertex.h"
#include "../types/base_object.h"
#include "../types/bounding_box.h"

#include <vector>
#include <oglplus/buffer.hpp>
#include <oglplus/vertex_array.hpp>

class Material;

/// <summary>
/// Mesh base class, contains the mesh data
/// </summary>
/// <seealso cref="BaseObject" />
class Mesh : public BaseObject
{
    public:
        /// <summary>
        /// The mesh's boundaries
        /// </summary>
        BoundingBox boundaries;
        /// <summary>
        /// The mesh's vertices
        /// </summary>
        std::vector<Vertex> vertices;
        /// <summary>
        /// The mesh's indices
        /// </summary>
        std::vector<unsigned int> indices;
        /// <summary>
        /// The mesh's material
        /// </summary>
        std::shared_ptr<Material> material;

        Mesh();
        ~Mesh();
};

/// <summary>
/// Mesh drawer class contains the neccesary buffers to
/// draw the associated mesh data
/// </summary>
/// <seealso cref="Mesh" />
class MeshDrawer : public Mesh
{
    public:
        MeshDrawer();
        ~MeshDrawer();
        /// <summary>
        /// Binds the vertex array buffer.
        /// </summary>
        void BindArrayBuffer() const;
        /// <summary>
        /// Binds the indices array buffer.
        /// </summary>
        void BindElementArrayBuffer() const;
        /// <summary>
        /// Binds the vertex array object.
        /// </summary>
        void BindVertexArrayObject() const;
        /// <summary>
        /// Determines whether the mesh's vertex and element buffers, and
        /// the mesh's vertex array object have been initialized
        /// </summary>
        /// <returns>
        ///   <c>true</c> if this mesh is loaded; otherwise, <c>false</c>.
        /// </returns>
        bool IsLoaded() const;
        /// <summary>
        /// Initializes the mesh's vertex buffer, element buffers and vertex
        /// array object with the associated mesh data
        /// </summary>
        virtual void Load();
        /// <summary>
        /// Binds the vertex array object and makes a draw call for the
        /// elements buffer
        /// </summary>
        virtual void DrawElements() const;
    protected:
        std::shared_ptr<oglplus::Buffer> vertexBuffer;
        std::shared_ptr<oglplus::Buffer> elementBuffer;
        std::shared_ptr<oglplus::VertexArray> vertexArray;
        bool loaded;
        unsigned int indicesCount;
        unsigned int vertexCount;
};

