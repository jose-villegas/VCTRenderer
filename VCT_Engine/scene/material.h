#pragma once
#include "texture.h"
class Material
{
    public:
        std::string name;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emissive;
        glm::vec3 transparent;

        float opacity;
        float shininess;
        float shininessStrenght;
        float refractionIndex;

        int shadingModel;
        int blendMode;

        Material();
        ~Material();
    private:
        std::vector<Texture *> textures;
};

