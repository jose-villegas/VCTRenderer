#pragma once

#include <set>
#include <memory>
#include <GL/glew.h>
#include <oglplus/texture.hpp>
#include <glm/detail/type_vec1.hpp>
#include <glm/detail/type_vec4.hpp>

class RawTexture
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
        std::set<TextureType> textureTypes;
        // unloads raw data from RAM memory
        void FreeRawData();
        std::string GetFilepath() const { return filepath; }

        RawTexture();
        ~RawTexture();
    protected:
        std::string filepath;

        unsigned int height;
        unsigned int width;
        unsigned int depth;
        // line width in bytes
        unsigned int lineWidth;
        unsigned int bitsPerPixel;

        std::unique_ptr<unsigned char> rawData;
    private:
        // No copying or copy assignment allowed of this class or any derived class
        RawTexture(RawTexture const &);
        RawTexture &operator=(RawTexture const &);
    private:
        // friends with
        friend class TextureImporter;
};

// holds information to opengl texture
class OGLTexture2D : public RawTexture
{
    public:
        enum class MinFilter : unsigned int
        {
            Nearest = (unsigned int)oglplus::TextureMinFilter::Nearest,
            Linear = (unsigned int)oglplus::TextureMinFilter::Linear,
            NearestMipmapNearest = (unsigned int)oglplus::TextureMinFilter::NearestMipmapNearest,
            LinearMipmapNearest = (unsigned int)oglplus::TextureMinFilter::LinearMipmapNearest,
            NearestMipmapLinear = (unsigned int)oglplus::TextureMinFilter::NearestMipmapLinear,
            LinearMipmapLinear = (unsigned int)oglplus::TextureMinFilter::LinearMipmapLinear
        };

        enum class MagFilter : unsigned int
        {
            Nearest = (unsigned int)oglplus::TextureMagFilter::Nearest,
            Linear = (unsigned int)oglplus::TextureMagFilter::Linear
        };

        enum class WrapMode : unsigned int
        {
            Repeat = (unsigned int)oglplus::TextureWrap::Repeat,
            MirroredRepeat = (unsigned int)oglplus::TextureWrap::MirroredRepeat,
            ClampToEdge = (unsigned int)oglplus::TextureWrap::ClampToEdge,
            ClampToBorder = (unsigned int)oglplus::TextureWrap::ClampToBorder
        };
    protected:
        std::unique_ptr<oglplus::Texture> oglTexture;

        bool mipmapGenerated;
        glm::vec4 borderColor;
        MagFilter magFilter;
        MinFilter minFilter;
        WrapMode wrapS;
        WrapMode wrapT;

        bool onGPUMemory;

        static OGLTexture2D * CreateColorTexture(std::string texName,
                glm::u8vec3 texColor);
    public:
        OGLTexture2D();
        ~OGLTexture2D();
        GLuint UploadToGPU(MinFilter minFilter = MinFilter::LinearMipmapLinear,
                           MagFilter magFilter = MagFilter::Linear,
                           WrapMode wrapS = WrapMode::Repeat,
                           WrapMode wrapT = WrapMode::Repeat,
                           bool unloadFromRAM = true,
                           bool generateMipmaps = true,
                           glm::vec4 borderColor = glm::vec4(0.f));
        bool OnGPUMemory() const { return onGPUMemory; }
        void Bind();
        int Name() const { return oglplus::GetName(*oglTexture); };

        static std::unique_ptr<OGLTexture2D> &GetDefaultTexture();
        static std::unique_ptr<OGLTexture2D> &GetDefaultNormalTexture();
        static std::unique_ptr<OGLTexture2D> &GetErrorTexture();
    private:
        // No copying or copy assignment allowed of this class or any derived class
        OGLTexture2D(OGLTexture2D const &);
        OGLTexture2D &operator=(OGLTexture2D const &);
};