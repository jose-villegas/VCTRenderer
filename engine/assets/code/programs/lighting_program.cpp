#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lighting_program.h"
#include "../scene/light.h"

void LightingProgram::ExtractUniforms()
{
    using namespace oglplus;
    // assign program
    inverseProjectionView.Assign(program);
    lightViewProjection.Assign(program);
    gDepth.Assign(program);
    gNormal.Assign(program);
    gAlbedo.Assign(program);
    gSpecular.Assign(program);
    shadowMap.Assign(program);
    ;
    exponents.Assign(program);
    lightBleedingReduction.Assign(program);
    // bind to uniform name
    inverseProjectionView.BindTo("inverseProjectionView");
    lightViewProjection.BindTo("lightViewProjection");
    gDepth.BindTo("gDepth");
    gNormal.BindTo("gNormal");
    gAlbedo.BindTo("gAlbedo");
    gSpecular.BindTo("gSpecular");
    shadowMap.BindTo("shadowMap");
    exponents.BindTo("exponents");
    lightBleedingReduction.BindTo("lightBleedingReduction");
    // collections
    lightTypeCount[0].Assign(program);
    lightTypeCount[1].Assign(program);
    lightTypeCount[2].Assign(program);
    lightTypeCount[0].BindTo("lightTypeCount[0]");
    lightTypeCount[1].BindTo("lightTypeCount[1]");
    lightTypeCount[2].BindTo("lightTypeCount[2]");
    directionalLight.resize(Light::DirectionalsLimit);
    pointLight.resize(Light::PointsLimit);
    spotLight.resize(Light::SpotsLimit);

    for (auto i = 0; i < directionalLight.size(); i++)
    {
        auto &light = directionalLight[i];
        auto index = std::to_string(i);
        light.shadowingMethod.Assign(program);
        light.shadowingMethod.BindTo("directionalLight[" + index + "].shadowingMethod");
        light.direction.Assign(program);
        light.ambient.Assign(program);
        light.diffuse.Assign(program);
        light.specular.Assign(program);
        light.direction.BindTo("directionalLight[" + index + "].direction");
        light.ambient.BindTo("directionalLight[" + index + "].ambient");
        light.diffuse.BindTo("directionalLight[" + index + "].diffuse");
        light.specular.BindTo("directionalLight[" + index + "].specular");
    }

    for (auto i = 0; i < pointLight.size(); i++)
    {
        auto &light = pointLight[i];
        auto index = std::to_string(i);
        light.shadowingMethod.Assign(program);
        light.shadowingMethod.BindTo("pointLight[" + index + "].shadowingMethod");
        light.position.Assign(program);
        light.ambient.Assign(program);
        light.diffuse.Assign(program);
        light.specular.Assign(program);
        light.attenuation.constant.Assign(program);
        light.attenuation.linear.Assign(program);
        light.attenuation.quadratic.Assign(program);
        light.position.BindTo("pointLight[" + index + "].position");
        light.ambient.BindTo("pointLight[" + index + "].ambient");
        light.diffuse.BindTo("pointLight[" + index + "].diffuse");
        light.specular.BindTo("pointLight[" + index + "].specular");
        light.attenuation.constant.BindTo("pointLight[" + index +
                                          "].attenuation.constant");
        light.attenuation.linear.BindTo("pointLight[" + index +
                                        "].attenuation.linear");
        light.attenuation.quadratic.BindTo("pointLight[" + index +
                                           "].attenuation.quadratic");
    }

    for (auto i = 0; i < spotLight.size(); i++)
    {
        auto &light = spotLight[i];
        auto index = std::to_string(i);
        light.shadowingMethod.Assign(program);
        light.shadowingMethod.BindTo("spotLight[" + index + "].shadowingMethod");
        light.position.Assign(program);
        light.direction.Assign(program);
        light.ambient.Assign(program);
        light.diffuse.Assign(program);
        light.specular.Assign(program);
        light.attenuation.constant.Assign(program);
        light.attenuation.linear.Assign(program);
        light.attenuation.quadratic.Assign(program);
        light.angleInnerCone.Assign(program);
        light.angleOuterCone.Assign(program);
        light.position.BindTo("spotLight[" + index + "].position");
        light.direction.BindTo("spotLight[" + index + "].direction");
        light.ambient.BindTo("spotLight[" + index + "].ambient");
        light.diffuse.BindTo("spotLight[" + index + "].diffuse");
        light.specular.BindTo("spotLight[" + index + "].specular");
        light.attenuation.constant.BindTo("spotLight[" + index +
                                          "].attenuation.constant");
        light.attenuation.linear.BindTo("spotLight[" + index +
                                        "].attenuation.linear");
        light.attenuation.quadratic.BindTo("spotLight[" + index +
                                           "].attenuation.quadratic");
        light.angleInnerCone.BindTo("spotLight[" + index + "].angleInnerCone");
        light.angleOuterCone.BindTo("spotLight[" + index + "].angleOuterCone");
    }

    cameraPosition.Assign(program);
    cameraPosition.BindTo("cameraPosition");
    // voxel volume setup
    volumeDimension.Assign(program);
    volumeDimension.BindTo("volumeDimension");
    voxelTex.Assign(program);
    voxelTex.BindTo("voxelTex");
    voxelTexMipmap.Assign(program);
    voxelTexMipmap.BindTo("voxelTexMipmap");
    // from world to voxel space
    worldMinPoint.Assign(program);
    worldMinPoint.BindTo("worldMinPoint");
    voxelScale.Assign(program);
    voxelScale.BindTo("voxelScale");
}

LightingProgram::~LightingProgram()
{}
