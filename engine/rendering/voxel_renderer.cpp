#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_renderer.h"

#include "render_window.h"
#include "../scene/scene.h"
#include "../core/assets_manager.h"
#include "../scene/camera.h"
#include "../scene/texture.h"
#include "../scene/material.h"

#include "../programs/voxelization_program.h"
#include "../programs/voxel_drawer_program.h"

#include <oglplus/context.hpp>
#include <glm/gtx/transform.hpp>

void VoxelRenderer::Render()
{
    static Scene * previous = nullptr;
    static auto frameCount = 0;
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    // scene changed or loaded
    if (previous != scene.get())
    {
        UpdateProjectionMatrices(scene->rootNode->boundaries);
        // update voxelization
        VoxelizeScene();
    }

    // store current for next call
    previous = scene.get();

    // another frame called on render
    if (framestep == 0 || frameCount++ % framestep != 0)
    {
        return;
    }

    // update voxelization
    VoxelizeScene();
}

void VoxelRenderer::SetMatricesUniforms(const Node &node) const
{
    // no space matrices for voxelization pass during node rendering
}

void VoxelRenderer::SetMaterialUniforms(const Material &material) const
{
    auto &prog = CurrentProgram<VoxelizationProgram>();
    // set textures
    oglplus::Texture::Active(RawTexture::Diffuse);
    material.BindTexture(RawTexture::Diffuse);
    prog.diffuseMap.Set(RawTexture::Diffuse);
}

void VoxelRenderer::SetUpdateFrequency(const unsigned int framestep)
{
    this->framestep = framestep;
}

void VoxelRenderer::VoxelizeScene()
{
    static oglplus::Context gl;
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    gl.Clear().ColorBuffer().DepthBuffer();
    gl.Viewport(volumeDimension, volumeDimension);
    // active voxelization pass program
    CurrentProgram<VoxelizationProgram>(VoxelizationPass());
    // rendering flags
    gl.Disable(oglplus::Capability::CullFace);
    gl.Disable(oglplus::Capability::DepthTest);
    gl.ColorMask(false, false, false, false);
    UseFrustumCulling = false;
    // pass voxelization pass uniforms
    SetVoxelizationPassUniforms();
    // bind the volume texture to be writen in shaders
    voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                          oglplus::ImageUnitFormat::RGBA8);
    // draw scene triangles
    scene->rootNode->DrawList();
    // recover gl and rendering state
    gl.Enable(oglplus::Capability::CullFace);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.ColorMask(true, true, true, true);
    gl.Viewport(Window().Info().width, Window().Info().height);
    UseFrustumCulling = true;
}

void VoxelRenderer::DrawVoxels()
{
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();

    if (!camera || !scene || !scene->IsLoaded()) { return; }

    static oglplus::Context gl;
    gl.ClearColor(0, 0, 0, 1);
    gl.Clear().ColorBuffer().DepthBuffer();
    // Open GL flags
    gl.Enable(oglplus::Capability::DepthTest);
    gl.Enable(oglplus::Capability::CullFace);
    // voxel volume info
    auto voxelCount = volumeDimension * volumeDimension * volumeDimension;
    auto voxelSize = volumeGridSize / volumeDimension;
    auto &prog = VoxelDrawerShader();
    CurrentProgram<VoxelDrawerProgram>(prog);
    // activate voxel albedo texture for reading
    voxelAlbedo.Active(0);
    voxelAlbedo.Bind(oglplus::TextureTarget::_3D);
    // whole voxel space
    auto voxelModel = translate(glm::vec3(0)) * glm::mat4(1.0f)
                      * scale(glm::vec3(voxelSize));
    auto &viewMatrix = camera->ViewMatrix();
    auto &projectionMatrix = camera->ProjectionMatrix();
    // pass voxel drawer uniforms
    prog.voxelAlbedo.Set(0);
    prog.volumeDimension.Set(volumeDimension);
    prog.matrices.modelViewProjection.Set(projectionMatrix * viewMatrix *
                                          voxelModel);
    // bind vertex buffer array to draw, needed but all geometry is generated
    // in the geometry shader
    voxelDrawerArray.Bind();
    gl.DrawArrays(oglplus::PrimitiveType::Points, 0, voxelCount);
}

void VoxelRenderer::UpdateProjectionMatrices(const BoundingBox &sceneBox)
{
    auto &center = sceneBox.Center();
    auto diagonal = sceneBox.MaxPoint() - sceneBox.MinPoint();
    auto maxSize = glm::max(diagonal.x, glm::max(diagonal.y, diagonal.z));
    auto halfSize = maxSize / 2.0f;
    auto projection = glm::ortho(-halfSize, halfSize, -halfSize, halfSize, 0.0f,
                                 maxSize);
    viewProjectionMatrix[0] = lookAt(center + glm::vec3(halfSize, 0.0f, 0.0f),
                                     center, glm::vec3(0.0f, 1.0f, 0.0f));
    viewProjectionMatrix[1] = lookAt(center + glm::vec3(0.0f, halfSize, 0.0f),
                                     center, glm::vec3(0.0f, 0.0f, -1.0f));
    viewProjectionMatrix[2] = lookAt(center + glm::vec3(0.0f, 0.0f, halfSize),
                                     center, glm::vec3(0.0f, 1.0f, 0.0f));

    for (auto &matrix : viewProjectionMatrix)
    {
        matrix = projection * matrix;
    }
}

void VoxelRenderer::GenerateVolumes() const
{
    using namespace oglplus;
    auto voxelData = new float[volumeDimension * volumeDimension * volumeDimension];
    voxelAlbedo.Bind(TextureTarget::_3D);
    voxelAlbedo.BaseLevel(TextureTarget::_3D, 0);
    voxelAlbedo.MaxLevel(TextureTarget::_3D, 0);
    voxelAlbedo.MinFilter(TextureTarget::_3D, TextureMinFilter::Nearest);
    voxelAlbedo.MagFilter(TextureTarget::_3D, TextureMagFilter::Nearest);
    voxelAlbedo.Image3D(TextureTarget::_3D, 0,
                        PixelDataInternalFormat::RGBA8,
                        volumeDimension, volumeDimension, volumeDimension, 0,
                        PixelDataFormat::RGBA, PixelDataType::UnsignedByte,
                        voxelData);
    voxelAlbedo.GenerateMipmap(TextureTarget::_3D);
    delete[] voxelData;
}

VoxelRenderer::VoxelRenderer(RenderWindow * window) : Renderer(window)
{
    renderVoxel = false;
    framestep = 0;
    volumeDimension = 128;
    volumeGridSize = 2;
    GenerateVolumes();
}

VoxelRenderer::~VoxelRenderer()
{
}

VoxelizationProgram &VoxelRenderer::VoxelizationPass()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<VoxelizationProgram *>
                        (assets->programs[AssetsManager::Voxelization].get());
    return prog;
}

VoxelDrawerProgram &VoxelRenderer::VoxelDrawerShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<VoxelDrawerProgram *>
                        (assets->programs[AssetsManager::VoxelDrawer].get());
    return prog;
}

void VoxelRenderer::SetVoxelizationPassUniforms() const
{
    auto &prog = CurrentProgram<VoxelizationProgram>();
    prog.viewProjections[0].Set(viewProjectionMatrix[0]);
    prog.viewProjections[1].Set(viewProjectionMatrix[1]);
    prog.viewProjections[2].Set(viewProjectionMatrix[2]);
    prog.volumeDimension.Set(volumeDimension);
}
