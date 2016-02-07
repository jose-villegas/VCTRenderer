#pragma once

#include <string>

class RawTexture;

class TextureImporter
{
    public:
        static bool ImportTexture2D(const std::string &sFilepath,
                                    RawTexture &outTexture);
};

