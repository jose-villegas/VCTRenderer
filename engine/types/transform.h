#pragma once

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "base_object.h"
#include <unordered_map>

/// <summary>
/// Handles all transformation operations such
/// as scaling, rotating and translating
/// </summary>
class Transform : public BaseObject
{
    public:
        Transform();
        virtual ~Transform();
        /// <summary>
        /// Sets the transform position
        /// </summary>
        /// <param name="val">The value.</param>
        void Position(const glm::vec3 &val);
        /// <summary>
        /// Sets the transform rotation.
        /// </summary>
        /// <param name="val">The value.</param>
        void Rotation(const glm::quat &val);
        /// <summary>
        /// Sets the transform rotation using euler angles
        /// </summary>
        /// <param name="angles">The angles.</param>
        void Rotation(const glm::vec3 &angles);
        /// <summary>
        /// Sets the transform scale
        /// </summary>
        /// <param name="val">The value.</param>
        void Scale(const glm::vec3 &val);
        const glm::vec3 &Position() const;
        const glm::quat &Rotation() const;
        const glm::vec3 &Scale() const;
        /// <summary>
        /// Sets the forward direction vector. Warning this does not update the
        /// rest of the direction vectors.
        /// </summary>
        /// <param name="val">The value.</param>
        void Forward(const glm::vec3 &val);
        /// <summary>
        /// Sets the right direction vector. Warning this does not update the
        /// rest of the direction vectors.
        /// </summary>
        /// <param name="val">The value.</param>
        void Right(const glm::vec3 &val);
        /// <summary>
        /// Sets the up direction vector. Warning this does not update the
        /// rest of the direction vectors.
        /// </summary>
        /// <param name="val">The value.</param>
        void Up(const glm::vec3 &val);
        const glm::vec3 &Forward() const;
        const glm::vec3 &Right() const;
        const glm::vec3 &Up() const;
        const glm::vec3 &Angles() const;
        /// <summary>
        /// Returns the transformation matrix result of this transform
        /// components. If changed is true Matrix will keep calculating
        /// the transform matrix, otherwise it will just return a reference to
        /// <see cref="transformation" />
        /// </summary>
        /// <returns></returns>
        const glm::mat4x4 &Matrix() const;
        virtual void UpdateTransformMatrix();


        static bool TransformChanged(const Transform &transform);
        static const std::unordered_map<const Transform *, bool> &TransformChangedMap();
        static void CleanEventMap();
    private:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 angles;
        glm::mat4x4 transformation;
        void UpdateCoordinates();
        // contains a unique set of transforms, on change bool is set to true
        static std::unordered_map<const Transform *, bool> transformChange;
};