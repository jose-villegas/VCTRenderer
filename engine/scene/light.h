#pragma once
#include <glm/detail/type_vec3.hpp>
#include "../util/raw_format.h"

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

        float angleInnerCone;
        float angleOuterCone;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        glm::vec3 direction;
        glm::vec3 position;

        Attenuation attenuation;

        LightType lightType;

        DataSegment<float> * angleInnerConeSeg;

        void DoSomething();
        Light();
        virtual ~Light();
};

