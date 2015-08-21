#pragma once
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

