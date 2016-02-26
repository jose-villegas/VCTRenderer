#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "deferred_renderer.h"

#include "../scene/camera.h"
#include "../scene/scene.h"
#include "../scene/material.h"
#include "../scene/light.h"
#include "../types/geometry_buffer.h"
#include "../rendering/render_window.h"
#include "../programs/geometry_program.h"
#include "../programs/lighting_program.h"

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
    static auto &gbuffer = GBuffer();
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();

    if (!camera || !scene || !scene->IsLoaded()) { return; }

    // bind g buffer for writing
    gbuffer->Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    geometryProgram->Use();
    // Open GL flags
    gl.ClearDepth(1.0f);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.Enable(oglplus::Capability::CullFace);
    gl.FrontFace(oglplus::FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    // camera position or forward has changed
    viewMatrixChanged = camera->transform.changed;
    // draw whole scene tree from root node
    scene->rootNode->DrawList();
    // start light pass
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // bind g buffer for reading
    lightingProgram->Use();
    gbuffer->ActivateTextures();
    SetLightPassUniforms();
    RenderFullscreenQuad();
}

void DeferredRenderer::SetMatricesUniforms() const
{
    static const auto &node = Node::Active();
    geometryProgram->matrices.modelView
    .Set(node->ModeView());
    geometryProgram->matrices.modelViewProjection
    .Set(node->ModelViewProjection());
}

void DeferredRenderer::SetMaterialUniforms(std::shared_ptr<Material> &mat)
const
{
    using namespace oglplus;
    geometryProgram->material.diffuse.Set(mat->Diffuse());
    geometryProgram->material.specular.Set(mat->Specular());
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
    lightingProgram->gPosition.Set(GeometryBuffer::Position);
    lightingProgram->gNormal.Set(GeometryBuffer::Normal);
    lightingProgram->gAlbedo.Set(GeometryBuffer::Albedo);
    lightingProgram->gSpecular.Set(GeometryBuffer::Specular);
    // set directional lights uniforms
    auto &directionals = Light::Directionals();
    auto &points = Light::Points();
    auto &spots = Light::Spots();
    // uniform arrays of lights
    auto &uDirectionals = lightingProgram->directionalLight;
    auto &uPoints = lightingProgram->pointLight;
    auto &uSpots = lightingProgram->spotLight;

    for (int i = 0; i < directionals.size(); i++)
    {
        auto &light = directionals[i];
        auto &uLight = uDirectionals[i];
        auto &intensity = light->Intensity();

        // update view space direction-position
        if (light->transform.changed || viewMatrixChanged)
        {
            light->UpdateViewRelative(false, true);
        }

        uLight.direction.Set(light->Direction(true));
        uLight.ambient.Set(light->Ambient() * intensity.x);
        uLight.diffuse.Set(light->Diffuse() * intensity.y);
        uLight.specular.Set(light->Specular() * intensity.z);
    }

    for (int i = 0; i < points.size(); i++)
    {
        auto &light = points[i];
        auto &uLight = uPoints[i];
        auto &intensity = light->Intensity();

        // update view space direction-position
        if (light->transform.changed || viewMatrixChanged)
        {
            light->UpdateViewRelative(true, false);
        }

        uLight.position.Set(light->Position(true));
        uLight.ambient.Set(light->Ambient() * intensity.x);
        uLight.diffuse.Set(light->Diffuse() * intensity.y);
        uLight.specular.Set(light->Specular() * intensity.z);
        uLight.attenuation.constant.Set(light->attenuation.Constant());
        uLight.attenuation.linear.Set(light->attenuation.Linear());
        uLight.attenuation.quadratic.Set(light->attenuation.Quadratic());
    }

    for (int i = 0; i < spots.size(); i++)
    {
        auto &light = spots[i];
        auto &uLight = uSpots[i];
        auto &intensity = light->Intensity();

        // update view space direction-position
        if (light->transform.changed || viewMatrixChanged)
        {
            light->UpdateViewRelative(true, true);
        }

        uLight.position.Set(light->Position(true));
        uLight.direction.Set(light->Direction(true));
        uLight.ambient.Set(light->Ambient() * intensity.x);
        uLight.diffuse.Set(light->Diffuse() * intensity.y);
        uLight.specular.Set(light->Specular() * intensity.z);
        uLight.attenuation.constant.Set(light->attenuation.Constant());
        uLight.attenuation.linear.Set(light->attenuation.Linear());
        uLight.attenuation.quadratic.Set(light->attenuation.Quadratic());
        uLight.angleInnerCone.Set(cos(light->AngleInnerCone()));
        uLight.angleOuterCone.Set(cos(light->AngleOuterCone()));
    }

    // pass number of lights per type
    lightingProgram->lightTypeCount[0].Set(directionals.size());
    lightingProgram->lightTypeCount[1].Set(points.size());
    lightingProgram->lightTypeCount[2].Set(spots.size());
}
