#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lighting_program.h"

void LightingProgram::ExtractUniforms()
{
    using namespace oglplus;
    auto &prog = *program;
    // assign program
    gPosition.Assign(prog);
    gNormal.Assign(prog);
    gAlbedo.Assign(prog);
    gSpecular.Assign(prog);
    ambientFactor.Assign(prog);
    viewPosition.Assign(prog);
    directionalLight.direction.Assign(prog);
    screenSize.Assign(prog);
    // bind to uniform name
    gPosition.BindTo("gPosition");
    gNormal.BindTo("gNormal");
    gAlbedo.BindTo("gAlbedo");
    gSpecular.BindTo("gSpecular");
    ambientFactor.BindTo("ambientFactor");
    viewPosition.BindTo("viewPosition");
    directionalLight.direction.BindTo("directionalLight.direction");
    screenSize.BindTo("screenSize");
}

LightingProgram::~LightingProgram()
{}
