#pragma once

#include "texture.h"
#include "../types/base_object.h"

#include <array>
#include <memory>
#include <glm/detail/type_vec3.hpp>

/// <summary>
/// Contains parameters that describe a material properties.
/// Its usually bound to a <see cref="Mesh"/> for rendering.
/// </summary>
/// <seealso cref="BaseObject" />
class Material : public BaseObject
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

        /// <summary>
        /// The material's ambient color component
        /// </summary>
        /// <param name="val">The value.</param>
        void Ambient(const glm::vec3 &val);
        /// <summary>
        /// The material's diffuse color component
        /// </summary>
        /// <param name="val">The value.</param>
        void Diffuse(const glm::vec3 &val);
        /// <summary>
        /// The material's specular color component
        /// </summary>
        /// <param name="val">The value.</param>
        void Specular(const glm::vec3 &val);
        /// <summary>
        /// The material's emissive color component
        /// </summary>
        /// <param name="val">The value.</param>
        void Emissive(const glm::vec3 &val);
        /// <summary>
        /// The material's transparent color component
        /// </summary>
        /// <param name="val">The value.</param>
        void Transparent(const glm::vec3 &val);
        /// <summary>
        /// The material's opacity value
        /// </summary>
        /// <param name="val">The value.</param>
        void Opacity(const float &val);
        /// <summary>
        /// The material's shininess value
        /// </summary>
        /// <param name="val">The value.</param>
        void Shininess(const float &val);
        /// <summary>
        /// The material's shininess strength value
        /// </summary>
        /// <param name="val">The value.</param>
        void ShininessStrenght(const float &val);
        /// <summary>
        /// The material's refractive index value
        /// </summary>
        /// <param name="val">The value.</param>
        void RefractionIndex(const float &val);

        const glm::vec3 &Ambient() const;
        const glm::vec3 &Diffuse() const;
        const glm::vec3 &Specular() const;
        const glm::vec3 &Emissive() const;
        const glm::vec3 &Transparent() const;
        float Opacity() const;
        float Shininess() const;
        float ShininessStrenght() const;
        float RefractionIndex() const;

        /// <summary>
        /// Determines wheter the material has the give texture
        /// </summary>
        /// <param name="texType">The texture type.</param>
        /// <returns>
        ///   <c>true</c> if the material has the texture type,
        ///   otherwise, <c>false</c>.
        /// </returns>
        bool HasTexture(RawTexture::TextureType texType) const;
        /// <summary>
        /// Adds a texture to the material
        /// </summary>
        /// <param name="spTexture">The sp texture.</param>
        /// <param name="texType">Type of the tex.</param>
        void AddTexture(const std::shared_ptr<Texture2D> &spTexture,
                        RawTexture::TextureType texType);
        /// <summary>
        /// Binds the material's texture type
        /// </summary>
        /// <param name="texType">Type of the tex.</param>
        /// <param name="bindDefaul">if set to <c>true</c> binds a
        /// default texture for the texture type.</param>
        /// <returns></returns>
        bool BindTexture(RawTexture::TextureType texType,
                         bool bindDefault = true) const;

        Material();
        ~Material();
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
        std::array<std::shared_ptr<Texture2D>, RawTexture::TYPE_MAX> textures;
};
