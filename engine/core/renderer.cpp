#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "../scene/node.h"

void Renderer::SetMatricesUniforms(const Node &node) const
{
}

void Renderer::SetMaterialUniforms(const Material &material) const
{
}

Renderer::Renderer(): program(nullptr)
{
}

Renderer::~Renderer()
{
}
