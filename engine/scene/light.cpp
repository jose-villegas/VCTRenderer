#include "light.h"

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

void Light::BuildFormat()
{
    (*this) << DataSegment<float>(&angleInnerCone)
            << DataSegment<float>(&angleOuterCone)
            << DataSegment<float>(value_ptr(ambient), sizeof(glm::vec3))
            << DataSegment<float>(value_ptr(diffuse), sizeof(glm::vec3))
            << DataSegment<float>(value_ptr(specular), sizeof(glm::vec3))
            << DataSegment<float>(value_ptr(position), sizeof(glm::vec3))
            << DataSegment<float>(value_ptr(direction), sizeof(glm::vec3));
}

Light::Light() : angleInnerCone(0), angleOuterCone(90), lightType(Directional)
{
    ambient = diffuse = specular = glm::vec3(1.0f);
    direction = glm::vec3(0.5f, -1.0f, 0.0f);
    position = glm::vec3(0.0f, 1.0f, 0.0f);
}


Light::~Light()
{
}
