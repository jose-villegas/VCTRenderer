#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "material.h"

#include <glm/detail/func_common.hpp>

const glm::vec3 &Material::Ambient() const
{
    return ambient;
}

void Material::Ambient(const glm::vec3 &val)
{
    ambient = clamp(val, glm::vec3(0.0f), glm::vec3(1.0f));
}

const glm::vec3 &Material::Diffuse() const
{
    return diffuse;
}

void Material::Diffuse(const glm::vec3 &val)
{
    diffuse = clamp(val, glm::vec3(0.0f), glm::vec3(1.0f));
}

const glm::vec3 &Material::Specular() const
{
    return specular;
}

void Material::Specular(const glm::vec3 &val)
{
    specular = clamp(val, glm::vec3(0.0f), glm::vec3(1.0f));
}

const glm::vec3 &Material::Emissive() const
{
    return emissive;
}

void Material::Emissive(const glm::vec3 &val)
{
    emissive = clamp(val, glm::vec3(0.0f), glm::vec3(1.0f));
}

const glm::vec3 &Material::Transparent() const
{
    return transparent;
}

void Material::Transparent(const glm::vec3 &val)
{
    transparent = clamp(val, glm::vec3(0.0f), glm::vec3(1.0f));
}

float Material::Opacity() const
{
    return opacity;
}

void Material::Opacity(float val)
{
    opacity = glm::clamp(opacity, 0.0f, 1.0f);
}

float Material::Shininess() const
{
    return shininess;
}

void Material::Shininess(float val)
{
    shininess = val;
}

float Material::ShininessStrenght() const
{
    return shininessStrenght;
}

void Material::ShininessStrenght(float val)
{
    shininessStrenght = val;
}

float Material::RefractionIndex() const
{
    return refractionIndex;
}

void Material::RefractionIndex(float val)
{
    refractionIndex = glm::max(0.0f, val);
}

Material::Material() : name("Default Material"), opacity(1), shininess(0),
    shininessStrenght(1), refractionIndex(1)
{
    diffuse = specular = ambient = emissive = transparent = glm::vec3(1.0f);
}

bool OGLMaterial::HasTexture(RawTexture::TextureType texType) const
{
    return textures[texType] != nullptr;
}

void OGLMaterial::AddTexture(const std::shared_ptr<OGLTexture2D> &spTexture,
                             RawTexture::TextureType texType)
{
    textures[texType] = spTexture;

    // will set the material specular to one
    // so multiplying keeps the texture values
    if (texType == RawTexture::Specular) { Specular(glm::vec3(1.0f)); }
}

bool OGLMaterial::BindTexture(RawTexture::TextureType texType,
                              bool bindDefault) const
{
    if (textures[texType] != nullptr)
    {
        textures[texType]->Bind();
        return true;
    }

    if (bindDefault) { OGLTexture2D::GetDefaultTexture()->Bind(); }

    return false;
}

OGLMaterial::OGLMaterial()
{
}

OGLMaterial::~OGLMaterial()
{
}