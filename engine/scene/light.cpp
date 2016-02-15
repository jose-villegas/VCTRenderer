#include "light.h"

#include <glm/detail/type_vec3.hpp>
#include <glm/detail/func_common.hpp>

float Light::AngleInnerCone() const
{
    return angleInnerCone;
}

void Light::AngleInnerCone(float val)
{
    angleInnerCone = glm::clamp(val, 1.0f, angleOuterCone);
}

float Light::AngleOuterCone() const
{
    return angleOuterCone;
}

void Light::AngleOuterCone(float val)
{
    angleOuterCone = glm::clamp(val, 1.0f, 179.0f);;
}

const glm::vec3 &Light::Ambient() const
{
    return ambient;
}

void Light::Ambient(const glm::vec3 &val)
{
    ambient = max(val, glm::vec3(0.0f));
}

const glm::vec3 &Light::Diffuse() const
{
    return diffuse;
}

void Light::Diffuse(const glm::vec3 &val)
{
    diffuse = max(val, glm::vec3(0.0f));;
}

const glm::vec3 &Light::Specular() const
{
    return specular;
}

void Light::Specular(const glm::vec3 &val)
{
    specular = max(val, glm::vec3(0.0f));
}

glm::vec3 Light::Position() const
{
    return position;
}

void Light::Position(glm::vec3 val)
{
    position = val;
}

glm::vec3 Light::Direction() const
{
    return direction;
}

void Light::Direction(glm::vec3 val)
{
    direction = val;
}

Light::LightType Light::Type() const
{
    return lightType;
}

void Light::Type(LightType val)
{
    lightType = val;
}

Light::Light() : lightType(Directional)
{
    angleInnerCone = 30.0f;
    angleOuterCone = 30.0f;
    ambient = diffuse = specular = glm::vec3(1.0f);
    direction = glm::vec3(0.25f, 0.7f, 0.3f);
    position = glm::vec3(0.0f, 1.0f, 0.0f);
}

Light::~Light()
{
}
