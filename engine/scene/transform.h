#pragma once

#include "../types/base_object.h"

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform : public BaseObject
{
    public:
        Transform();
        virtual ~Transform();
        void Position(const glm::vec3 &val);
        void Position(float x, float y, float z);
        void Rotation(const glm::quat &val);
        void Rotation(float w, float x, float y, float z);
        void Scale(const glm::vec3 &val);
        void Scale(float x, float y, float z);

        const glm::vec3 &Position() const;
        const glm::quat &Rotation() const;
        const glm::vec3 &Scale() const;
        const glm::mat4x4 &ToMatrix() const;
    private:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::mat4x4 transformation;
        void ComputeMatrix();
};