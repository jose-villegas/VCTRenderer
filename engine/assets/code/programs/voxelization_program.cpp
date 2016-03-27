#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxelization_program.h"

#include "../../../scene/light.h"

void VoxelizationProgram::ExtractUniforms()
{
    // program owner
    shadowMap.Assign(program);
    lightViewProjection.Assign(program);
    matrices.model.Assign(program);
    matrices.normal.Assign(program);
    material.diffuse.Assign(program);
    material.specular.Assign(program);
    material.shininess.Assign(program);
    diffuseMap.Assign(program);
    specularMap.Assign(program);
    lightTypeCount[0].Assign(program);
    lightTypeCount[1].Assign(program);
    lightTypeCount[2].Assign(program);
    volumeDimension.Assign(program);
    viewProjections[0].Assign(program);
    viewProjections[1].Assign(program);
    viewProjections[2].Assign(program);
    // binding point
    shadowMap.BindTo("shadowMap");
    lightViewProjection.BindTo("lightViewProjection");
    matrices.model.BindTo("matrices.model");
    matrices.normal.BindTo("matrices.normal");
    material.diffuse.BindTo("material.diffuse");
    material.specular.BindTo("material.specular");
    material.shininess.BindTo("material.shininess");
    diffuseMap.BindTo("diffuseMap");
    specularMap.BindTo("specularMap");
    volumeDimension.BindTo("volumeDimension");
    viewProjections[0].BindTo("viewProjections[0]");
    viewProjections[1].BindTo("viewProjections[1]");
    viewProjections[2].BindTo("viewProjections[2]");
    // lighting uniforms
    lightTypeCount[0].BindTo("lightTypeCount[0]");
    lightTypeCount[1].BindTo("lightTypeCount[1]");
    lightTypeCount[2].BindTo("lightTypeCount[2]");
    // collections
    directionalLight.resize(Light::DirectionalsLimit);
    pointLight.resize(Light::PointsLimit);
    spotLight.resize(Light::SpotsLimit);

    for (auto i = 0; i < directionalLight.size(); i++)
    {
        auto &light = directionalLight[i];
        auto index = std::to_string(i);
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
};

VoxelizationProgram::~VoxelizationProgram()
{
}
