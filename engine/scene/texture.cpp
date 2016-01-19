#include "texture.h"

#include <oglplus/bound/texture.hpp>
#include <oglplus/context.hpp>
#include <glm/detail/type_vec3.hpp>

void RawTexture::FreeRawData()
{
    if (!this->rawData) { return; }

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

GLuint OGLTexture2D::UploadToGPU(MinFilter minFilter, MagFilter magFilter,
                                 WrapMode wrapS , WrapMode wrapT ,
                                 bool unloadFromRAM , bool generateMipmaps,
                                 glm::vec4 borderColor)
{
    static oglplus::Context gl;

    // already loaded a texture
    if (this->oglTexture) { return 0; }

    using namespace oglplus;
    PixelDataFormat pdf = PixelDataFormat::BGRA;
    PixelDataInternalFormat pdif = PixelDataInternalFormat::RGBA8;
    this->oglTexture = std::unique_ptr<Texture>(new oglplus::Texture());
    unsigned int bytesPerPixel = this->bitsPerPixel / 8;
    // proper data format
    pdf = bytesPerPixel == 3 ? PixelDataFormat::BGR : pdf;
    pdif = bytesPerPixel == 3 ? PixelDataInternalFormat::RGB8 : pdif;
    pdf = bytesPerPixel == 2 ? PixelDataFormat::RG : pdf;
    pdif = bytesPerPixel == 2 ? PixelDataInternalFormat::RG8 : pdif;
    pdf = bytesPerPixel == 1 ? PixelDataFormat::Red : pdf;
    pdif = bytesPerPixel == 1 ? PixelDataInternalFormat::R8 : pdif;
    // create texture with raw data (upload to gpu)
    gl.Bound(Texture::Target::_2D, *this->oglTexture)
    .Image2D(0, pdif, this->width, this->height, 0, pdf,
             PixelDataType::UnsignedByte, this->rawData.get());

    if (unloadFromRAM)
    {
        this->FreeRawData();
    }

    // gen mipmaps
    if (generateMipmaps)
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

    if (wrapS == WrapMode::ClampToBorder || wrapT == WrapMode::ClampToBorder)
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

OGLTexture2D * OGLTexture2D::CreateColorTexture(std::string texName,
        glm::u8vec3 texColor)
{
    OGLTexture2D * defaultTexture = new OGLTexture2D();
    defaultTexture->filepath = texName;
    defaultTexture->width = 1;
    defaultTexture->height = 1;
    defaultTexture->lineWidth = 1;
    defaultTexture->depth = 1;
    defaultTexture->bitsPerPixel = 24;
    defaultTexture->rawData.reset(new unsigned char[3] {texColor.b, texColor.g, texColor.r});

    // texture types conveyed by default
    for (unsigned int i = 0; i < RawTexture::TYPE_MAX; ++i)
    {
        defaultTexture->textureTypes.insert(
            RawTexture::TextureType(RawTexture::None + i)
        );
    }

    return defaultTexture;
}

std::unique_ptr<OGLTexture2D> &OGLTexture2D::GetDefaultTexture()
{
    static std::unique_ptr<OGLTexture2D> instance = nullptr;

    if (!instance)
    {
        // default texture is white
        std::string texName = "!defaultTexture";
        glm::u8vec3 texColor = glm::u8vec3(255, 255, 255);
        // save to instance
        instance.reset(CreateColorTexture(texName, texColor));
        instance->UploadToGPU();
    }

    return instance;
}

std::unique_ptr<OGLTexture2D> &OGLTexture2D::GetDefaultNormalTexture()
{
    static std::unique_ptr<OGLTexture2D> instance = nullptr;

    if (!instance)
    {
        // default texture is white
        std::string texName = "!defaultNormalTexture";
        glm::u8vec3 texColor = glm::u8vec3(128, 128, 255);
        // save to instance
        instance.reset(CreateColorTexture(texName, texColor));
        instance->UploadToGPU();
    }

    return instance;
}

std::unique_ptr<OGLTexture2D> &OGLTexture2D::GetErrorTexture()
{
    static std::unique_ptr<OGLTexture2D> instance = nullptr;

    if (!instance)
    {
        // default texture is white
        std::string texName = "!errorTexture";
        glm::u8vec3 texColor = glm::u8vec3(128, 0, 128);
        // save to instance
        instance.reset(CreateColorTexture(texName, texColor));
        instance->UploadToGPU();
    }

    return instance;
}

OGLTexture2D::OGLTexture2D()
    : mipmapGenerated(false),
      borderColor(glm::uninitialize),
      magFilter(MagFilter::Linear),
      minFilter(MinFilter::Linear),
      wrapS(WrapMode::Repeat),
      wrapT(WrapMode::Repeat),
      onGPUMemory(false)
{
}

OGLTexture2D::~OGLTexture2D()
{
}
