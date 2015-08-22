#include "stdafx.h"
#include "texture.h"


void RawTexture::SaveTextureInfo(unsigned int height, unsigned int width,
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

void RawTexture::FreeRawData()
{
    unsigned char * pRawData = this->rawData.release();
    delete pRawData;
}

RawTexture::RawTexture() : loaded(false), rawData(nullptr)
{
    oglTexture2D.baseTextureData = this;
}


RawTexture::~RawTexture()
{
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
    unsigned int bytesPerPixel = baseTextureData->bitsPerPixel / 8;
    // proper data format
    pdf = bytesPerPixel <= 3 ? PixelDataFormat::BGR : pdf;
    pdif = bytesPerPixel <= 3 ? PixelDataInternalFormat::RGB8 : pdif;
    // create texture with raw data (upload to gpu)
    gl.Bound(Texture::Target::_2D, this->oglTexture)
    .Image2D(0, PixelDataInternalFormat::RGB8, baseTextureData->width,
             baseTextureData->height, 0, PixelDataFormat::BGR,
             PixelDataType::UnsignedByte, baseTextureData->rawData.get());

    if(unloadFromRAM)
    {
        baseTextureData->FreeRawData();
    }

    // gen mipmaps
    if(generateMipmaps) { gl.Bound(Texture::Target::_2D, this->oglTexture).GenerateMipmap(); }

    // opengl texture parameters
    gl.Bound(Texture::Target::_2D, this->oglTexture)
    .MinFilter((TextureMinFilter)minFilter)
    .MagFilter((TextureMagFilter)magFilter)
    .WrapS((TextureWrap)wrapS)
    .WrapT((TextureWrap)wrapT);

    if(wrapS == WrapMode::ClampToBorder || wrapT == WrapMode::ClampToBorder)
    {
        oglplus::Vector<float, 4> color(borderColor.x, borderColor.y, borderColor.z,
                                        borderColor.w);
        gl.Bound(Texture::Target::_2D, this->oglTexture).BorderColor(color);
    }

    // return ogl idenfier on success
    return GetGLName(this->oglTexture);
}
