#include "stdafx.h"
#include "material.h"

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

OGLMaterial::OGLMaterial() : name("Default Material")
{
    diffuse = specular = ambient = emissive = transparent = Black;
    refractionIndex = 1.5f; shininess = 0.0f; opacity = shininessStrenght = 1.0f;
}

OGLMaterial::~OGLMaterial()
{
}