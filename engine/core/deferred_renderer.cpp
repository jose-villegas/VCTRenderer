#include "deferred_renderer.h"

#include "render_window.h"
#include "../scene/camera.h"
#include "../scene/scene.h"

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
    deferredHandler.geometryProgram.Use();
    // opengl flags
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
    scene->rootNode.DrawRecursive();
    // start light pass
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // bind g buffer for reading
    deferredHandler.lightingProgram.Use();
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

    for (auto &matrixId : geom.ActiveTransformMatrices())
    {
        switch (matrixId)
        {
            default:
                break;

            case TransformMatrices::ModelView:
                geom.SetUniform(
                    TransformMatrices::ModelView,
                    transformMatrices.GetModelView()
                );
                break;

            case TransformMatrices::ModelViewProjection:
                geom.SetUniform(
                    TransformMatrices::ModelViewProjection,
                    transformMatrices.GetModelViewProjection()
                );
                break;

            case TransformMatrices::Model:
                geom.SetUniform(
                    TransformMatrices::Model,
                    transformMatrices.GetModel()
                );
                break;

            case TransformMatrices::View:
                geom.SetUniform(
                    TransformMatrices::View,
                    transformMatrices.GetView()
                );
                break;

            case TransformMatrices::Projection:
                geom.SetUniform(
                    TransformMatrices::Projection,
                    transformMatrices.GetProjection()
                );
                break;

            case TransformMatrices::Normal:
                geom.SetUniform(
                    TransformMatrices::Normal,
                    transformMatrices.GetNormal()
                );
                break;
        }
    }
}

void DeferredRenderer::SetMaterialUniforms(std::shared_ptr<OGLMaterial> &mat)
{
    static auto &geom = deferredHandler.geometryProgram;

    for (auto &float3PropertyId : geom.ActiveMaterialFloat3Properties())
    {
        switch (float3PropertyId)
        {
            default:
                break;

            case OGLMaterial::Ambient:
                geom.SetUniform(
                    OGLMaterial::Ambient,
                    mat->HasTexture(RawTexture::Ambient) ?
                    mat->White : mat->ambient
                );
                break;

            case OGLMaterial::Diffuse:
                geom.SetUniform(
                    OGLMaterial::Diffuse,
                    mat->HasTexture(RawTexture::Diffuse) ?
                    mat->White : mat->diffuse
                );
                break;

            case OGLMaterial::Specular:
                geom.SetUniform(
                    OGLMaterial::Specular,
                    mat->HasTexture(RawTexture::Specular) ?
                    mat->White : mat->specular
                );
                break;

            case OGLMaterial::Emissive:
                geom.SetUniform(
                    OGLMaterial::Emissive,
                    mat->HasTexture(RawTexture::Emissive) ?
                    mat->White : mat->emissive
                );
                break;

            case OGLMaterial::Transparent:
                geom.SetUniform
                (OGLMaterial::Transparent, mat->transparent);
                break;
        }
    }

    for (auto &float1PropertyId : geom.ActiveMaterialFloat1Properties())
    {
        switch (float1PropertyId)
        {
            default:
                break;

            case OGLMaterial::Opacity:
                geom.SetUniform(
                    OGLMaterial::Opacity,
                    mat->HasTexture(RawTexture::Opacity) ?
                    1.0f : mat->opacity
                );
                break;

            case OGLMaterial::Shininess:
                geom.SetUniform(
                    OGLMaterial::Shininess,
                    mat->HasTexture(RawTexture::Shininess) ?
                    0.0f : mat->shininess
                );
                break;

            case OGLMaterial::ShininessStrenght:
                geom.SetUniform
                (OGLMaterial::ShininessStrenght, mat->shininessStrenght);
                break;

            case OGLMaterial::RefractionIndex:
                geom.SetUniform
                (OGLMaterial::RefractionIndex, mat->refractionIndex);
                break;
        }
    }

    for (auto &uInt1PropertyId : geom.ActiveMaterialUInt1Properties())
    {
        switch (uInt1PropertyId)
        {
            default:
                break;

            case OGLMaterial::NormalMapping:
                geom.SetUniform(
                    OGLMaterial::NormalMapping,
                    mat->HasTexture(RawTexture::Normals)
                );
                break;
        }
    }

    for (auto &texType : geom.ActiveSamplers())
    {
        oglplus::Texture::Active(texType);

        if (!mat->BindTexture(texType))
        {
            OGLTexture2D::GetDefaultTexture()->Bind();
        }

        geom.SetUniform(texType, static_cast<int>(texType));
    }
}

void DeferredRenderer::SetLightPassUniforms()
{
    static auto &light = deferredHandler.lightingProgram;
    light.SetUniform(Camera::Active()->position);
    light.SetUniform(
        GBufferTextureId::Position,
        (int)GBufferTextureId::Position
    );
    light.SetUniform(
        GBufferTextureId::Normal,
        (int)GBufferTextureId::Normal
    );
    light.SetUniform(
        GBufferTextureId::Albedo,
        (int)GBufferTextureId::Albedo
    );
    light.SetUniform(
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