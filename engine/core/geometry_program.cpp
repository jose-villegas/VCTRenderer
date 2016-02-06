#include "geometry_program.h"

void GeometryProgram::ExtractUniform(const oglplus::aux::ActiveUniformInfo
                                     &info)
{
    using namespace oglplus;
    auto &name = info.Name();

    // fragment shader names
    if (name == "material.diffuse")
    {
        materialDiffuse = Uniform<glm::vec3>(program, name);
    }
    else if (name == "material.specular")
    {
        materialSpecular = Uniform<glm::vec3>(program, name);
    }
    else if (name == "material.useNormalsMap")
    {
        materialUseNormalsMap = Uniform<unsigned int>(program, name);
    }
    else if (name == "diffuseMap")
    {
        diffuseMap = UniformSampler(program, name);
    }
    else if (name == "specularMap")
    {
        specularMap = UniformSampler(program, name);
    }
    else if (name == "normalsMap")
    {
        normalsMap = UniformSampler(program, name);
    }
    else if (name == "alphaCutoff")
    {
        alphaCutoff = Uniform<float>(program, name);
    }
    else if (name == "matrices.normal")
    {
        matricesNormal = Uniform<glm::mat4>(program, name);
    }
    else if (name == "matrices.modelView")
    {
        matricesModelView = Uniform<glm::mat4>(program, name);
    }
    else if (name == "matrices.modelViewProjection")
    {
        matricesModelViewProjection = Uniform<glm::mat4>(program, name);
    }
}

GeometryProgram::GeometryProgram()
{
}
