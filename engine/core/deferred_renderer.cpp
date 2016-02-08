#include "deferred_renderer.h"
#include "geometry_program.h"
#include "lighting_program.h"

#include "render_window.h"
#include "../scene/camera.h"
#include "../scene/scene.h"
#include "../scene/material.h"

DeferredRenderer::DeferredRenderer(const RenderWindow &rWindow) :
    DeferredHandler(rWindow.Info().width, rWindow.Info().height)
{
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::Render()
{
    static oglplus::Context gl;
    auto &camera = Camera::Active();
    auto &scene = Scene::Active();

    if (!camera || !scene) { return; }

    // bind g buffer for writing
    geometryBuffer.Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    geometryProgram->Use();
    // Open GL flags
    gl.ClearDepth(1.0f);
    //gl.Disable(Capability::Blend);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.Enable(oglplus::Capability::CullFace);
    gl.FrontFace(oglplus::FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // play with camera parameters - testing
    camera->Position(glm::vec3(0.0f, 0.50f, 0.0f));
    camera->LookAt(glm::vec3(
                       sin(glfwGetTime() * 0.5f),
                       0.50f,
                       cos(glfwGetTime()  * 0.5f)
                   ));
    // draw whole scene hierarchy tree from root node
    scene->rootNode.DrawList();
    // start light pass
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // bind g buffer for reading
    lightingProgram->Use();
    geometryBuffer.ActivateTextures();
    SetLightPassUniforms();
    RenderFullscreenQuad();
}

void DeferredRenderer::SetMatricesUniforms() const
{
    auto &node = Node::Active();
    geometryProgram->matrices.normal.Set(node->NormalMatrix());
    geometryProgram->matrices.modelView.Set(node->ModelViewMatrix());
    geometryProgram->matrices.modelViewProjection.Set(
        node->ModelViewProjectionMatrix());
}

void DeferredRenderer::SetMaterialUniforms(std::shared_ptr<OGLMaterial> &mat)
const
{
    using namespace oglplus;
    static OGLMaterial * previousMaterial = nullptr;
    static glm::vec3 white = glm::vec3(1.0f);

    if (previousMaterial == mat.get())
    {
        return;
    }

    previousMaterial = mat.get();
    geometryProgram->material.diffuse.Set(mat->Diffuse());
    geometryProgram->material.specular.Set
    (
        mat->HasTexture(RawTexture::Specular)
        ? white
        : mat->Specular()
    );
    geometryProgram->material.useNormalsMap.Set
    (
        mat->HasTexture(RawTexture::Normals)
    );
    // set textures
    Texture::Active(RawTexture::Diffuse);
    mat->BindTexture(RawTexture::Diffuse);
    geometryProgram->diffuseMap.Set(RawTexture::Diffuse);
    Texture::Active(RawTexture::Specular);
    mat->BindTexture(RawTexture::Specular);
    geometryProgram->specularMap.Set(RawTexture::Specular);
    Texture::Active(RawTexture::Normals);
    mat->BindTexture(RawTexture::Normals);
    geometryProgram->normalsMap.Set(RawTexture::Normals);
}

void DeferredRenderer::SetLightPassUniforms() const
{
    lightingProgram->viewPosition.Set(Camera::Active()->Position());
    lightingProgram->gPosition.Set(GeometryBuffer::Position);
    lightingProgram->gNormal.Set(GeometryBuffer::Normal);
    lightingProgram->gAlbedo.Set(GeometryBuffer::Albedo);
    lightingProgram->gSpecular.Set(GeometryBuffer::Specular);
}
