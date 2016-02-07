#pragma once

#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>

#include "../types/bounding_volume.h"

class OGLMesh;

class Node
{
    public:
        // node boundaries
        BoundingVolume boundaries;
        // node identifier
        std::string name;
        // meshes associated and children
        std::vector<std::shared_ptr<OGLMesh>> meshes;
        std::vector<std::shared_ptr<Node>> nodes;

        Node();
        ~Node();

        glm::mat4x4 GetModelMatrix() const;
        // draws only the meshes associated to the node
        void Draw();
        // draws the meshes associated to the parents and all descendants
        void DrawRecursive();
        // draws using the built drawList
        void DrawList();
        // transformations
        void Transform(const glm::vec3 &position,
                       const glm::vec3 &scaling,
                       const glm::quat &rotation);
        void Position(const glm::vec3 &position);
        void Scaling(const glm::vec3 &scaling);
        void Rotation(const glm::quat &rotation);
        void BuildDrawList();
    private:
        typedef std::reference_wrapper<Node> NodeRef;
        typedef std::shared_ptr<OGLMesh> MeshPtr;
        typedef std::pair <NodeRef, MeshPtr> DrawInfo;

        std::vector<DrawInfo> drawList;

        glm::mat4x4 modelMatrix;
        glm::vec3 position;
        glm::vec3 scaling;
        glm::quat rotation;

        void RecalculateModelMatrix();
        void BuildDrawList(std::vector<DrawInfo> &base);
        // call drawElements per mesh
        void DrawMeshes();
};

