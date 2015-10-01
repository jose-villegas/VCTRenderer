#include "stdafx.h"
#include "texture.h"

void RawTexture::FreeRawData()
{
    if(!this->rawData) return;

    unsigned char * pRawData = this->rawData.release();
    delete []pRawData;
}

RawTexture::RawTexture() : rawData(nullptr)
{
}


RawTexture::~RawTexture()
{
    FreeRawData();
}

GLuint OGLTexture2D::UploadToGPU(MinFilter
                                 minFilter /*= MinFilter::LinearMipmapLinear*/,
                                 MagFilter magFilter /*= MagFilter::Linear*/,
                                 WrapMode wrapS /*= WrapMode::Repeat*/, WrapMode wrapT /*= WrapMode::Repeat*/,
                                 bool unloadFromRAM /*= true*/, bool generateMipmaps /*= true*/,
                                 glm::vec4 borderColor /*= glm::vec4(0.f)*/)
{
    // already loaded a texture
    if(this->oglTexture) return 0;

    using namespace oglplus;
    Context gl;
    PixelDataFormat pdf = PixelDataFormat::BGRA;
    PixelDataInternalFormat pdif = PixelDataInternalFormat::RGBA8;
    this->oglTexture = std::unique_ptr<Texture>(new oglplus::Texture());
    unsigned int bytesPerPixel = this->bitsPerPixel / 8;
    // proper data format
    pdf = bytesPerPixel <= 3 ? PixelDataFormat::BGR : pdf;
    pdif = bytesPerPixel <= 3 ? PixelDataInternalFormat::RGB8 : pdif;
    // create texture with raw data (upload to gpu)
    gl.Bound(Texture::Target::_2D, *this->oglTexture)
    .Image2D(0, PixelDataInternalFormat::RGB8, this->width, this->height, 0,
             PixelDataFormat::BGR, PixelDataType::UnsignedByte, this->rawData.get());

    if(unloadFromRAM)
    {
        this->FreeRawData();
    }

    // gen mipmaps
    if(generateMipmaps)
    {
        gl.Bound(Texture::Target::_2D, *this->oglTexture).GenerateMipmap();
        this->mipmapGenerated = true;
    }

    // opengl texture parameters
    gl.Bound(Texture::Target::_2D, *this->oglTexture)
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
        gl.Bound(Texture::Target::_2D, *this->oglTexture).BorderColor(color);
        this->borderColor = borderColor;
    }

    this->onGPUMemory = true;
    // return ogl idenfier on success
    return GetGLName(*this->oglTexture);
}

void OGLTexture2D::Bind()
{
    this->oglTexture->Bind(oglplus::Texture::Target::_2D);
}

const std::shared_ptr<OGLTexture2D> & OGLTexture2D::GetDefaultTexture()
{
    static std::shared_ptr<OGLTexture2D> instance = nullptr;

    if(!instance)
    {
        // default texture is white
        OGLTexture2D *defaultTexture = new OGLTexture2D();
        defaultTexture->width = 1;
        defaultTexture->height = 1;
        defaultTexture->lineWidth = 1;
        defaultTexture->depth = 1;
        defaultTexture->bitsPerPixel = 32;
        defaultTexture->rawData.reset(new unsigned char[3] { 255, 255, 255});
        // upload raw data to texture
        defaultTexture->UploadToGPU();

        // texture types conveyed by default
        for(unsigned int i = 0; i < RawTexture::TEXTURE_TYPE_MAX; ++i)
        {
            defaultTexture->textureTypes.insert(
                RawTexture::TextureType(RawTexture::None + i)
            );
        }

        // save to instance
        instance.reset(defaultTexture);
    }

    return instance;
}

const std::shared_ptr<OGLTexture2D> & OGLTexture2D::GetErrorTexture()
{
    static std::shared_ptr<OGLTexture2D> instance = nullptr;

    if(!instance)
    {
        // error texture is purple
        OGLTexture2D *defaultTexture = new OGLTexture2D();
        defaultTexture->width = 1;
        defaultTexture->height = 1;
        defaultTexture->lineWidth = 1;
        defaultTexture->depth = 1;
        defaultTexture->bitsPerPixel = 32;
        defaultTexture->rawData.reset(new unsigned char[3] { 128, 0, 128});
        // upload raw data to texture
        defaultTexture->UploadToGPU();

        // texture types conveyed by default
        for(unsigned int i = 0; i < RawTexture::TEXTURE_TYPE_MAX; ++i)
        {
            defaultTexture->textureTypes.insert(
                RawTexture::TextureType(RawTexture::None + i)
            );
        }

        // save to instance
        instance.reset(defaultTexture);
    }

    return instance;
}

OGLTexture2D::OGLTexture2D()
{
}

OGLTexture2D::~OGLTexture2D()
{
}
