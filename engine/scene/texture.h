#pragma once

#include <set>
#include "../types/base_object.h"

#include <memory>
#include <glm/detail/type_vec1.hpp>
#include <glm/detail/type_vec4.hpp>
#include <oglplus/texture.hpp>

class RawTexture : public BaseObject
{
    public:
        enum TextureType
        {
            None,
            Diffuse,
            Specular,
            Ambient,
            Emissive,
            Height,
            Normals,
            Shininess,
            Opacity,
            Displacement,
            Lightmap,
            Reflection,
            Unknow,
            TYPE_MAX
        };
        /// <summary>
        /// Determines the texture is of the given type.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <returns>
        ///   <c>true</c> if the specified type is type; otherwise, <c>false</c>.
        /// </returns>
        bool IsType(TextureType type);
        /// <summary>
        /// Gets the texture filepath.
        /// </summary>
        /// <returns></returns>
        std::string GetFilepath() const;

        std::set<TextureType> textureTypes;

        RawTexture();
        ~RawTexture();
        RawTexture(RawTexture const &) = delete;
        RawTexture &operator=(RawTexture const &) = delete;
    protected:
        std::string filepath;
        unsigned int height;
        unsigned int width;
        unsigned int depth;
        unsigned int lineWidth;
        unsigned int bitsPerPixel;
        std::unique_ptr<unsigned char> rawData;
    private:
        friend class TextureImporter;
};

class Texture2D : public RawTexture
{
    public:
        /// <summary>
        /// Loads the associated texture file with the given parameters.
        /// </summary>
        /// <param name="minFilter">The minimum filter.</param>
        /// <param name="magFilter">The mag filter.</param>
        /// <param name="wrapS">The wrap s.</param>
        /// <param name="wrapT">The wrap t.</param>
        /// <param name="generateMipmaps">if set to <c>true</c> [generate mipmaps].</param>
        /// <param name="borderColor">Color of the border.</param>
        void Load(oglplus::TextureMinFilter minFilter,
                  oglplus::TextureMagFilter magFilter,
                  oglplus::TextureWrap wrapS,
                  oglplus::TextureWrap wrapT,
                  bool generateMipmaps = true,
                  glm::vec4 borderColor = glm::vec4(0.f));
        /// <summary>
        /// Binds the texture.
        /// </summary>
        void Bind() const;
        /// <summary>
        /// The texture name
        /// </summary>
        /// <returns></returns>
        int Name() const;
        /// <summary>
        /// Gets the a default texture white 1x1 texture.
        /// </summary>
        /// <returns></returns>
        static std::unique_ptr<Texture2D> &GetDefaultTexture();

        Texture2D();
        ~Texture2D();
    protected:
        std::unique_ptr<oglplus::Texture> oglTexture;
        /// <summary>
        /// Creates a 1x1 texture with the given color.
        /// </summary>
        /// <param name="texName">Name of the texture.</param>
        /// <param name="texColor">Color of the texture.</param>
        /// <returns></returns>
        static Texture2D * CreateColorTexture(std::string texName,
                                              glm::u8vec4 texColor);
    private:
        Texture2D(Texture2D const &) = delete;
        Texture2D &operator=(Texture2D const &) = delete;
};