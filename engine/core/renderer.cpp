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
    // bind g buffer for writing
    deferredHandler.BindGBuffer(FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    deferredHandler.UseGeometryPass();
    // opengl flags
    gl.ClearDepth(1.0f);
    //gl.Disable(Capability::Blend);
    gl.Enable(Capability::DepthTest);
    gl.Enable(Capability::CullFace);
    gl.FrontFace(FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // play with camera parameters - testing
    activeCamera.position = glm::vec3(0.0f, 0.50f, 0.0f);
    activeCamera.lookAt =
        glm::vec3(
            std::sin(glfwGetTime() * 0.5f),
            0.50f,
            std::cos(glfwGetTime() * 0.5f)
        );
    activeCamera.clipPlaneFar = 10000.0f;
    // update view and projection matrices with camera parameters
    transformMatrices.UpdateProjectionMatrix(activeCamera.GetProjecctionMatrix());
    transformMatrices.UpdateViewMatrix(activeCamera.GetViewMatrix());

    // update frustum planes with new viewProjection matrix
    if (FrustumCulling) { transformMatrices.UpdateFrustumPlanes(viewFrustum); }

    // draw whole scene hierarchy tree from root node
    activeScene.rootNode.DrawRecursive();
    // start light pass
    DefaultFramebuffer().Bind(FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // bind g buffer for reading
    deferredHandler.UseLightPass();
    deferredHandler.ActivateGBufferTextures();
    deferredHandler.SetLightPassUniforms(activeCamera.position,
                                         activeScene.lights);
    deferredHandler.RenderFSQuad();
}

void Renderer::SetMatricesUniforms()
{
    for each(auto & matrixId in deferredHandler.geometryPass
             .ActiveTransformMatrices())
    {
        switch (matrixId)
        {
            default:
                break;

            case TransformMatrices::ModelView:
                deferredHandler.geometryPass.SetUniform(
                    TransformMatrices::ModelView,
                    transformMatrices.GetModelView()
                );
                break;

            case TransformMatrices::ModelViewProjection:
                deferredHandler.geometryPass.SetUniform(
                    TransformMatrices::ModelViewProjection,
                    transformMatrices.GetModelViewProjection()
                );
                break;

            case TransformMatrices::Model:
                deferredHandler.geometryPass.SetUniform(
                    TransformMatrices::Model,
                    transformMatrices.GetModel()
                );
                break;

            case TransformMatrices::View:
                deferredHandler.geometryPass.SetUniform(
                    TransformMatrices::View,
                    transformMatrices.GetView()
                );
                break;

            case TransformMatrices::Projection:
                deferredHandler.geometryPass.SetUniform(
                    TransformMatrices::Projection,
                    transformMatrices.GetProjection()
                );
                break;

            case TransformMatrices::Normal:         deferredHandler.geometryPass.SetUniform(
                    TransformMatrices::Normal,
                    transformMatrices.GetNormal()
                );
                break;
        }
    }
}

void Renderer::SetMaterialUniforms(const OGLMaterial &mat)
{
    for each(auto & float3PropertyId in deferredHandler.geometryPass
             .ActiveMaterialFloat3Properties())
    {
        switch (float3PropertyId)
        {
            default:
                break;

            case OGLMaterial::Ambient:
                deferredHandler.geometryPass.SetUniform(
                    OGLMaterial::Ambient,
                    mat.HasTexture(RawTexture::Ambient) ?
                    mat.White : mat.ambient
                );
                break;

            case OGLMaterial::Diffuse:
                deferredHandler.geometryPass.SetUniform(
                    OGLMaterial::Diffuse,
                    mat.HasTexture(RawTexture::Diffuse) ?
                    mat.White : mat.diffuse
                );
                break;

            case OGLMaterial::Specular:
                deferredHandler.geometryPass.SetUniform(
                    OGLMaterial::Specular,
                    mat.HasTexture(RawTexture::Specular) ?
                    mat.White : mat.specular
                );
                break;

            case OGLMaterial::Emissive:
                deferredHandler.geometryPass.SetUniform(
                    OGLMaterial::Emissive,
                    mat.HasTexture(RawTexture::Emissive) ?
                    mat.White : mat.emissive
                );
                break;

            case OGLMaterial::Transparent:
                deferredHandler.geometryPass.SetUniform
                (OGLMaterial::Transparent, mat.transparent);
                break;
        }
    }

    for each(auto & float1PropertyId in deferredHandler.geometryPass
             .ActiveMaterialFloat1Properties())
    {
        switch (float1PropertyId)
        {
            default:
                break;

            case OGLMaterial::Opacity:
                deferredHandler.geometryPass.SetUniform(
                    OGLMaterial::Opacity,
                    mat.HasTexture(RawTexture::Opacity) ?
                    1.0f : mat.opacity
                );
                break;

            case OGLMaterial::Shininess:
                deferredHandler.geometryPass.SetUniform(
                    OGLMaterial::Shininess,
                    mat.HasTexture(RawTexture::Shininess) ?
                    0.0f : mat.shininess
                );
                break;

            case OGLMaterial::ShininessStrenght:
                deferredHandler.geometryPass.SetUniform
                (OGLMaterial::ShininessStrenght, mat.shininessStrenght);
                break;

            case OGLMaterial::RefractionIndex:
                deferredHandler.geometryPass.SetUniform
                (OGLMaterial::RefractionIndex, mat.refractionIndex);
                break;
        }
    }

    for each(auto & uInt1PropertyId in deferredHandler.geometryPass
             .ActiveMaterialUInt1Properties())
    {
        switch (uInt1PropertyId)
        {
            default:
                break;

            case OGLMaterial::NormalMapping:
                deferredHandler.geometryPass.SetUniform(
                    OGLMaterial::NormalMapping,
                    mat.HasTexture(RawTexture::Normals)
                );
                break;
        }
    }

    for each(auto texType in deferredHandler.geometryPass.ActiveSamplers())
    {
        oglplus::Texture::Active(texType);

        if (!mat.BindTexture(texType))
        {
            OGLTexture2D::GetDefaultTexture()->Bind();
        }

        deferredHandler.geometryPass
        .SetUniform(texType, static_cast<int>(texType));
    }
}
