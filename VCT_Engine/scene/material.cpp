#include "stdafx.h"
#include "material.h"
#include "core\deferred_handler.h"


void Material::AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                          RawTexture::TextureType texType)
{
    textures.push_back(
        std::move(
            std::pair<std::shared_ptr<OGLTexture2D>, RawTexture::TextureType>
            (spTexture, texType))
    );
}

void Material::SetTextureUniforms(const oglplus::Program &program)
{
    static Material * activeMaterial = nullptr;

    // no need to reset uniforms if material is already set
    if(activeMaterial == this) { return; }
    else { activeMaterial = this; }

    for(auto it = this->textures.begin(); it != this->textures.end(); ++it)
    {
        if(!DeferredHandler::IsSamplerAvailable(it->second)) continue;

        oglplus::Texture::Active(it->second);
        (*it).first->Bind();
        // set active uniform id
        DeferredHandler::SetSamplerUniform(program, it->second);
    }
}

Material::Material() : name("Default Material")
{
}


Material::~Material()
{
}
