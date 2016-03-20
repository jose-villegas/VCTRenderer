#include "light.h"

#include <glm/detail/type_vec3.hpp>
#include <glm/detail/func_common.hpp>
#include "../util/const_definitions.h"
#include "camera.h"

const unsigned int Light::DirectionalsLimit = 8;
const unsigned int Light::PointsLimit = 256;
const unsigned int Light::SpotsLimit = 256;

std::vector<Light *> Light::directionals;
std::vector<Light *> Light::points;
std::vector<Light *> Light::spots;


float Light::AngleInnerCone() const
{
    return angleInnerCone;
}

void Light::AngleInnerCone(float val)
{
    angleInnerCone = glm::clamp(val, 0.0f, glm::pi<float>());
}

float Light::AngleOuterCone() const
{
    return angleOuterCone;
}

void Light::AngleOuterCone(float val)
{
    angleOuterCone = glm::clamp(val, 0.0f, glm::pi<float>());;
    angleInnerCone = glm::min(angleInnerCone, angleOuterCone);
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

const glm::vec3 &Light::Intensity() const
{
    return intensity;
}

const glm::vec3 &Light::Direction() const
{
    return transform.Forward();
}

const glm::vec3 &Light::Position() const
{
    return transform.Position();
}

glm::vec3 Light::Direction(const glm::mat4x4 &mat) const
{
    return glm::vec3(mat * glm::vec4(Direction(), 0.0f));
}

glm::vec3 Light::Position(const glm::mat4x4 &mat) const
{
    return glm::vec3(mat * glm::vec4(Position(), 1.0f));
}

void Light::Specular(const glm::vec3 &val)
{
    specular = max(val, glm::vec3(0.0f));
}

void Light::Intensity(const glm::vec3 &val)
{
    intensity = max(val, glm::vec3(0.0f));
}

Light::LightType Light::Type() const
{
    return lightType;
}


void Light::TypeCollection(LightType val, bool force)
{
    // will be added without check for previous addition
    if (force)
    {
        collectionIndex = -1;
    }

    // no change
    if (val == lightType && collectionIndex >= 0) { return; }

    // this light is stored in another collection
    if (collectionIndex >= 0)
    {
        switch (lightType)
        {
            case Directional:
                if (collectionIndex >= directionals.size()) { break; }

                directionals.erase(directionals.begin() + collectionIndex);
                break;

            case Point:
                if (collectionIndex >= points.size()) { break; }

                points.erase(points.begin() + collectionIndex);
                break;

            case Spot:
                if (collectionIndex >= spots.size()) { break; }

                spots.erase(spots.begin() + collectionIndex);
                break;

            default: break;
        }
    }

    // we are changing our type
    switch (val)
    {
        case Directional:
            if (directionals.size() == DirectionalsLimit) { return; }

            collectionIndex = static_cast<int>(directionals.size());
            directionals.push_back(this);
            break;

        case Point:
            if (points.size() == PointsLimit) { return; }

            collectionIndex = static_cast<int>(points.size());
            points.push_back(this);
            break;

        case Spot:
            if (spots.size() == SpotsLimit) { return; }

            collectionIndex = static_cast<int>(spots.size());
            spots.push_back(this);
            break;

        default: break;
    }

    lightType = val;
}

Light::Light() : lightType(Directional)
{
    name = "Default Light";
    angleInnerCone = glm::radians(25.0f);
    angleOuterCone = glm::radians(30.0f);
    ambient = Vector3::zero;
    diffuse = specular = intensity =  Vector3::one;
    transform.Rotation(radians(glm::vec3(130.0f, -30.0f, 0.0f)));
    // indicates this light hasn't been added to any collection
    collectionIndex = -1;
    // add to type collection
    TypeCollection(lightType);
}

Light::~Light()
{
}

void Light::CleanCollections()
{
    directionals.clear();
    points.clear();
    spots.clear();
}

const std::vector<Light *> &Light::Directionals()
{
    return directionals;
}

const std::vector<Light *> &Light::Points()
{
    return points;
}

const std::vector<Light *> &Light::Spots()
{
    return spots;
}
