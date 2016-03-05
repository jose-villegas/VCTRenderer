#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "voxel_renderer.h"

#include "render_window.h"
#include "../scene/scene.h"
#include "../core/assets_manager.h"
#include "../scene/texture.h"
#include "../scene/material.h"
#include "../util/const_definitions.h"

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
        BuildVoxelList();
    }

    if (framestep != 0 && frameCount % framestep  == 0)
    {
        VoxelizeScene();
    }

    // store current for next call
    previous = scene.get();
    // another frame called on render
    frameCount++;
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
    gl.Viewport(voxelDimension, voxelDimension);
    // active voxelization pass program
    CurrentProgram<VoxelizationProgram>(VoxelizationPass());
    // rendering flags
    gl.Disable(oglplus::Capability::CullFace);
    gl.Disable(oglplus::Capability::DepthTest);
    gl.ColorMask(false, false, false, false);
    UseFrustumCulling = false;
    // pass voxelization pass uniforms
    SetVoxelizationPassUniforms();
    // bind atomic buffer with voxel count
    atomicBuffer.BindBase(oglplus::BufferIndexedTarget::AtomicCounter, 0);

    // bind target textures
    if (storeMode == 1)
    {
        voxelPosition.BindImage(0, 0, false, 0, oglplus::AccessSpecifier::ReadWrite,
                                oglplus::ImageUnitFormat::RGB10_A2UI);
        voxelAlbedo.BindImage(0, 0, false, 0, oglplus::AccessSpecifier::ReadWrite,
                              oglplus::ImageUnitFormat::RGBA8);
        voxelNormal.BindImage(0, 0, false, 0, oglplus::AccessSpecifier::ReadWrite,
                              oglplus::ImageUnitFormat::RGBA16F);
    }

    scene->rootNode->DrawList();
    // recover gl and rendering state
    gl.Enable(oglplus::Capability::CullFace);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.ColorMask(true, true, true, true);
    gl.Viewport(Window().Info().width, Window().Info().height);
    UseFrustumCulling = true;
}

void VoxelRenderer::SetDimension(const unsigned int size)
{
    voxelDimension = size;
}

void VoxelRenderer::ProjectionSetup()
{
    auto ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 3.0f);
    viewProjectionMatrix[0] = ortho * lookAt(Vector3::right, Vector3::zero,
                              Vector3::up);
    viewProjectionMatrix[1] = ortho * lookAt(Vector3::up, Vector3::zero,
                              -Vector3::forward);
    viewProjectionMatrix[2] = ortho * lookAt(Vector3::forward, Vector3::zero,
                              Vector3::up);
}

void VoxelRenderer::BuildAtomicBuffer() const
{
    unsigned int initialValue = 0;
    atomicBuffer.Bind(oglplus::BufferTarget::AtomicCounter);
    atomicBuffer.Data(oglplus::BufferTarget::AtomicCounter, sizeof(unsigned int),
                      &initialValue);
    oglplus::Buffer().Bind(oglplus::BufferIndexedTarget::AtomicCounter, 0);
}

void VoxelRenderer::BuildVoxelList()
{
    using namespace oglplus;
    static Context gl;
    BuildAtomicBuffer();
    // voxelize scene first, counting mode
    storeMode = 0;
    VoxelizeScene();
    gl.MemoryBarrier(MemoryBarrierBit::AtomicCounter);
    // lookup number of voxels
    atomicBuffer.Bind(BufferTarget::AtomicCounter);
    auto access = Bitfield<BufferMapAccess>(BufferMapAccess::Read);
    access = access | Bitfield<BufferMapAccess>(BufferMapAccess::Write);
    auto count = BufferRawMap(BufferTarget::AtomicCounter, 0,
                              sizeof(unsigned int), access);
    voxelFragmentCount = *static_cast<unsigned int *>(count.RawData());
    // create buffer for the voxel fragment pass
    BuildLinearBuffer(voxelPositionTBuffer, voxelPosition,
                      PixelDataInternalFormat::R32UI,
                      sizeof(unsigned int) * voxelFragmentCount);
    BuildLinearBuffer(voxelAlbedoTBuffer, voxelAlbedo,
                      PixelDataInternalFormat::RGBA8,
                      sizeof(unsigned int) * voxelFragmentCount);
    BuildLinearBuffer(voxelNormalTBuffer, voxelNormal,
                      PixelDataInternalFormat::RGBA16F,
                      sizeof(unsigned int) * voxelFragmentCount);
    // reset counter
    memset(count.RawData(), 0, sizeof(unsigned int));
    // unmap buffer range.
    count.Unmap();
    // voxelize again, store fragments
    storeMode = 1;
    VoxelizeScene();
    gl.MemoryBarrier(MemoryBarrierBit::ShaderImageAccess);
}

void VoxelRenderer::RenderVoxel()
{
}

void VoxelRenderer::BuildLinearBuffer(const oglplus::Buffer &buffer,
                                      const oglplus::Texture &texture, const oglplus::PixelDataInternalFormat &format,
                                      size_t size)
{
    auto empty = 0;
    buffer.Bind(oglplus::BufferTarget::Texture);
    buffer.Data(oglplus::BufferTarget::Texture, oglplus::BufferData(size, nullptr));
    texture.Bind(oglplus::TextureTarget::Buffer);
    texture.Buffer(oglplus::TextureTarget::Buffer, format, buffer);
}

VoxelRenderer::VoxelRenderer(RenderWindow * window) : Renderer(window)
{
    renderVoxel = false;
    framestep = 0; // no dynamic update
    voxelDimension = 128;
    ProjectionSetup();
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
    prog.cellSize[0].Set(voxelDimension);
    prog.cellSize[1].Set(voxelDimension);
    prog.storeMode.Set(storeMode);
}
