#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "deferred_renderer.h"

#include "deferred_handler.h"
#include "../scene/camera.h"
#include "../scene/scene.h"
#include "../scene/material.h"
#include "../scene/light.h"
#include "../types/geometry_buffer.h"
#include "../rendering/render_window.h"
#include "../programs/geometry_program.h"
#include "../programs/lighting_program.h"

DeferredRenderer::DeferredRenderer(RenderWindow &window) : Renderer(window),
    viewMatrixChanged(false)
{
    // create textures and attachments for framebuffer in deferredhandler
    SetupGeometryBuffer(window.Info().width, window.Info().height);
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
    gbuffer.Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // activate geometry pass shader program
    CurrentProgram<GeometryProgram>(GeometryPass());
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
    CurrentProgram<LightingProgram>(LightingPass());
    // bind g buffer textures for reading
    gbuffer.ActivateTextures();
    // pass light info and texture locations for final light pass
    SetLightPassUniforms();
    // draw the result onto a fullscreen quad
    DrawFullscreenQuad();
}

void DeferredRenderer::SetMatricesUniforms(const Node &node) const
{
    auto &prog = CurrentProgram<GeometryProgram>();
    prog.matrices.modelView.Set(node.ModeView());
    prog.matrices.modelViewProjection.Set(node.ModelViewProjection());
}

void DeferredRenderer::SetMaterialUniforms(const Material &material)
const
{
    using namespace oglplus;
    auto &prog = CurrentProgram<GeometryProgram>();
    prog.material.diffuse.Set(material.Diffuse());
    prog.material.specular.Set(material.Specular());
    prog.material.shininess.Set(material.Shininess());
    prog.material.useNormalsMap.Set(material.HasTexture(RawTexture::Normals));
    // set textures
    Texture::Active(RawTexture::Diffuse);
    material.BindTexture(RawTexture::Diffuse);
    prog.diffuseMap.Set(RawTexture::Diffuse);
    Texture::Active(RawTexture::Specular);
    material.BindTexture(RawTexture::Specular);
    prog.specularMap.Set(RawTexture::Specular);
    Texture::Active(RawTexture::Normals);
    material.BindTexture(RawTexture::Normals);
    prog.normalsMap.Set(RawTexture::Normals);
}

void DeferredRenderer::SetLightPassUniforms() const
{
    static auto &camera = Camera::Active();
    auto &prog = CurrentProgram<LightingProgram>();;
    prog.inverseProjection.Set(camera->InverseProjectionMatrix());
    prog.gDepth.Set(GeometryBuffer::Depth);
    prog.gNormal.Set(GeometryBuffer::Normal);
    prog.gAlbedo.Set(GeometryBuffer::Albedo);
    prog.gSpecular.Set(GeometryBuffer::Specular);
    // set directional lights uniforms
    auto &directionals = Light::Directionals();
    auto &points = Light::Points();
    auto &spots = Light::Spots();
    // uniform arrays of lights
    auto &uDirectionals = prog.directionalLight;
    auto &uPoints = prog.pointLight;
    auto &uSpots = prog.spotLight;

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
    prog.lightTypeCount[0].Set(directionals.size());
    prog.lightTypeCount[1].Set(points.size());
    prog.lightTypeCount[2].Set(spots.size());
}
