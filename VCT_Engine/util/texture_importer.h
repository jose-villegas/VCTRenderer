#pragma once
#include "scene\texture.h"

class TextureImporter
{
    public:
        Texture ImportTexture(const std::string sFilepath);

        TextureImporter();
        virtual ~TextureImporter();
};

