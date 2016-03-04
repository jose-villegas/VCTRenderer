#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

#include "../scene/node.h"

const bool Renderer::UseFrustumCulling = true;

RenderWindow &Renderer::Window() const
{
    return *window;
}

void Renderer::SetMatricesUniforms(const Node &node) const
{
}

void Renderer::SetMaterialUniforms(const Material &material) const
{
}

Renderer::Renderer(): window(nullptr), program(nullptr)
{
}

Renderer::Renderer(RenderWindow * window): program(nullptr)
{
    this->window = window;
}

Renderer::~Renderer()
{
}
