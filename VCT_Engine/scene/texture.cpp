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
        this->rawData.reset(data);
        this->loaded = true;
    }
}

void Texture::FreeRawData()
{
    unsigned char * pRawData = this->rawData.release();
    delete pRawData;
}

Texture::Texture() : loaded(false), rawData(nullptr)
{
}


Texture::~Texture()
{
}
