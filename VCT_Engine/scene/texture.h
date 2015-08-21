#pragma once
//class Texture;

// holds information to opengl texture
struct OGLTexture
{
    bool mipmapGenerated;
    glm::vec4 borderColor;
    unsigned int magFilter;
    unsigned int minFilter;
    unsigned int textureId;
    unsigned int wrapS;
    unsigned int wrapT;

    // GLuint UploadToGPU(Texture &ramTexture);
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
        Texture(Texture const &);
        Texture &operator=(Texture const &);
    public:
        Texture();
        ~Texture();
};

