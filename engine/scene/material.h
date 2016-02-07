#pragma once
#include <glm/detail/type_vec3.hpp>
#include <array>
#include <memory>
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

        Material();
};

class OGLMaterial : public Material
{
    public:
        static glm::vec3 White;
        static glm::vec3 Black;

        bool HasTexture(RawTexture::TextureType texType) const;
        void AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                        RawTexture::TextureType texType);
        bool BindTexture(RawTexture::TextureType texType, bool bindDefaul = true) const;

        OGLMaterial();
        ~OGLMaterial();
    private:
        std::array<std::shared_ptr<OGLTexture2D>,
            RawTexture::TYPE_MAX> textures;
};

