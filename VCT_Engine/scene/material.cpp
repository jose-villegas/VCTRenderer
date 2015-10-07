#include "stdafx.h"
#include "material.h"
#include "core\deferred_handler.h"
#include "core\Renderer.h"
#include "core\engine_base.h"

glm::vec3 Material::White = glm::vec3(1.0f);;
glm::vec3 Material::Black = glm::vec3(1.0f);

bool Material::HasTexture(RawTexture::TextureType texType) const
{
    return textures[texType] != nullptr;
}

void Material::AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                          RawTexture::TextureType texType)
{
    textures[texType] = spTexture;
}

void Material::SetMaterialUniforms()
{
    static Material *activeMaterial = nullptr;
    static Renderer *renderer = &EngineBase::Instance()->GetRenderer();

    // no need to reset uniforms if material is already set
    if(activeMaterial == this) { return; }
    else { activeMaterial = this; }

    // set material properties uniforms
    renderer->SetMaterialUniforms(*this);

    // bind and active material textures
    for(auto it = renderer->ActiveSamplers().begin();
        it != renderer->ActiveSamplers().end(); ++it)
    {
        oglplus::Texture::Active(*it);

        if(textures[*it]) { textures[*it]->Bind(); }
        else // default (1,1,1) 1hx1w texture
        { OGLTexture2D::GetDefaultTexture()->Bind(); }

        // set active uniform samplers id
        renderer->SetTextureUniform(*it);
    }
}

Material::Material() : name("Default Material")
{
    diffuse = specular = ambient = emissive = transparent = Black;
    refractionIndex = 1.5f; shininess = 0.0f; opacity = shininessStrenght = 1.0f;
}
Material::~Material()
{
}