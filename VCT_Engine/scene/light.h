#pragma once
struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

class Light
{
    private:
        float angleInnerCone;
        float angleOuterCone;
        Attenuation attenuation;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 direction;
        glm::vec3 position;
        unsigned int lightType;
    public:
        Light();
        virtual ~Light();
};

