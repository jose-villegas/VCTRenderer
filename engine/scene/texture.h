#pragma once

#include <set>
#include <memory>
#include <glm/detail/type_vec1.hpp>
#include <glm/detail/type_vec4.hpp>
#include <oglplus/texture.hpp>

class RawTexture
{
    protected:
        std::string filepath;

        unsigned int height;
        unsigned int width;
        unsigned int depth;
        // line width in bytes
        unsigned int lineWidth;
        unsigned int bitsPerPixel;

        std::unique_ptr<unsigned char> rawData;
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
        std::set<TextureType> textureTypes;
        // unloads raw data from RAM memory
        void FreeRawData();
        std::string GetFilepath() const { return filepath; }

        RawTexture();
        ~RawTexture();
        // No copying or copy assignment allowed of this class or any derived class
        RawTexture(RawTexture const &) = delete;
        RawTexture &operator=(RawTexture const &) = delete;
    private:
        // friends with
        friend class TextureImporter;
};

// holds information to opengl texture
class OGLTexture2D : public RawTexture
{
    protected:
        std::unique_ptr<oglplus::Texture> oglTexture;

        static OGLTexture2D * CreateColorTexture(std::string texName,
                glm::u8vec3 texColor);
    public:
        OGLTexture2D();
        ~OGLTexture2D();
        void Load(oglplus::TextureMinFilter minFilter,
                  oglplus::TextureMagFilter magFilter,
                  oglplus::TextureWrap wrapS,
                  oglplus::TextureWrap wrapT,
                  bool generateMipmaps = true,
                  glm::vec4 borderColor = glm::vec4(0.f));
        void Bind() const;
        int Name() const;

        static std::unique_ptr<OGLTexture2D> &GetDefaultTexture();
    private:
        // No copying or copy assignment allowed of this class or any derived class
        OGLTexture2D(OGLTexture2D const &) = delete;
        OGLTexture2D &operator=(OGLTexture2D const &) = delete;
};