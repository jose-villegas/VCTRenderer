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
            std::sin(glfwGetTime() * 0.5f),
            -12.5f,
            std::cos(glfwGetTime() * 0.5f)
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

void Renderer::SetMatricesUniforms() const
{
    for(auto it = ActiveTransformMatrices().begin();
        it != ActiveTransformMatrices().end(); ++it)
    {
        (*it) == TransformMatrices::ModelView ?
        deferredHandler.geometryPass.SetUniform(
            TransformMatrices::ModelView,
            transformMatrices.GetModelView()
        )
        : (*it) == TransformMatrices::ModelViewProjection ?
        deferredHandler.geometryPass.SetUniform(
            TransformMatrices::ModelViewProjection,
            transformMatrices.GetModelViewProjection()
        )
        : (*it) == TransformMatrices::Model ?
        deferredHandler.geometryPass.SetUniform(
            TransformMatrices::Model,
            transformMatrices.GetModel()
        )
        : (*it) == TransformMatrices::View ?
        deferredHandler.geometryPass.SetUniform(
            TransformMatrices::View,
            transformMatrices.GetView()
        )
        : (*it) == TransformMatrices::Projection ?
        deferredHandler.geometryPass.SetUniform(
            TransformMatrices::Projection,
            transformMatrices.GetProjection()
        )
        : (*it) == TransformMatrices::Normal ?
        deferredHandler.geometryPass.SetUniform(
            TransformMatrices::Normal,
            transformMatrices.GetNormal()
        ) : 0;
    }
}

void Renderer::SetMaterialUniforms(const Material &mat) const
{
    for(auto it = ActiveMaterialFloat3Properties().begin();
        it != ActiveMaterialFloat3Properties().end(); ++it)
    {
        (*it) == Material::Ambient ?
        deferredHandler.geometryPass.SetUniform(
            Material::Ambient,
            mat.HasTexture(RawTexture::Ambient) ?
            mat.White : mat.ambient
        )
        : (*it) == Material::Diffuse ?
        deferredHandler.geometryPass.SetUniform(
            Material::Diffuse,
            mat.HasTexture(RawTexture::Diffuse) ?
            mat.White : mat.diffuse
        )
        : (*it) == Material::Specular ?
        deferredHandler.geometryPass.SetUniform(
            Material::Specular,
            mat.HasTexture(RawTexture::Specular) ?
            mat.White : mat.specular
        )
        : (*it) == Material::Emissive ?
        deferredHandler.geometryPass.SetUniform(
            Material::Emissive,
            mat.HasTexture(RawTexture::Emissive) ?
            mat.White : mat.emissive
        )
        : (*it) == Material::Transparent ?
        deferredHandler.geometryPass.SetUniform
        (Material::Transparent, mat.transparent) : 0;
    }

    for(auto it = ActiveMaterialFloat1Properties().begin();
        it != ActiveMaterialFloat1Properties().end(); ++it)
    {
        (*it) == Material::Opacity ?
        deferredHandler.geometryPass.SetUniform(
            Material::Opacity,
            mat.HasTexture(RawTexture::Opacity) ?
            1.0f : mat.opacity
        )
        : (*it) == Material::Shininess ?
        deferredHandler.geometryPass.SetUniform(
            Material::Shininess,
            mat.HasTexture(RawTexture::Shininess) ?
            0.0f : mat.shininess
        )
        : (*it) == Material::ShininessStrenght ?
        deferredHandler.geometryPass.SetUniform
        (Material::ShininessStrenght, mat.shininessStrenght)
        : (*it) == Material::RefractionIndex ?
        deferredHandler.geometryPass.SetUniform
        (Material::RefractionIndex, mat.refractionIndex) : 0;
    }
}

void Renderer::SetTextureUniforms() const
{
    for(auto it = ActiveSamplers().begin();
        it != ActiveSamplers().end(); ++it)
    {
        deferredHandler.geometryPass.SetUniform(*it, (int)(*it));
    }
}

void Renderer::SetTextureUniform(RawTexture::TextureType texType) const
{
    deferredHandler.geometryPass.SetUniform(texType, (int)texType);
}

const std::vector<RawTexture::TextureType>
& Renderer::ActiveSamplers() const
{
    return deferredHandler.geometryPass.ActiveSamplers();
}

const std::vector<TransformMatrices::TransformMatrixId>
& Renderer::ActiveTransformMatrices() const
{
    return deferredHandler.geometryPass.ActiveTransformMatrices();
}

const std::vector<Material::MaterialFloat3PropertyId>
& Renderer::ActiveMaterialFloat3Properties() const
{
    return deferredHandler.geometryPass.ActiveMaterialFloat3Properties();
}

const std::vector<Material::MaterialFloat1PropertyId>
& Renderer::ActiveMaterialFloat1Properties() const
{
    return deferredHandler.geometryPass.ActiveMaterialFloat1Properties();
}
