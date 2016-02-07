#include "light.h"

#include <glm/detail/type_vec3.hpp>

Light::Light() : lightType(Directional)
{
    angleInnerCone = 0.0f;
    angleOuterCone = 90.0f;
    ambient = diffuse = specular = glm::vec3(1.0f);
    direction = glm::vec3(0.0f, -0.7f, 0.3f);
    position = glm::vec3(0.0f, 1.0f, 0.0f);
    constant = 1.0f;
    linear = 0.2f;
    quadratic = 0.08f;
}

Light::~Light()
{
}
