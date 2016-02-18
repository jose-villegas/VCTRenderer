#pragma once

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
    public:
        Transform();
        virtual ~Transform();

        void Position(const glm::vec3 &val);
        void Rotation(const glm::quat &val);
        void Rotation(const glm::vec3 &angles);
        void Scale(const glm::vec3 &val);

        const glm::vec3 &Position() const;
        const glm::quat &Rotation() const;
        const glm::vec3 &Scale() const;
        const glm::mat4x4 &ToMatrix();

        bool changed;
    private:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        glm::mat4x4 transformation;
};