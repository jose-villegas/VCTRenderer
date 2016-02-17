#pragma once

#include "../types/scene_object.h"
#include "../types/bounding_box.h"
#include "../util/single_active.h"

#include <vector>
#include <glm/gtc/quaternion.hpp>

class Camera;
class MeshDrawer;

class Node : public SceneObject, public SingleActive<Node>
{
    public:
        // node boundaries
        BoundingBox boundaries;
        // meshes associated and children
        std::vector<std::shared_ptr<MeshDrawer>> meshes;
        std::vector<std::shared_ptr<Node>> nodes;
        Node();
        virtual ~Node();

        // draws only the meshes associated to the node
        void Draw();
        // draws the meshes associated to the parents and all descendants
        void DrawRecursive();
        // draws using the built drawList
        void DrawList();
        // transformations
        void BuildDrawList();

        const glm::mat4x4 &NormalMatrix() const;
        const glm::mat4x4 &ModelViewMatrix() const;
        const glm::mat4x4 &ModelViewProjectionMatrix() const;
    private:
        std::vector<Node *> drawList;

        glm::mat4x4 modelViewMatrix;
        glm::mat4x4 modelViewProjectionMatrix;
        glm::mat4x4 normalMatrix;

        bool outsideFrustum;

        void BuildDrawList(std::vector<Node *> &base);
        // call drawElements per mesh
        void DrawMeshes();
        void ComputeMatrices();
        void UpdateBoundaries();
        // updates all nodes in the drawlist
        void PoolDrawList();
};

