#pragma once

#include "../types/scene_object.h"
#include "../types/bounding_box.h"
#include "../util/single_active.h"

#include <vector>

class Camera;
class MeshDrawer;

/// <summary>
/// Describes a node in scene. A node can contain
/// an undefined number of meshes and subnodes.
/// </summary>
/// <seealso cref="SceneObject" />
class Node : public SceneObject
{
    public:
        enum GeometryState
        {
            Dynamic,
            Static,
        };
        /// <summary>
        /// The node geometry state, flags this mesh
        /// as dynamic or static
        /// </summary>
        GeometryState nodeState;
        /// <summary>
        /// Updates the transform matrix, the boundaries
        /// and the inverse transpose
        /// </summary>
        void UpdateTransformMatrix() override;
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
        /// <summary>
        /// If <see cref="BuildDrawList"/>  has been called before,
        /// at least once. Draws all the meshes associated with this
        ///  node and its subnodes.
        /// </summary>
        void DrawList() const;
        /// <summary>
        /// Same as <see cref="DrawList"/> but only draws the
        /// nodes that meet the given <see cref="GeometryState"/>
        /// </summary>
        /// <param name="state">The state.</param>
        void DrawListState(GeometryState state) const;
        /// <summary>
        /// Builds a draw list from all the meshes associated with
        /// this node and its subnodes
        /// </summary>
        void BuildDrawList();
        /// <summary>
        /// Node's inverse transpose matrix
        /// </summary>
        /// <returns></returns>
        const glm::mat4 &InverseTranspose() const;
        /// <summary>
        /// The node's draw list
        /// </summary>
        /// <returns></returns>
        const std::vector<Node *> &DrawListNodes() const;

        Node();
        virtual ~Node();
    private:
        glm::mat4 inverseTransposeModel;
        std::vector<Node *> drawList;
        /// <summary>
        /// Builds the draw list.
        /// </summary>
        /// <param name="base">The base.</param>
        void BuildDrawList(std::vector<Node *> &base);
        /// <summary>
        /// Draws the node's meshes.
        /// </summary>
        void DrawMeshes() const;
        /// <summary>
        /// Updates the node's boundaries and the boundaries
        /// of the node's meshes accordly with the
        /// <see cref="SceneObject.transform"/>
        /// </summary>
        void TransformBoundaries();
};

