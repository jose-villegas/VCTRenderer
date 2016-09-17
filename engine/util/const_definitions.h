#pragma once

#include <glm/detail/type_vec3.hpp>
#include <glm/matrix.hpp>

/// <summary>
/// Vector3 constants
/// </summary>
class Vector3
{
    public:
        // (0,0,1)
        static const glm::vec3 forward;
        // (1,0,0)
        static const glm::vec3 right;
        // (0,1,0)
        static const glm::vec3 up;
        // (0,0,0)
        static const glm::vec3 zero;
        // (1,1,1)
        static const glm::vec3 one;
};

/// <summary>
/// Matrix constants
/// </summary>
class Matrix
{
    public:
        static const glm::mat4x4 identity4;
        static const glm::mat3x3 identity3;
        static const glm::mat2x2 identity2;
};