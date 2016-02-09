#pragma once

#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>

#include "../types/bounding_volume.h"
#include "../util/single_active.h"

class Camera;
class OGLMesh;

class Node : public SingleActive<Node>
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
        virtual ~Node();

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

        const glm::mat4x4 &NormalMatrix() const;
        const glm::mat4x4 &ModelViewMatrix() const;
        const glm::mat4x4 &ModelViewProjectionMatrix() const;
    private:
        std::vector<Node *> drawList;

        glm::mat4x4 modelMatrix;
        glm::mat4x4 modelViewMatrix;
        glm::mat4x4 modelViewProjectionMatrix;
        glm::mat4x4 normalMatrix;

        glm::vec3 position;
        glm::vec3 scaling;
        glm::quat rotation;

        void ComputeModelMatrix();
        void BuildDrawList(std::vector<Node *> &base);
        // call drawElements per mesh
        void DrawMeshes();
        void ComputeMatrices();
        void UpdateBoundaries();
};

