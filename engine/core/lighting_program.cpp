#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lighting_program.h"

LightingProgram::LightingProgram()
{
}

void LightingProgram::ExtractUniforms()
{
    using namespace oglplus;
    auto &prog = *program;
    gPosition = UniformSampler(prog, "gPosition");
    gNormal = UniformSampler(prog, "gNormal");
    gAlbedo = UniformSampler(prog, "gAlbedo");
    gSpecular = UniformSampler(prog, "gSpecular");
    ambientFactor = Uniform<float>(prog, "ambientFactor");
    viewPosition = Uniform<glm::vec3>(prog, "viewPosition");
    directionalLight.direction = Uniform<glm::vec3>(prog,
                                 "directionalLight.direction");
    screenSize = Uniform<glm::vec2>(prog, "screenSize");
}
