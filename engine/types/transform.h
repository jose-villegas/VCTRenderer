#pragma once

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/quaternion.hpp>

/// <summary>
/// Handles all transformation operations such
/// as scaling, rotating and translating
/// </summary>
class Transform
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

        void Forward(const glm::vec3 &val);
        void Right(const glm::vec3 &val);
        void Up(const glm::vec3 &val);

        const glm::vec3 &Forward() const;
        const glm::vec3 &Right() const;
        const glm::vec3 &Up() const;

        const glm::vec3 &Angles() const;
        /// <summary>
        /// Returns the transformation matrix product of this transform
        /// </summary>
        /// <returns></returns>
        const glm::mat4x4 &ToMatrix();

        /// <summary>
        /// Makes the transform forward point at the specified position
        /// </summary>
        /// <param name="pos">The position.</param>
        /// <param name="up">Up.</param>
        void LookAt(const glm::vec3 &pos, const glm::vec3 &up);
        /// <summary>
        /// Indicates if the current transform has been in any way modifed
        /// the value has to be reset externally by any other component using
        /// the transform class. If changed is true ToMatrix will compute the
        /// the transformation matrix everytime its called.
        /// </summary>
        bool changed;
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
};