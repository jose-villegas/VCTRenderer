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

        const glm::vec3 &Ambient() const;
        void Ambient(const glm::vec3 &val);
        const glm::vec3 &Diffuse() const;
        void Diffuse(const glm::vec3 &val);
        const glm::vec3 &Specular() const;
        void Specular(const glm::vec3 &val);
        const glm::vec3 &Emissive() const;
        void Emissive(const glm::vec3 &val);
        const glm::vec3 &Transparent() const;
        void Transparent(const glm::vec3 &val);

        float Opacity() const;
        void Opacity(float val);
        float Shininess() const;
        void Shininess(float val);
        float ShininessStrenght() const;
        void ShininessStrenght(float val);
        float RefractionIndex() const;
        void RefractionIndex(float val);
        Material();
    private:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emissive;
        glm::vec3 transparent;
        float opacity;
        float shininess;
        float shininessStrenght;
        float refractionIndex;
};

class OGLMaterial : public Material
{
    public:
        bool HasTexture(RawTexture::TextureType texType) const;
        void AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                        RawTexture::TextureType texType);
        bool BindTexture(RawTexture::TextureType texType, bool bindDefaul = true) const;

        OGLMaterial();
        ~OGLMaterial();
    private:
        std::array<std::shared_ptr<OGLTexture2D>, RawTexture::TYPE_MAX> textures;
};

