#include "stdafx.h"
#include "material.h"
#include "core\deferred_handler.h"
#include "core\Renderer.h"
#include "core\engine_base.h"

glm::vec3 OGLMaterial::White = glm::vec3(1.0f);;
glm::vec3 OGLMaterial::Black = glm::vec3(1.0f);

bool OGLMaterial::HasTexture(RawTexture::TextureType texType) const
{
    return textures[texType] != nullptr;
}

void OGLMaterial::AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                             RawTexture::TextureType texType)
{
    textures[texType] = spTexture;
}

bool OGLMaterial::BindTexture(RawTexture::TextureType texType) const
{
    if (texType > 0  &&
        static_cast<size_t>(texType) < textures.size() &&
        textures[texType] != nullptr)
    {
        textures[texType]->Bind();
        return true;
    }

    return false;
}

void OGLMaterial::SetMaterialUniforms()
{
    static OGLMaterial * activeMaterial = nullptr;
    static Renderer * renderer = &EngineBase::Instance()->GetRenderer();

    // no need to reset uniforms if material is already set
    if (activeMaterial == this) { return; }
    else { activeMaterial = this; }

    // set material properties and texture uniforms
    renderer->SetMaterialUniforms(*this);
}

OGLMaterial::OGLMaterial() : name("Default Material")
{
    diffuse = specular = ambient = emissive = transparent = Black;
    refractionIndex = 1.5f; shininess = 0.0f; opacity = shininessStrenght = 1.0f;
}

OGLMaterial::~OGLMaterial()
{
}