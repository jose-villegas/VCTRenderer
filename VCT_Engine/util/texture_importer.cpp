#include "stdafx.h"
#include "texture_importer.h"


Texture TextureImporter::ImportTexture(const std::string sFilepath)
{
    Texture newTexture;
    newTexture.filepath = sFilepath;
    return newTexture;
}

TextureImporter::TextureImporter()
{
}


TextureImporter::~TextureImporter()
{
}
