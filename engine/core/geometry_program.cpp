#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "geometry_program.h"

void GeometryProgram::ExtractUniforms()
{
    using namespace oglplus;
    auto &prog = *program;
    material.diffuse = Uniform<glm::vec3>(prog, "material.diffuse");
    material.specular = Uniform<glm::vec3>(prog, "material.specular");
    material.useNormalsMap = Uniform<unsigned int>(prog,
                             "material.useNormalsMap");
    diffuseMap = UniformSampler(prog, "diffuseMap");
    specularMap = UniformSampler(prog, "specularMap");
    normalsMap = UniformSampler(prog, "normalsMap");
    alphaCutoff = Uniform<float>(prog, "alphaCutoff");
    matrices.normal  = Uniform<glm::mat4>(prog, "matrices.normal");
    matrices.modelView = Uniform<glm::mat4>(prog, "matrices.modelView");
    matrices.modelViewProjection = Uniform<glm::mat4>(prog,
                                   "matrices.modelViewProjection");
}

GeometryProgram::GeometryProgram()
{
}
