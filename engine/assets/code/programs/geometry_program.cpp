#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "geometry_program.h"

void GeometryProgram::ExtractUniforms()
{
    using namespace oglplus;
    // assign program
    material.diffuse.Assign(program);
    material.specular.Assign(program);
    material.shininess.Assign(program);
    material.useNormalsMap.Assign(program);
    diffuseMap.Assign(program);
    specularMap.Assign(program);
    normalsMap.Assign(program);
    alphaCutoff.Assign(program);
    matrices.normal.Assign(program);
    matrices.modelViewProjection.Assign(program);
    // bind to uniform name
    material.diffuse.BindTo("material.diffuse");
    material.specular.BindTo("material.specular");
    material.shininess.BindTo("material.shininess");
    material.useNormalsMap.BindTo("material.useNormalsMap");
    diffuseMap.BindTo("diffuseMap");
    specularMap.BindTo("specularMap");
    normalsMap.BindTo("normalsMap");
    alphaCutoff.BindTo("alphaCutoff");
    matrices.normal.BindTo("matrices.normal");
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
}
GeometryProgram::~GeometryProgram()
{
}