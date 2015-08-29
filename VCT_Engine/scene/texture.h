#pragma once

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
            TEXTURE_TYPE_MAX
        };
    public:
        std::set<TextureType> textureTypes;
        // unloads raw data from RAM memory
        void FreeRawData();

        std::string GetFilepath() const { return filepath; }
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
    public:
        RawTexture();
        ~RawTexture();
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
    public:
        OGLTexture2D();
        virtual ~OGLTexture2D();
        GLuint UploadToGPU(MinFilter minFilter = MinFilter::LinearMipmapLinear,
                           MagFilter magFilter = MagFilter::Linear, WrapMode wrapS = WrapMode::Repeat,
                           WrapMode wrapT = WrapMode::Repeat, bool unloadFromRAM = true,
                           bool generateMipmaps = true, glm::vec4 borderColor = glm::vec4(0.f));
    private:
        // No copying or copy assignment allowed of this class or any derived class
        OGLTexture2D(OGLTexture2D const &);
        OGLTexture2D &operator=(OGLTexture2D const &);
};

