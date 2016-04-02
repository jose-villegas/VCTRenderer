#pragma once

#include "../types/scene_object.h"
#include "../types/bounding_box.h"
#include "../util/single_active.h"

#include <vector>

class Camera;
class MeshDrawer;

class Node : public SceneObject
{
    public:
        /// <summary>
        /// Node world space boundaries
        /// </summary>
        BoundingBox boundaries;
        /// <summary>
        /// All the node meshes
        /// </summary>
        std::vector<std::shared_ptr<MeshDrawer>> meshes;
        /// <summary>
        /// All the subnodes to this node
        /// </summary>
        std::vector<std::shared_ptr<Node>> nodes;
        Node();
        virtual ~Node();
        void DrawList();
        void BuildDrawList();
    private:
        std::vector<Node *> drawList;
        void BuildDrawList(std::vector<Node *> &base);
        // call drawElements per mesh
        void DrawMeshes();
        void TransformBoundaries();
};

