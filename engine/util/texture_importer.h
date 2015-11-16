#pragma once

#include <string>

class RawTexture;

class TextureImporter
{
    public:
        static bool Minimum24Bits;
        static bool ImportTexture2D(const std::string &sFilepath,
                                    RawTexture &outTexture);
};

