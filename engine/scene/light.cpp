#include "light.h"

#include <glm/detail/type_vec3.hpp>
#include <iostream>

void Light::DoSomething()
{
    std::cout << "Hi";
}

Light::Light() : angleInnerCone(0), angleOuterCone(90), lightType(Directional),
    angleInnerConeSeg(SegmentPush<float>("angleInnerCone"))
{
    ambient = diffuse = specular = glm::vec3(1.0f);
    direction = glm::vec3(0.5f, -1.0f, 0.0f);
    position = glm::vec3(0.0f, 1.0f, 0.0f);
}

Light::~Light()
{
}
