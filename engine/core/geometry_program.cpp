#include "geometry_program.h"

GeometryProgram::GeometryProgram()
{
    samplers.Resize(RawTexture::TYPE_MAX);
    transformMatrices.Resize(TransformMatrices::MATRIX_ID_MAX);
    materialFloat3.Resize(OGLMaterial::FLOAT3_PROPERTY_ID_MAX);
    materialFloat1.Resize(OGLMaterial::FLOAT1_PROPERTY_ID_MAX);
    materialUInt1.Resize(OGLMaterial::UINT1_PROPERTY_ID_MAX);
}

void GeometryProgram::ExtractUniform(oglplus::SLDataType uType,
                                     std::string uName)
{
    using namespace oglplus;

    if (uType == SLDataType::Sampler2D)
    {
        // texture samplers
        auto addSamplerType =
            uName == "noneMap"
            ? RawTexture::None
            : uName == "diffuseMap"
            ? RawTexture::Diffuse
            : uName == "specularMap"
            ? RawTexture::Specular
            : uName == "ambientMap"
            ? RawTexture::Ambient
            : uName == "emissiveMap"
            ? RawTexture::Emissive
            : uName == "heightMap"
            ? RawTexture::Height
            : uName == "normalsMap"
            ? RawTexture::Normals
            : uName == "shininessMap"
            ? RawTexture::Shininess
            : uName == "opacityMap"
            ? RawTexture::Opacity
            : uName == "displacementMap"
            ? RawTexture::Displacement
            : uName == "lightMap"
            ? RawTexture::Lightmap
            : uName == "reflectionMap"
            ? RawTexture::Reflection
            : uName == "unknowMap"
            ? RawTexture::Unknow : -1;

        // geometry pass active samplers
        if (addSamplerType != -1)
        {
            samplers
            .Save(static_cast<RawTexture::TextureType>(addSamplerType),
                  UniformSampler(program, uName));
        }
    }
    else if (uType == SLDataType::FloatMat4)
    {
        auto addMat4Type =
            uName == "matrices.modelView"
            ? TransformMatrices::ModelView
            : uName == "matrices.modelViewProjection"
            ? TransformMatrices::ModelViewProjection
            : uName == "matrices.model"
            ? TransformMatrices::Model
            : uName == "matrices.view"
            ? TransformMatrices::View
            : uName == "matrices.projection"
            ? TransformMatrices::Projection
            : uName == "matrices.normal"
            ? TransformMatrices::Normal : -1;

        if (addMat4Type != -1)
        {
            transformMatrices.Save(
                static_cast<TransformMatrices::MatrixId>(addMat4Type),
                Uniform<glm::mat4x4>(program, uName));
        }
    }
    else if (uType == SLDataType::FloatVec3)
    {
        auto addF3MatType =
            uName == "material.ambient"
            ? OGLMaterial::Ambient
            : uName == "material.diffuse"
            ? OGLMaterial::Diffuse
            : uName == "material.specular"
            ? OGLMaterial::Specular
            : uName == "material.emissive"
            ? OGLMaterial::Emissive
            : uName == "material.transparent"
            ? OGLMaterial::Transparent : -1;

        if (addF3MatType != -1)
        {
            materialFloat3.
            Save(static_cast<OGLMaterial::Float3PropertyId>(addF3MatType),
                 Uniform<glm::vec3>(program, uName));
        }
    }
    else if (uType == SLDataType::Float)
    {
        auto addF1MatType =
            uName == "material.opacity"
            ? OGLMaterial::Opacity
            : uName == "material.shininess"
            ? OGLMaterial::Shininess
            : uName == "material.shininessStrenght"
            ? OGLMaterial::ShininessStrenght
            : uName == "material.refractionIndex"
            ? OGLMaterial::RefractionIndex : -1;

        if (addF1MatType != -1)
        {
            materialFloat1.
            Save(static_cast<OGLMaterial::Float1PropertyId>(addF1MatType),
                 Uniform<float>(program, uName));
        }
    }
    else if (uType == SLDataType::UnsignedInt)
    {
        auto addUint1MatType =
            uName == "material.useNormalsMap"
            ? OGLMaterial::NormalMapping : -1;

        if (addUint1MatType != -1)
        {
            materialUInt1.
            Save(static_cast<OGLMaterial::UInt1PropertyId>(addUint1MatType),
                 Uniform<unsigned int>(program, uName));
        }
    }
}

void GeometryProgram::SetUniform(TransformMatrices::MatrixId mId,
                                 const glm::mat4x4 &val)
{
    if (transformMatrices.Has(mId))
    {
        transformMatrices[mId].Set(val);
    }
}

void GeometryProgram::SetUniform(RawTexture::TextureType tId, const int val)
{
    if (samplers.Has(tId))
    {
        samplers[tId].Set(val);
    }
}

void GeometryProgram::SetUniform(OGLMaterial::Float3PropertyId mF3Id,
                                 const glm::vec3 &val)
{
    if (materialFloat3.Has(mF3Id))
    {
        materialFloat3[mF3Id].Set(val);
    }
}

void GeometryProgram::SetUniform(OGLMaterial::Float1PropertyId mF1Id,
                                 const float val)
{
    if (materialFloat1.Has(mF1Id))
    {
        materialFloat1[mF1Id].Set(val);
    }
}

void GeometryProgram::SetUniform(OGLMaterial::UInt1PropertyId mF1Id,
                                 const unsigned int val)
{
    if (materialUInt1.Has(mF1Id))
    {
        materialUInt1[mF1Id].Set(val);
    }
}

const std::vector<RawTexture::TextureType> &GeometryProgram::ActiveSamplers()
const
{
    return samplers.Actives();
}

const std::vector<TransformMatrices::MatrixId>
&GeometryProgram::ActiveTransformMatrices() const
{
    return transformMatrices.Actives();
}

const std::vector<OGLMaterial::Float3PropertyId>
&GeometryProgram::ActiveMaterialFloat3Properties() const
{
    return materialFloat3.Actives();
}

const std::vector<OGLMaterial::Float1PropertyId>
&GeometryProgram::ActiveMaterialFloat1Properties() const
{
    return materialFloat1.Actives();
}

const std::vector<OGLMaterial::UInt1PropertyId>
&GeometryProgram::ActiveMaterialUInt1Properties() const
{
    return materialUInt1.Actives();
}

