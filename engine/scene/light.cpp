#include "light.h"

#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

void Light::StackFormat()
{
    Stack(DataSegment<float>("angleInnerCone"));
    Stack(DataSegment<float>("angleOuterCone"));
    Stack(DataSegment<float>("ambient"), sizeof(glm::vec3));
    Stack(DataSegment<float>("diffuse"), sizeof(glm::vec3));
    Stack(DataSegment<float>("specular"), sizeof(glm::vec3));
    Stack(DataSegment<float>("direction"), sizeof(glm::vec3));
    Stack(DataSegment<float>("position"), sizeof(glm::vec3));
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
