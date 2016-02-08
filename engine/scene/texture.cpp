#include "texture.h"

#include <oglplus/bound/texture.hpp>
#include <oglplus/context.hpp>
#include <glm/detail/type_vec3.hpp>

void RawTexture::FreeRawData()
{
    if (!this->rawData) { return; }

    free(this->rawData.release());
}

RawTexture::RawTexture() :
    height(1),
    width(1),
    depth(1),
    lineWidth(1),
    bitsPerPixel(1),
    rawData(nullptr)
{
}


RawTexture::~RawTexture()
{
    FreeRawData();
}

GLuint OGLTexture2D::Load(oglplus::TextureMinFilter minFilter,
                          oglplus::TextureMagFilter magFilter,
                          oglplus::TextureWrap wrapS,
                          oglplus::TextureWrap wrapT,
                          bool generateMipmaps,
                          glm::vec4 borderColor)
{
    static oglplus::Context gl;

    // already loaded a texture
    if (this->oglTexture) { return 0; }

    using namespace oglplus;
    auto pdf = PixelDataFormat::BGRA;
    auto pdif = PixelDataInternalFormat::RGBA8;
    this->oglTexture = std::make_unique<Texture>();
    unsigned int bytesPerPixel = this->bitsPerPixel / 8;
    // proper data format
    pdf = bytesPerPixel == 3 ? PixelDataFormat::BGR : pdf;
    pdif = bytesPerPixel == 3 ? PixelDataInternalFormat::RGB8 : pdif;
    pdf = bytesPerPixel == 2 ? PixelDataFormat::RG : pdf;
    pdif = bytesPerPixel == 2 ? PixelDataInternalFormat::RG8 : pdif;
    pdf = bytesPerPixel == 1 ? PixelDataFormat::Red : pdf;
    pdif = bytesPerPixel == 1 ? PixelDataInternalFormat::R8 : pdif;
    // create texture with raw data (upload to gpu)
    gl.Bound(TextureTarget::_2D, *this->oglTexture)
    .Image2D(0, pdif, this->width, this->height, 0, pdf,
             PixelDataType::UnsignedByte, this->rawData.get());

    if (bytesPerPixel == 1)
    {
        Texture::SwizzleRGBA(TextureTarget::_2D, TextureSwizzle::Red,
                             TextureSwizzle::Red, TextureSwizzle::Red,
                             TextureSwizzle::Red);
    }

    this->FreeRawData();

    // gen mipmaps
    if (generateMipmaps)
    {
        gl.Bound(TextureTarget::_2D, *this->oglTexture).GenerateMipmap();
        this->mipmapGenerated = true;
    }

    // opengl texture parameters
    gl.Bound(TextureTarget::_2D, *this->oglTexture)
    .MinFilter(minFilter)
    .MagFilter(magFilter)
    .WrapS(wrapS)
    .WrapT(wrapT);
    // set internals
    this->minFilter = minFilter;
    this->magFilter = magFilter;
    this->wrapS = wrapS;
    this->wrapT = wrapT;

    if (wrapS == TextureWrap::ClampToBorder || wrapT == TextureWrap::ClampToBorder)
    {
        Vector<float, 4> color(borderColor.x, borderColor.y, borderColor.z,
                               borderColor.w);
        gl.Bound(TextureTarget::_2D, *this->oglTexture).BorderColor(color);
        this->borderColor = borderColor;
    }

    // return ogl idenfier on success
    return GetGLName(*this->oglTexture);
}

void OGLTexture2D::Bind() const
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
    defaultTexture->rawData.reset(new unsigned char[3]
    {
        texColor.b, texColor.g, texColor.r
    });

    // texture types conveyed by default
    for (unsigned int i = 0; i < TYPE_MAX; ++i)
    {
        defaultTexture->textureTypes.insert(TextureType(None + i));
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
        instance->Load();
    }

    return instance;
}

OGLTexture2D::OGLTexture2D()
    : mipmapGenerated(false),
      borderColor(glm::uninitialize),
      magFilter(oglplus::TextureMagFilter::Linear),
      minFilter(oglplus::TextureMinFilter::Linear),
      wrapS(oglplus::TextureWrap::Repeat),
      wrapT(oglplus::TextureWrap::Repeat)
{
}

OGLTexture2D::~OGLTexture2D()
{
}
