#pragma once
#include "scene\texture.h"

class TextureImporter
{
    public:
        bool ImportTexture(const std::string &sFilepath, RawTexture &outTexture);
        TextureImporter();
        virtual ~TextureImporter();
};

