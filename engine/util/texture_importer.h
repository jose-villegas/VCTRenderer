#pragma once
#include "scene\texture.h"

class TextureImporter
{
    public:
        static bool Minimum24Bits;

        bool ImportTexture2D(const std::string &sFilepath, RawTexture &outTexture);
        TextureImporter();
        virtual ~TextureImporter();
};

