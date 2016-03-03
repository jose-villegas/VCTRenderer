#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_renderer.h"

#include "../scene/scene.h"
#include "../scene/camera.h"

#include <oglplus/context.hpp>
#include "../types/geometry_buffer.h"

void VoxelRenderer::Render()
{
    static oglplus::Context gl;
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();

    if (!camera || !scene || !scene->IsLoaded()) { return; }

    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    gl.Disable(oglplus::Capability::DepthTest);
    gl.ColorMask(false, false, false, false);
}

VoxelRenderer::VoxelRenderer(RenderWindow &window) : Renderer(window)
{
}

VoxelRenderer::~VoxelRenderer()
{
}
