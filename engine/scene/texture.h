#pragma once

#include <set>
#include <memory>
#include <GL/glew.h>
#include <oglplus/texture.hpp>
#include <glm/detail/type_vec1.hpp>
#include <glm/detail/type_vec4.hpp>

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

        bool mipmapGenerated;
        glm::vec4 borderColor;
        oglplus::TextureMagFilter magFilter;
        oglplus::TextureMinFilter minFilter;
        oglplus::TextureWrap wrapS;
        oglplus::TextureWrap wrapT;

        static OGLTexture2D * CreateColorTexture(std::string texName,
                glm::u8vec3 texColor);
    public:
        OGLTexture2D();
        ~OGLTexture2D();
        GLuint Load(oglplus::TextureMinFilter minFilter =
                        oglplus::TextureMinFilter::LinearMipmapLinear,
                    oglplus::TextureMagFilter magFilter =
                        oglplus::TextureMagFilter::Linear,
                    oglplus::TextureWrap wrapS =
                        oglplus::TextureWrap::Repeat,
                    oglplus::TextureWrap wrapT =
                        oglplus::TextureWrap::Repeat,
                    bool generateMipmaps = true,
                    glm::vec4 borderColor = glm::vec4(0.f));
        void Bind() const;
        int Name() const { return oglplus::GetName(*oglTexture); };

        static std::unique_ptr<OGLTexture2D> &GetDefaultTexture();
    private:
        // No copying or copy assignment allowed of this class or any derived class
        OGLTexture2D(OGLTexture2D const &);
        OGLTexture2D &operator=(OGLTexture2D const &);
};