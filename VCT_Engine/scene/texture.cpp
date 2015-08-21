#include "stdafx.h"
#include "texture.h"


void Texture::SaveTextureInfo(unsigned int height, unsigned int width,
                              unsigned int pitch, unsigned int bitsPerPixel, unsigned char * data)
{
    if(!loaded)
    {
        this->height = height;
        this->width = width;
        this->lineWidth = pitch;
        this->bitsPerPixel = bitsPerPixel;
        this->rawData = data;
        this->loaded = true;
    }
}

Texture::Texture() : loaded(false)
{
}


Texture::~Texture()
{
}
