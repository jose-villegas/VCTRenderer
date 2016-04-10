#include "texture_importer.h"

#include <FreeImagePlus.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../scene/texture.h"

// transforms picture to raw data and stores info in Texture class
bool TextureImporter::ImportTexture2D(const std::string &sFilepath,
                                      RawTexture &outTexture)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(sFilepath.c_str());

    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN)
    {
        fif = FreeImage_GetFIFFromFilename(sFilepath.c_str());
    }

    //if still unkown, exit
    if (fif == FIF_UNKNOWN)
    {
        return false;
    }

    // pointer to the image once loaded
    FIBITMAP * dib = nullptr;

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif))
    {
        dib = FreeImage_Load(fif, sFilepath.c_str());
    }

    // get raw data
    auto bits = FreeImage_GetBits(dib);
    // get image data
    auto width = FreeImage_GetWidth(dib);
    auto height = FreeImage_GetHeight(dib);
    auto lineWidth = FreeImage_GetPitch(dib);
    auto bitsPerPixel = FreeImage_GetBPP(dib);

    // If this somehow one of these failed (they shouldn't), return failure
    if ((bitsPerPixel == 0) || (height == 0) || (width == 0) || !bits)
    {
        FreeImage_Unload(dib);
        return false;
    }

    // copy data before unload
    auto buffer_size = height * lineWidth * sizeof(unsigned char);
    auto data = reinterpret_cast<unsigned char *>(malloc(buffer_size));

    // couldn't allocate memory
    if (data == nullptr)
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
    //// successful image data extraction
    return true;
}
