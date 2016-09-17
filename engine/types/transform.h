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
        /// <summary>
        /// Clearns the transform changed map
        /// </summary>
        static void CleanEventMap();
        /// <summary>
        /// Registers a change in the transform parameters onto
        /// the transform changed map
        /// </summary>
        /// <param name="val">if set to <c>true</c> [value].</param>
        void RegisterChange(bool val = true) const;
        /// <summary>
        /// Determines whether this transform has changed
        /// </summary>
        /// <returns></returns>
        bool TransformChanged() const;
        /// <summary>
        /// Returns a map of bool associated with a transform that
        /// tells if the transform has changed
        /// </summary>
        /// <returns></returns>
        static const std::unordered_map<const Transform *, bool> &TransformChangedMap();
        /// <summary>
        /// Updates the transform matrix.
        /// </summary>
        virtual void UpdateTransformMatrix();
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

        const glm::vec3 &Position() const;
        const glm::quat &Rotation() const;
        const glm::vec3 &Scale() const;
        const glm::vec3 &Forward() const;
        const glm::vec3 &Right() const;
        const glm::vec3 &Up() const;
        const glm::vec3 &Angles() const;
        const glm::mat4x4 &Matrix() const;

        Transform();
        virtual ~Transform();
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
        static std::unordered_map<const Transform *, bool> transformChange;
};