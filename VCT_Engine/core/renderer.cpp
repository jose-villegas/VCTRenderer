#include "stdafx.h"
#include "renderer.h"
#include "engine_base.h"
bool Renderer::FrustumCulling = true;

Renderer::Renderer(const RenderWindow &rWindow) : deferredHandler(
        rWindow.Settings().width, rWindow.Settings().height)
{
    renderWindow = &rWindow;
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
}

void Renderer::Render(Scene &activeScene, Camera &activeCamera)
{
    using namespace oglplus;
    // bind gbuffer for writing
    deferredHandler.BindGBuffer(FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // opengl flags
    gl.ClearDepth(1.0f);
    //gl.Disable(Capability::Blend);
    gl.Enable(Capability::DepthTest);
    gl.Enable(Capability::CullFace);
    gl.FrontFace(FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // activate geometry pass shader program
    deferredHandler.UseGeometryPass();
    // play with camera parameters
    activeCamera.position = glm::vec3(0.0f, -12.5f, 0.0f);
    activeCamera.lookAt =
        glm::vec3(
            std::sin(glfwGetTime()),
            -12.5f,
            std::cos(glfwGetTime())
        );
    activeCamera.clipPlaneFar = 10000.0f;
    // update view and projection matrices with camera parameters
    transformMatrices.UpdateProjectionMatrix(activeCamera.GetProjecctionMatrix());
    transformMatrices.UpdateViewMatrix(activeCamera.GetViewMatrix());

    // update frustum planes with new viewProjection matrix
    if(FrustumCulling) transformMatrices.UpdateFrustumPlanes(viewFrustum);

    // draw whole scene hierachy tree from root node
    activeScene.rootNode.DrawRecursive();
    // unbind geom buffer
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
}
