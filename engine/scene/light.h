#pragma once
#include "../util/raw_format.h"
#include "../util/data_segment.h"

#include <glm/detail/type_vec3.hpp>

class Light : public RawFormat
{
    public:
        struct Attenuation
        {
            float constant;
            float linear;
            float quadratic;

            Attenuation() : constant(1.0f), linear(0.2f), quadratic(0.08f) {};
            ~Attenuation() {};
        };
        enum LightType
        {
            Disabled,
            Directional,
            Point,
            Spot
        };

        DataSegment<float> angleInnerCone;
        DataSegment<float> angleOuterCone;

        DataSegment<glm::vec3> ambient;
        DataSegment<glm::vec3> diffuse;
        DataSegment<glm::vec3> specular;

        DataSegment<glm::vec3> position;
        DataSegment<glm::vec3> direction;

        DataSegment<float> constant;
        DataSegment<float> linear;
        DataSegment<float> quadratic;

        LightType lightType;

        Light();
        virtual ~Light();
};

