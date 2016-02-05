#include "lighting_program.h"
#include "geometry_program.h"

LightingProgram::LightingProgram()
{
    samplers.Resize(static_cast<size_t>(GBufferTextureId::TEXTURE_TYPE_MAX));
}

void LightingProgram::ExtractUniform(oglplus::SLDataType uType,
                                     std::string uName)
{
    using namespace oglplus;
    //uBlock = UniformBlock(program, "SceneLights");
    //uBlock.Binding(1);
    //auto bSize = uBlock.DataSize();

    // extract light uniform data
    if (uType == SLDataType::FloatVec3)
    {
        auto float3ToSet =
            uName == "viewPosition"
            ? &viewPosUniform
            : uName == "directionalLight.ambient"
            ? &directionalLight.ambient
            : uName == "directionalLight.diffuse"
            ? &directionalLight.diffuse
            : uName == "directionalLight.specular"
            ? &directionalLight.specular
            : uName == "directionalLight.position"
            ? &directionalLight.position
            : uName == "directionalLight.direction"
            ? &directionalLight.direction : nullptr;

        if (float3ToSet != nullptr)
        {
            *float3ToSet = Uniform<glm::vec3>(program, uName);
        }
    }

    if (uType == SLDataType::Float)
    {
        auto floatToSet =
            uName == "directionalLight.angleInnerCone"
            ? &directionalLight.angleInnerCone
            : uName == "directionalLight.angleOuterCone"
            ? &directionalLight.angleOuterCone
            : uName == "directionalLight.constant"
            ? &directionalLight.attenuation.constant
            : uName == "directionalLight.linear"
            ? &directionalLight.attenuation.linear
            : uName == "directionalLight.quadratic"
            ? &directionalLight.attenuation.quadratic : nullptr;

        if (floatToSet != nullptr)
        {
            *floatToSet = Uniform<float>(program, uName);
        }
    }

    if (uType == SLDataType::UnsignedInt)
    {
        auto uintToSet =
            uName == "directionalLight.lightType"
            ? &directionalLight.lightType : nullptr;

        if (uintToSet != nullptr)
        {
            *uintToSet = Uniform<unsigned int>(program, uName);
        }
    }

    if (uType == SLDataType::Sampler2D)
    {
        // gbuffer samplers to read
        auto addGSamplerType =
            uName == "gPosition"
            ? (int)GBufferTextureId::Position
            : uName == "gNormal"
            ? (int)GBufferTextureId::Normal
            : uName == "gAlbedo"
            ? (int)GBufferTextureId::Albedo
            : uName == "gSpecular"
            ? (int)GBufferTextureId::Specular : -1;

        // g buffer active samplers
        if ((int)addGSamplerType != -1)
        {
            samplers.Save(static_cast<GBufferTextureId>(addGSamplerType),
                          UniformSampler(program, uName));
        }
    }
}

void LightingProgram::SetUniform(GBufferTextureId tId, const int val)
{
    if (samplers.Has(tId))
    {
        samplers[tId].Set(val);
    }
}

void LightingProgram::SetUniform(const glm::vec3 &val, bool viewPosition)
{
    if (viewPosition == true)
    {
        viewPosUniform.Set(val);
    }
}
