#pragma once
#include "texture.h"

class Material
{
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emissive;
        glm::vec3 transparent;

        float opacity;
        float shininess;
        float shininessStrenght;
        float refractionIndex;

        unsigned int materialFlags;
};

class OGLMaterial : public Material
{
    private:
        std::array <std::shared_ptr<OGLTexture2D>,
            RawTexture::TYPE_MAX> textures;
    public:
        enum Float3PropertyId
        {
            Ambient,
            Diffuse,
            Specular,
            Emissive,
            Transparent,
            FLOAT3_PROPERTY_ID_MAX
        };
        enum Float1PropertyId
        {
            Opacity,
            Shininess,
            ShininessStrenght,
            RefractionIndex,
            FLOAT1_PROPERTY_ID_MAX
        };
        enum UInt1PropertyId
        {
            NormalMapping,
            UINT1_PROPERTY_ID_MAX
        };

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
        static glm::vec3 White;
        static glm::vec3 Black;

        std::string name;

        bool HasTexture(RawTexture::TextureType texType) const;
        void AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                        RawTexture::TextureType texType);
        bool BindTexture(RawTexture::TextureType texType) const;

        OGLMaterial();
        virtual ~OGLMaterial();
};

