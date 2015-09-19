#pragma once
#include "texture.h"

class Material
{
    public:
        enum ShadingMode
        {
            Flat,
            Gourad,
            Phong,
            Blinn,
            Toon,
            OrenNayar,
            Minnaert,
            CookTorrance,
            NoShading,
            Fresnel
        };
        enum BlendMode
        {
            Default,
            Additive
        };
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

        unsigned int shadingModel;
        unsigned int blendMode;

        std::array<std::shared_ptr<OGLTexture2D>, RawTexture::TEXTURE_TYPE_MAX>
        textures;

        Material();
        ~Material();
};

class OGLMaterial : public Material
{
    public:
        std::shared_ptr<oglplus::Program> materialShader;

        OGLMaterial();
        ~OGLMaterial();
};

