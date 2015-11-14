#include "stdafx.h"
#include "deferred_renderer.h"

bool DeferredRenderer::FrustumCulling = true;

DeferredRenderer::DeferredRenderer(const RenderWindow &rWindow) :
    deferredHandler(
        rWindow.Info().width, rWindow.Info().height)
{
    renderWindow = &rWindow;
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::Initialize()
{
}

void DeferredRenderer::Render(Scene &activeScene)
{
    using namespace oglplus;
    auto mainCamera = Camera::Main();

    // no active camera
    if (mainCamera == nullptr) { return; }

    // bind g buffer for writing
    deferredHandler.BindGeometryBuffer(FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    deferredHandler.geometryProgram.Use();
    // opengl flags
    gl.ClearDepth(1.0f);
    //gl.Disable(Capability::Blend);
    gl.Enable(Capability::DepthTest);
    gl.Enable(Capability::CullFace);
    gl.FrontFace(FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // play with camera parameters - testing
    mainCamera->position = glm::vec3(0.0f, 0.50f, 0.0f);
    mainCamera->lookAt =
        glm::vec3(
            std::sin(glfwGetTime() * 0.5f),
            0.50f,
            std::cos(glfwGetTime() * 0.5f)
        );
    mainCamera->clipPlaneFar = 10000.0f;
    // update view and projection matrices with camera parameters
    transformMatrices.UpdateProjectionMatrix
    (mainCamera->GetProjecctionMatrix());
    transformMatrices.UpdateViewMatrix
    (mainCamera->GetViewMatrix());

    // update frustum planes with new viewProjection matrix
    if (FrustumCulling) { transformMatrices.UpdateFrustumPlanes(viewFrustum); }

    // draw whole scene hierarchy tree from root node
    activeScene.rootNode.DrawRecursive();
    // start light pass
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // bind g buffer for reading
    deferredHandler.lightingProgram.Use();
    deferredHandler.ActivateBindTextureTargets();
    SetLightPassUniforms();
    deferredHandler.RenderFullscreenQuad();
}

void DeferredRenderer::SetMatricesUniforms()
{
    auto &geom = deferredHandler.geometryProgram;

    for each(auto & matrixId in geom.ActiveTransformMatrices())
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

void DeferredRenderer::SetMaterialUniforms(const std::shared_ptr<OGLMaterial>
        &mat)
{
    static const OGLMaterial * activeMaterial = nullptr;

    // no need to reset uniforms if material is already set
    if (activeMaterial == mat.get()) { return; }

    auto &geom = deferredHandler.geometryProgram;
    activeMaterial = mat.get();

    for each(auto & float3PropertyId in geom.ActiveMaterialFloat3Properties())
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

    for each(auto & float1PropertyId in geom.ActiveMaterialFloat1Properties())
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

    for each(auto & uInt1PropertyId in geom.ActiveMaterialUInt1Properties())
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

    for each(auto texType in geom.ActiveSamplers())
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
    auto &light = deferredHandler.lightingProgram;
    light.SetUniform(Camera::Main()->position);
    light.SetUniform(
        DeferredProgram::Position,
        DeferredProgram::Position
    );
    light.SetUniform(
        DeferredProgram::Normal,
        DeferredProgram::Normal
    );
    light.SetUniform(
        DeferredProgram::Albedo,
        DeferredProgram::Albedo
    );
    light.SetUniform(
        DeferredProgram::Specular,
        DeferredProgram::Specular
    );
}