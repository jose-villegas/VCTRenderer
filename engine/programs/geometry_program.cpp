#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "geometry_program.h"

void GeometryProgram::ExtractUniforms()
{
    using namespace oglplus;
    auto &prog = *program;
    // assign program
    material.diffuse.Assign(prog);
    material.specular.Assign(prog);
    material.useNormalsMap.Assign(prog);
    diffuseMap.Assign(prog);
    specularMap.Assign(prog);
    normalsMap.Assign(prog);
    alphaCutoff.Assign(prog);
    matrices.modelView.Assign(prog);
    matrices.modelViewProjection.Assign(prog);
    // bind to uniform name
    material.diffuse.BindTo("material.diffuse");
    material.specular.BindTo("material.specular");
    material.useNormalsMap.BindTo("material.useNormalsMap");
    diffuseMap.BindTo("diffuseMap");
    specularMap.BindTo("specularMap");
    normalsMap.BindTo("normalsMap");
    alphaCutoff.BindTo("alphaCutoff");
    matrices.modelView.BindTo("matrices.modelView");
    matrices.modelViewProjection.BindTo("matrices.modelViewProjection");
}

GeometryProgram::~GeometryProgram()
{
}
