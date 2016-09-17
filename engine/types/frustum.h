#pragma once
#include <array>

#include <glm\detail\type_mat4x4.hpp>
#include <glm\detail\type_vec4.hpp>

class BoundingBox;
class BoundingSphere;
class BoundingBox;

/// <summary>
/// A frustum formed by six planes
/// </summary>
class Frustum
{
    public:
        enum FrustumPlane
        {
            Left, Right,
            Bottom, Top,
            Near, Far,
        };
        /// <summary>
        /// Extracts the frustum planes from the model view matrix
        /// </summary>
        /// <param name="mvMatrix">The model view matrix.</param>
        /// <param name="normalize">if set to <c>true</c> [normalize].</param>
        void ExtractPlanes(const glm::mat4x4 &modelView, bool normalize = true);
        /// <summary>
        /// Returns the plane from the give frustum face
        /// </summary>
        /// <param name="face">The face.</param>
        /// <returns></returns>
        const glm::vec4 &Plane(const FrustumPlane face) const;
        /// <summary>
        /// Returns the frustum planes
        /// </summary>
        /// <returns></returns>
        const std::array<glm::vec4, 6> &Planes() const;
    protected:
        std::array<glm::vec4, 6> planes;
};

/// <summary>
/// Contains methods for frustum culling
/// with its <see cref="Frustum" /> parameters.
/// </summary>
/// <seealso cref="Frustum" />
class CullingFrustum : public Frustum
{
    public:
        /// <summary>
        /// Determines if the given volume is inside the frustum
        /// </summary>
        /// <param name="volume">The volume.</param>
        /// <returns></returns>
        bool InFrustum(const BoundingBox &volume) const;

        CullingFrustum();
        virtual ~CullingFrustum();
};
