#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

#include "../scene/node.h"

bool Renderer::UseFrustumCulling = true;

RenderWindow &Renderer::Window() const
{
    return *window;
}

void Renderer::RenderAll()
{
    for (auto &renderer : instances)
    {
        renderer->Render();
    }
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

Renderer::Renderer(RenderWindow &window): program(nullptr)
{
    this->window = std::make_unique<std::reference_wrapper
                   <RenderWindow>>(window);
}

Renderer::~Renderer()
{
}
