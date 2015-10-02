#include "stdafx.h"
#include "material.h"
#include "core\deferred_handler.h"


void Material::AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                          RawTexture::TextureType texType)
{
    textures[texType] = spTexture;
}

void Material::SetMaterialUniforms(oglplus::Program &program)
{
    static Material *activeMaterial = nullptr;
    static const std::vector<RawTexture::TextureType> &programSamplers =
        DeferredHandler::ProgramSamplers();

    // no need to reset uniforms if material is already set
    if(activeMaterial == this) { return; }
    else { activeMaterial = this; }

    // set material values
    oglplus::Uniform<glm::vec3>(program, "material.ambient").Set(this->ambient);
    oglplus::Uniform<glm::vec3>(program, "material.diffuse").Set(this->diffuse);
    oglplus::Uniform<glm::vec3>(program, "material.specular").Set(this->specular);

    // set material textures
    for(auto it = programSamplers.begin(); it != programSamplers.end(); ++it)
    {
        oglplus::Texture::Active(*it);

        if(textures[*it]) { textures[*it]->Bind(); }
        else { OGLTexture2D::GetDefaultTexture()->Bind(); }

        // set active uniform id
        DeferredHandler::SetSamplerUniform(program, *it);
    }
}

Material::Material() : name("Default Material")
{
}
Material::~Material()
{
}