#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_renderer.h"

#include "render_window.h"
#include "../scene/scene.h"
#include "../core/assets_manager.h"
#include "../scene/texture.h"
#include "../scene/material.h"

#include "../programs/voxelization_program.h"

#include <oglplus/context.hpp>
#include <glm/gtc/matrix_transform.hpp>

void VoxelRenderer::Render()
{
    static Scene * previous = nullptr;
    static auto frameCount = 0;
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    // scene changed or loaded
    if (previous != scene.get())
    {
        // update voxelization
        VoxelizeScene();;
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

void VoxelRenderer::ProjectionSetup()
{
    auto size = volumeGridSize;
    auto ortho = glm::ortho(-(size * 0.5f), size * 0.5f, -(size * 0.5f),
                            size * 0.5f, size * 0.5f, size * 1.5f);
    viewProjectionMatrix[0] = ortho * lookAt(glm::vec3(size, 0, 0), glm::vec3(0, 0,
                              0), glm::vec3(0, 1, 0));
    viewProjectionMatrix[1] = ortho * lookAt(glm::vec3(0, size, 0), glm::vec3(0, 0,
                              0), glm::vec3(0, 0, -1));
    viewProjectionMatrix[2] = ortho * lookAt(glm::vec3(0, 0, size), glm::vec3(0, 0,
                              0), glm::vec3(0, 1, 0));
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
    framestep = 0; // no dynamic update
    volumeDimension = 128;
    volumeGridSize = 2.0f;
    ProjectionSetup();
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

void VoxelRenderer::SetVoxelizationPassUniforms() const
{
    auto &prog = CurrentProgram<VoxelizationProgram>();
    prog.viewProjections[0].Set(viewProjectionMatrix[0]);
    prog.viewProjections[1].Set(viewProjectionMatrix[1]);
    prog.viewProjections[2].Set(viewProjectionMatrix[2]);
    prog.volumeDimension.Set(volumeDimension);
}
