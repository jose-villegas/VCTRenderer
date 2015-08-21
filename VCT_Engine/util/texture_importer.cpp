#include "stdafx.h"
#include "FreeImage.h"
#include "texture_importer.h"

// transforms picture to raw data and stores info in Texture class
bool TextureImporter::ImportTexture(const std::string &sFilepath,
                                    Texture &outTexture)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(sFilepath.c_str());

    //if still unknown, try to guess the file format from the file extension
    if(fif == FIF_UNKNOWN)
    {
        fif = FreeImage_GetFIFFromFilename(sFilepath.c_str());
    }

    //if still unkown, exit
    if(fif == FIF_UNKNOWN)
    {
        return false;
    }

    // pointer to the image once loaded
    FIBITMAP *dib = 0;

    //check that the plugin has reading capabilities and load the file
    if(FreeImage_FIFSupportsReading(fif))
    {
        dib = FreeImage_Load(fif, sFilepath.c_str());
    }

    // get raw data
    unsigned char * bits = FreeImage_GetBits(dib);
    // get image data
    unsigned int width = FreeImage_GetWidth(dib);
    unsigned int height = FreeImage_GetHeight(dib);
    unsigned int lineWidth = FreeImage_GetPitch(dib);
    unsigned int bitsPerPixel = FreeImage_GetBPP(dib);

    // If this somehow one of these failed (they shouldn't), return failure
    if((bitsPerPixel == 0) || (height == 0) || (width == 0))
    {
        FreeImage_Unload(dib);
        return false;
    }

    // copy data before unload
    size_t buffer_size = height * lineWidth;
    unsigned char * data = (unsigned char *)malloc(buffer_size);
    memcpy(data, bits, buffer_size);
    // store data into texture class
    outTexture.filepath = sFilepath;
    outTexture.SaveTextureInfo(height, width, lineWidth, bitsPerPixel, data);
    // unload free image struct handler
    FreeImage_Unload(dib);
    // successful image data extraction
    return true;
}

TextureImporter::TextureImporter()
{
}


TextureImporter::~TextureImporter()
{
}
