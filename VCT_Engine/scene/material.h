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
            RawTexture::TEXTURE_TYPE_MAX> textures;
    public:
        enum MaterialFloat3PropertyId
        {
            Ambient,
            Diffuse,
            Specular,
            Emissive,
            Transparent,
            MATERIAL_FLOAT3_PROPERTY_ID_MAX
        };
        enum MaterialFloat1PropertyId
        {
            Opacity,
            Shininess,
            ShininessStrenght,
            RefractionIndex,
            MATERIAL_FLOAT1_PROPERTY_ID_MAX
        };
        enum MaterialUInt1PropertyId
        {
            NormalMapping,
            MATERIAL_UINT1_PROPERTY_ID_MAX
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
        void SetMaterialUniforms();

        OGLMaterial();
        virtual ~OGLMaterial();
};

