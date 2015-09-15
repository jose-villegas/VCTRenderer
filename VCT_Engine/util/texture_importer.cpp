#include "stdafx.h"
#include "FreeImage.h"
#include "texture_importer.h"

// transforms picture to raw data and stores info in Texture class
bool TextureImporter::ImportTexture2D(const std::string &sFilepath,
                                      RawTexture &outTexture)
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
    FIBITMAP *dib = nullptr;

    //check that the plugin has reading capabilities and load the file
    if(FreeImage_FIFSupportsReading(fif))
    {
        dib = FreeImage_Load(fif, sFilepath.c_str());
    }

    if(FreeImage_GetBPP(dib) / 8 < 3)
    {
        dib = FreeImage_ConvertTo24Bits(dib);
    }

    // get raw data
    unsigned char * bits = FreeImage_GetBits(dib);
    // get image data
    unsigned int width = FreeImage_GetWidth(dib);
    unsigned int height = FreeImage_GetHeight(dib);
    unsigned int lineWidth = FreeImage_GetPitch(dib);
    unsigned int bitsPerPixel = FreeImage_GetBPP(dib);

    // If this somehow one of these failed (they shouldn't), return failure
    if((bitsPerPixel == 0) || (height == 0) || (width == 0) || !bits)
    {
        FreeImage_Unload(dib);
        return false;
    }

    // copy data before unload
    size_t buffer_size = height * lineWidth;
    unsigned char * data = new(std::nothrow)unsigned char[buffer_size];

    // couldn't allocate memory
    if(data == nullptr)
    {
        FreeImage_Unload(dib);
        return false;
    }

    std::move(bits, bits + buffer_size, data);
    // store data into texture class
    outTexture.filepath = sFilepath;
    outTexture.height = height;
    outTexture.width = width;
    outTexture.depth = 1;
    outTexture.lineWidth = lineWidth;
    outTexture.bitsPerPixel = bitsPerPixel;
    outTexture.rawData.reset(data);
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
