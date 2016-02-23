#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lighting_program.h"
#include "../scene/light.h"

void LightingProgram::ExtractUniforms()
{
    using namespace oglplus;
    auto &prog = *program;
    // assign program
    gPosition.Assign(prog);
    gNormal.Assign(prog);
    gAlbedo.Assign(prog);
    gSpecular.Assign(prog);
    screenSize.Assign(prog);
    // bind to uniform name
    gPosition.BindTo("gPosition");
    gNormal.BindTo("gNormal");
    gAlbedo.BindTo("gAlbedo");
    gSpecular.BindTo("gSpecular");
    screenSize.BindTo("screenSize");
	// collections
	directionalLight.resize(Light::DirectionalsLimit);

	for (auto i = 0; i < directionalLight.size(); i++)
	{
		auto& light = directionalLight[i];
		auto index = std::to_string(i);

		light.direction.Assign(prog);
		light.ambient.Assign(prog);
		light.diffuse.Assign(prog);
		light.specular.Assign(prog);
		light.direction.BindTo("directionalLight[" + index + "].direction");
		light.ambient.BindTo("directionalLight[" + index + "].ambient");
		light.diffuse.BindTo("directionalLight[" + index + "].diffuse");
		light.specular.BindTo("directionalLight[" + index + "].specular");
	}
}

LightingProgram::~LightingProgram()
{}
