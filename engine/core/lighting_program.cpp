#include "lighting_program.h"

LightingProgram::LightingProgram()
{
}

void LightingProgram::ExtractUniform(const oglplus::aux::ActiveUniformInfo
                                     &info)
{
    using namespace oglplus;
    auto &name = info.Name();

    if (name == "gPosition")
    {
        gPosition = UniformSampler(program, name);
    }
    else if (name == "gNormal")
    {
        gNormal = UniformSampler(program, name);
    }
    else if (name == "gAlbedo")
    {
        gAlbedo = UniformSampler(program, name);
    }
    else if (name == "gSpecular")
    {
        gSpecular = UniformSampler(program, name);
    }
    else if (name == "ambientFactor")
    {
        ambientFactor = Uniform<float>(program, name);
    }
    else if (name == "viewPosition")
    {
        viewPosition = Uniform<glm::vec3>(program, name);
    }
    else if (name == "directionalLight.direction")
    {
        directionalLight.direction = Uniform<glm::vec3>(program, name);
    }
    else if (name == "screenSize")
    {
        screenSize = Uniform<glm::vec2>(program, name);
    }
}
