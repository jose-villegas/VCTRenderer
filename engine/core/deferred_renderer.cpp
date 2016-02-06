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
    deferredHandler.BindGeometryBuffer(oglplus::FramebufferTarget::Draw);
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
    deferredHandler.ActivateBindTextureTargets();
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
    geom->matricesNormal.Set(transformMatrices.GetNormal());
    geom->matricesModelView.Set(transformMatrices.GetModelView());
    geom->matricesModelViewProjection.Set(
        transformMatrices.GetModelViewProjection());
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
    geom->materialDiffuse.Set(mat->diffuse);
    geom->materialSpecular.Set(mat->HasTexture(RawTexture::Specular) ?
                               mat->White : mat->specular);
    geom->materialUseNormalsMap.Set(mat->HasTexture(RawTexture::Normals));
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
    light->SetUniform(Camera::Active()->position);
    light->SetUniform(
        GBufferTextureId::Position,
        (int)GBufferTextureId::Position
    );
    light->SetUniform(
        GBufferTextureId::Normal,
        (int)GBufferTextureId::Normal
    );
    light->SetUniform(
        GBufferTextureId::Albedo,
        (int)GBufferTextureId::Albedo
    );
    light->SetUniform(
        GBufferTextureId::Specular,
        (int)GBufferTextureId::Specular
    );
}

bool DeferredRenderer::InFrustum(const BoundingVolume &volume)
{
    return viewFrustum.BoxInFrustum(volume);
}

TransformMatrices &DeferredRenderer::Matrices()
{
    return transformMatrices;
}