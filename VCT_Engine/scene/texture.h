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
            Unknow
        };
    private:
        unsigned int height;
        unsigned int width;
        unsigned int textureType;
    public:
        Texture();
        ~Texture();
};

