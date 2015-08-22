#pragma once
class RawTexture;

// holds information to opengl texture
struct OGLTexture2D
{
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

    oglplus::Texture oglTexture;
    RawTexture *baseTextureData;

    bool mipmapGenerated;
    glm::vec4 borderColor;
    unsigned int magFilter;
    unsigned int minFilter;
    unsigned int textureId;
    unsigned int wrapS;
    unsigned int wrapT;

    GLuint UploadToGPU(MinFilter minFilter = MinFilter::LinearMipmapLinear,
                       MagFilter magFilter = MagFilter::Linear, WrapMode wrapS = WrapMode::Repeat,
                       WrapMode wrapT = WrapMode::Repeat, bool unloadFromRAM = true,
                       bool generateMipmaps = true, glm::vec4 borderColor = glm::vec4(0.f));
};

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
        OGLTexture2D oglTexture2D;
        // indicates this texture was loaded from a file
        bool loaded;
        unsigned int textureType;

        std::string filepath;

        void SaveTextureInfo(unsigned int height, unsigned int width,
                             unsigned int pitch, unsigned int bitsPerPixel, unsigned char * data);
        // unloads raw data from RAM memory
        void FreeRawData();
    private:
        unsigned int height;
        unsigned int width;
        // line width in bytes
        unsigned int lineWidth;
        unsigned int bitsPerPixel;

        std::unique_ptr<unsigned char> rawData;
    private:
        // No copying or copy assignment allowed of this class or any derived class
        RawTexture(RawTexture const &);
        RawTexture &operator=(RawTexture const &);

        friend OGLTexture2D;
    public:
        RawTexture();
        ~RawTexture();
};

