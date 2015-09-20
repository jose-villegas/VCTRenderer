#pragma once
struct Attenuation
{
    float constant;
    float linear;
    float quadratic;

    Attenuation() : constant(1.0f), linear(0.2f), quadratic(0.08f) {};
    virtual ~Attenuation() {};
};

class Light
{
    public:
        enum LightType
        {
            Directional,
            Point,
            Spot
        };

        float angleInnerCone;
        float angleOuterCone;
        Attenuation attenuation;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 direction;
        glm::vec3 position;
        LightType lightType;

        Light();
        virtual ~Light();
};

