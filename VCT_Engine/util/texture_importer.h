#pragma once
#include "scene\texture.h"

class TextureImporter
{
    public:
        bool ImportTexture(const std::string &sFilepath, Texture &outTexture);
        TextureImporter();
        virtual ~TextureImporter();
};

