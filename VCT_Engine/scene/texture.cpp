#include "stdafx.h"
#include "texture.h"

void RawTexture::FreeRawData()
{
    unsigned char * pRawData = this->rawData.release();
    delete pRawData;
}

RawTexture::RawTexture() : rawData(nullptr)
{
}


RawTexture::~RawTexture()
{
    this->textureTypes.clear();
}

GLuint OGLTexture2D::UploadToGPU(MinFilter
                                 minFilter /*= MinFilter::LinearMipmapLinear*/,
                                 MagFilter magFilter /*= MagFilter::Linear*/,
                                 WrapMode wrapS /*= WrapMode::Repeat*/, WrapMode wrapT /*= WrapMode::Repeat*/,
                                 bool unloadFromRAM /*= true*/, bool generateMipmaps /*= true*/,
                                 glm::vec4 borderColor /*= glm::vec4(0.f)*/)
{
    using namespace oglplus;
    Context gl;
    PixelDataFormat pdf = PixelDataFormat::BGRA;
    PixelDataInternalFormat pdif = PixelDataInternalFormat::RGBA8;
    unsigned int bytesPerPixel = this->bitsPerPixel / 8;
    // proper data format
    pdf = bytesPerPixel <= 3 ? PixelDataFormat::BGR : pdf;
    pdif = bytesPerPixel <= 3 ? PixelDataInternalFormat::RGB8 : pdif;
    // create texture with raw data (upload to gpu)
    gl.Bound(Texture::Target::_2D, this->oglTexture)
    .Image2D(0, PixelDataInternalFormat::RGB8, this->width, this->height, 0,
             PixelDataFormat::BGR, PixelDataType::UnsignedByte, this->rawData.get());
    // uploaded to gpu
    onGPUMemory = true;

    if(unloadFromRAM)
    {
        this->FreeRawData();
    }

    // gen mipmaps
    if(generateMipmaps)
    {
        gl.Bound(Texture::Target::_2D, this->oglTexture).GenerateMipmap();
        this->mipmapGenerated = true;
    }

    // opengl texture parameters
    gl.Bound(Texture::Target::_2D, this->oglTexture)
    .MinFilter((TextureMinFilter)minFilter)
    .MagFilter((TextureMagFilter)magFilter)
    .WrapS((TextureWrap)wrapS)
    .WrapT((TextureWrap)wrapT);
    // set internals
    this->minFilter = minFilter;
    this->magFilter = magFilter;
    this->wrapS = wrapS;
    this->wrapT = wrapT;

    if(wrapS == WrapMode::ClampToBorder || wrapT == WrapMode::ClampToBorder)
    {
        oglplus::Vector<float, 4> color(borderColor.x, borderColor.y, borderColor.z,
                                        borderColor.w);
        gl.Bound(Texture::Target::_2D, this->oglTexture).BorderColor(color);
        this->borderColor = borderColor;
    }

    // return ogl idenfier on success
    return GetGLName(this->oglTexture);
}

OGLTexture2D::OGLTexture2D()
{
}

OGLTexture2D::~OGLTexture2D()
{
}
