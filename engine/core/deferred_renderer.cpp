#include "deferred_renderer.h"
#include "geometry_program.h"
#include "lighting_program.h"

#include "render_window.h"
#include "../scene/camera.h"
#include "../scene/scene.h"
#include "../scene/light.h"

bool DeferredRenderer::FrustumCulling = true;

DeferredRenderer::DeferredRenderer(const RenderWindow &rWindow) :
    deferredHandler(rWindow.Info().width, rWindow.Info().height)
{
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::Initialize()
{
}

void DeferredRenderer::Render()
{
    static oglplus::Context gl;
    auto &camera = Camera::Active();
    auto &scene = Scene::Active();

    if (!camera || !scene) { return; }

    // bind g buffer for writing
    deferredHandler.geometryBuffer.Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    deferredHandler.geometryProgram->Use();
    // Open GL flags
    gl.ClearDepth(1.0f);
    //gl.Disable(Capability::Blend);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.Enable(oglplus::Capability::CullFace);
    gl.FrontFace(oglplus::FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // play with camera parameters - testing
    camera->position = glm::vec3(0.0f, 0.50f, 0.0f);
    camera->lookAt =
        glm::vec3(
            sin(glfwGetTime() * 0.5f),
            0.50f,
            cos(glfwGetTime()  * 0.5f)
        );
    camera->clipPlaneFar = 10000.0f;
    // update view and projection matrices with camera parameters
    transformMatrices.UpdateProjectionMatrix
    (camera->GetProjecctionMatrix());
    transformMatrices.UpdateViewMatrix
    (camera->GetViewMatrix());

    // update frustum planes with new viewProjection matrix
    if (FrustumCulling) { transformMatrices.UpdateFrustumPlanes(viewFrustum); }

    // draw whole scene hierarchy tree from root node
    scene->rootNode.DrawList();
    // start light pass
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // bind g buffer for reading
    deferredHandler.lightingProgram->Use();
    deferredHandler.geometryBuffer.ActivateTextures();
    SetLightPassUniforms();
    deferredHandler.RenderFullscreenQuad();
}

DeferredHandler &DeferredRenderer::GetDeferredHandler()
{
    return deferredHandler;
}

void DeferredRenderer::SetMatricesUniforms()
{
    static auto &geom = deferredHandler.geometryProgram;
    geom->matrices.normal.Set(transformMatrices.GetNormal());
    geom->matrices.modelView.Set(transformMatrices.GetModelView());
    geom->matrices.modelViewProjection.Set(transformMatrices
                                           .GetModelViewProjection());
}

void DeferredRenderer::SetMaterialUniforms(std::shared_ptr<OGLMaterial> &mat)
{
    using namespace oglplus;
    static OGLMaterial * previousMaterial = nullptr;
    static auto &geom = deferredHandler.geometryProgram;

    if (previousMaterial == mat.get())
    {
        return;
    }

    previousMaterial = mat.get();
    geom->material.diffuse.Set(mat->diffuse);
    geom->material.specular.Set(mat->HasTexture(RawTexture::Specular) ?
                                mat->White : mat->specular);
    geom->material.useNormalsMap.Set(mat->HasTexture(RawTexture::Normals));
    // set textures
    Texture::Active(RawTexture::Diffuse);
    mat->BindTexture(RawTexture::Diffuse);
    geom->diffuseMap.Set(RawTexture::Diffuse);
    Texture::Active(RawTexture::Specular);
    mat->BindTexture(RawTexture::Specular);
    geom->specularMap.Set(RawTexture::Specular);
    Texture::Active(RawTexture::Normals);
    mat->BindTexture(RawTexture::Normals);
    geom->normalsMap.Set(RawTexture::Normals);
}

void DeferredRenderer::SetLightPassUniforms()
{
    static auto &light = deferredHandler.lightingProgram;
    light->viewPosition.Set(Camera::Active()->position);
    light->gPosition.Set(GeometryBuffer::Position);
    light->gNormal.Set(GeometryBuffer::Normal);
    light->gAlbedo.Set(GeometryBuffer::Albedo);
    light->gSpecular.Set(GeometryBuffer::Specular);
}

bool DeferredRenderer::InFrustum(const BoundingVolume &volume)
{
    return viewFrustum.BoxInFrustum(volume);
}

TransformMatrices &DeferredRenderer::Matrices()
{
    return transformMatrices;
}