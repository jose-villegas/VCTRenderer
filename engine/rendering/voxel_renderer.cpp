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
#include <oglplus/framebuffer.hpp>
#include <glm/gtc/matrix_transform.inl>

void VoxelRenderer::Render()
{
    VoxelizeScene();
}

void VoxelRenderer::VoxelizeScene()
{
    static oglplus::Context gl;
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.Clear().ColorBuffer().DepthBuffer();
    // active voxelization pass program
    CurrentProgram<VoxelizationProgram>(VoxelizationPass());
    // rendering flags
    gl.Disable(oglplus::Capability::CullFace);
    gl.Disable(oglplus::Capability::DepthTest);
    gl.ColorMask(false, false, false, false);
    // pass voxelization pass uniforms
    SetVoxelizationPassUniforms();
    // bind atomic buffer with voxel count
    atomicBuffer.BindBase(oglplus::BufferIndexedTarget::AtomicCounter, 0);
    // bind target textures
    oglplus::Texture::BindImage(0, voxelPosition, 0, false, 0,
                                oglplus::AccessSpecifier::ReadWrite,
                                oglplus::ImageUnitFormat::RGB10_A2UI);
    oglplus::Texture::BindImage(1, voxelAlbedo, 0, false, 0,
                                oglplus::AccessSpecifier::ReadWrite,
                                oglplus::ImageUnitFormat::RGBA8);
    oglplus::Texture::BindImage(2, voxelNormal, 0, false, 0,
                                oglplus::AccessSpecifier::ReadWrite,
                                oglplus::ImageUnitFormat::RGBA16F);
    // draw whole scene tree from root node
    scene->rootNode->DrawList();
    // recover gl state
    gl.Enable(oglplus::Capability::CullFace);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.ColorMask(true, true, true, true);
    gl.Viewport(Window().Info().width, Window().Info().height);
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
    static oglplus::Context gl;
    BuildAtomicBuffer();
    // voxelize scene first
    VoxelizeScene();
    gl.MemoryBarrier(oglplus::MemoryBarrierBit::AtomicCounter);
    // lookup number of voxels
    atomicBuffer.Bind(oglplus::BufferTarget::AtomicCounter);
    auto voxelCount = voxelDimension * voxelDimension * voxelDimension;
    // create buffer for the voxel fragment pass
    BuildLinearBuffer(voxelPositionTBuffer, voxelPosition,
                      oglplus::PixelDataInternalFormat::R32UI,
                      sizeof(unsigned int) * voxelCount);
    BuildLinearBuffer(voxelAlbedoTBuffer, voxelAlbedo,
                      oglplus::PixelDataInternalFormat::RGBA8,
                      sizeof(unsigned int) * voxelCount);
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
    voxelDimension = 128;
    ProjectionSetup();
    BuildVoxelList();
}

VoxelRenderer::~VoxelRenderer()
{
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
}
