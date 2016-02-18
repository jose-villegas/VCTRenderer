#include "const_definitions.h"

const glm::vec3 Vector3::forward = glm::vec3(0, 0, 1);
const glm::vec3 Vector3::up = glm::vec3(0, 1, 0);
const glm::vec3 Vector3::right = glm::vec3(1, 0, 0);
const glm::vec3 Vector3::zero = glm::vec3(0, 0, 0);
const glm::vec3 Vector3::one = glm::vec3(1, 1, 1);

const glm::mat4x4 Matrix::identity4 = glm::mat4x4(1);
const glm::mat3x3 Matrix::identity3 = glm::mat3x3(1);;
const glm::mat2x2 Matrix::identity2 = glm::mat2x2(1);;
