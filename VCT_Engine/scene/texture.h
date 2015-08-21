#pragma once

// holds information to opengl texture
struct OGLTexture
{
    unsigned int textureId;
    bool mipmapGenerated;
    unsigned int minFilter;
    unsigned int magFilter;
    unsigned int wrapS;
    unsigned int wrapT;
};

class Texture
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
        // indicates this texture was loaded from a file
        bool loaded;
        unsigned int textureType;

        std::string filepath;

        void SaveTextureInfo(unsigned int height, unsigned int width,
                             unsigned int pitch, unsigned int bitsPerPixel, unsigned char * data);
    private:
        unsigned int height;
        unsigned int width;
        // line width in bytes
        unsigned int lineWidth;
        unsigned int bitsPerPixel;

        unsigned char * rawData;
    public:
        Texture();
        ~Texture();
};

