#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "voxelizer_renderer.h"
#include "shadow_map_renderer.h"
#include "../scene/scene.h"
#include "../scene/light.h"
#include "../core/assets_manager.h"
#include "../scene/camera.h"
#include "../scene/texture.h"
#include "../scene/material.h"
#include "../rendering/render_window.h"

#include "../programs/voxelization_program.h"
#include "../programs/voxel_drawer_program.h"
#include "../programs/radiance_program.h"
#include "../programs/mipmapping_program.h"

#include <oglplus/context.hpp>
#include <oglplus/framebuffer.hpp>
#include <glm/gtx/transform.hpp>

bool VoxelizerRenderer::ShowVoxels = false;

void VoxelizerRenderer::Render()
{
    // negative framestep means this is disabled
    if (framestep < 0) { return; }

    static Scene * previous = nullptr;
    static auto frameCount = 1;
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

    // another frame called on render, if framestep is > 0
    // Voxelization will happen every framestep frame
    if (framestep != 0 && frameCount++ % framestep == 0)
    {
        frameCount = 1;
        // update voxelization
        VoxelizeScene();
    }

    if (ShowVoxels)
    {
        DrawVoxels();
    }
}

void VoxelizerRenderer::SetMatricesUniforms(const Node &node) const
{
    // no space matrices for voxelization pass during node rendering
    auto &prog = CurrentProgram<VoxelizationProgram>();
    prog.matrices.model.Set(node.transform.Matrix());
    prog.matrices.normal.Set(node.InverseTranspose());
}

void VoxelizerRenderer::SetMaterialUniforms(const Material &material) const
{
    using namespace oglplus;
    auto &prog = CurrentProgram<VoxelizationProgram>();
    prog.material.diffuse.Set(material.Diffuse());
    // set textures
    Texture::Active(RawTexture::Diffuse);
    material.BindTexture(RawTexture::Diffuse);
    prog.diffuseMap.Set(RawTexture::Diffuse);
    Texture::Active(RawTexture::Specular);
    material.BindTexture(RawTexture::Specular);
}

void VoxelizerRenderer::SetUpdateFrequency(const unsigned int framestep)
{
    this->framestep = framestep;
}

void VoxelizerRenderer::VoxelizeScene()
{
    static oglplus::Context gl;
    static auto &scene = Scene::Active();
    static float zero[] = { 0, 0, 0, 0 };
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);

    if (!scene || !scene->IsLoaded()) { return; }

    auto &prog = VoxelizationPass();
    auto &sceneBox = scene->rootNode->boundaries;
    // current renderer as active
    SetAsActive();
    // unbind fbos use default
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    // clear and setup viewport
    gl.ColorMask(false, false, false, false);
    gl.Viewport(volumeDimension, volumeDimension);
    gl.Clear().ColorBuffer().DepthBuffer();
    // active voxelization pass program
    CurrentProgram<VoxelizationProgram>(prog);
    // rendering flags
    gl.Disable(oglplus::Capability::CullFace);
    gl.Disable(oglplus::Capability::DepthTest);
    UseFrustumCulling = false;
    // pass voxelization uniforms
    prog.viewProjections[0].Set(viewProjectionMatrix[0]);
    prog.viewProjections[1].Set(viewProjectionMatrix[1]);
    prog.viewProjections[2].Set(viewProjectionMatrix[2]);
    prog.volumeDimension.Set(volumeDimension);
    prog.worldMinPoint.Set(sceneBox.MinPoint());
    prog.voxelScale.Set(1.0f / volumeGridSize);
    // bind the volume texture to be writen in shaders
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                       oglplus::ImageUnitFormat::R32UI);
    voxelTex.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    voxelNormal.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    // draw scene triangles
    scene->rootNode->DrawList();
    // sync barrier
    gl.MemoryBarrier(shImage | texFetch);
    // compute shader injects diffuse lighting and shadowing
    InjectRadiance();
    // generate mipmap levels per face for anisotropic mipmapping
    GenerateMipmap();
}

void VoxelizerRenderer::InjectRadiance()
{
    static oglplus::Context gl;
    static auto &scene = Scene::Active();
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);
    static auto &prog = InjectRadianceShader();
    static auto &shadowing = *static_cast<ShadowMapRenderer *>
                             (AssetsManager::Instance()
                              ->renderers["Shadowmapping"].get());
    // control vars
    auto &sceneBox = scene->rootNode->boundaries;
    // inject radiance into voxel texture and also mip-map
    CurrentProgram<InjectRadianceProgram>(prog);
    // pass compute shader uniform
    prog.lightViewProjection.Set(shadowing.LightSpaceMatrix());
    prog.lightBleedingReduction.Set(shadowing.LightBleedingReduction());
    prog.voxelSize.Set(voxelSize);
    prog.worldMinPoint.Set(sceneBox.MinPoint());

    // shadow casting
    if (shadowing.Caster() != nullptr)
    {
        auto &caster = *shadowing.Caster();
        prog.directionalLight.diffuse.Set(caster.Diffuse() * caster.Intensities().y);
        prog.directionalLight.direction.Set(caster.Direction());
        prog.shadowMapping.Set(1);
    }
    else
    {
        prog.shadowMapping.Set(0);
    }

    // pass uniform texture for shadowing
    shadowing.BindReading(6);
    prog.shadowMap.Set(6);
    prog.exponents.Set(shadowing.Exponents());
    // voxel texture to read
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                       oglplus::ImageUnitFormat::R32UI);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                          oglplus::ImageUnitFormat::R32UI);
    voxelTex.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                       oglplus::ImageUnitFormat::RGBA8);
    auto workGroups = static_cast<unsigned>(glm::ceil(volumeDimension / 8.0f));
    // inject radiance at level 0 of texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // mipmap radiance resulting volume
    gl.MemoryBarrier(shImage | texFetch);
}

void VoxelizerRenderer::GenerateMipmap()
{
    static oglplus::Context gl;
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);
    static auto &baseProg = MipMappingBaseShader();
    static auto &volumeProg = MipMappingVolumeShader();
    auto halfDimension = volumeDimension / 2;
    // base volume mipmap from voxel tex to first mip level
    CurrentProgram<MipmappingBaseProgram>(baseProg);
    // bind images for reading / writing
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                       oglplus::ImageUnitFormat::RGBA8);
    voxelTexMipmap.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                             oglplus::ImageUnitFormat::RGBA8);
    auto workGroups = static_cast<unsigned>(glm::ceil(halfDimension / 8.0f));
    // mipmap from base texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // mipmap radiance resulting volume
    gl.MemoryBarrier(shImage | texFetch);
    // from first mip level to the rest aniso mipmapping
    CurrentProgram<MipmappingVolumeProgram>(volumeProg);
    // bind source texture
    voxelTexMipmap.Active(0);
    voxelTexMipmap.Bind(oglplus::TextureTarget::_3D);
    // setup current mip level uniforms
    auto mipDimension = halfDimension / 2;
    auto mipLevel = 0;

    while(mipDimension >= 1)
    {
        volumeProg.mipDimension.Set(mipDimension);
        volumeProg.mipLevel.Set(mipLevel);
        // bind for writing at mip level
        voxelTexMipmap.BindImage(1, mipLevel + 1, true, 0,
                                 oglplus::AccessSpecifier::WriteOnly,
                                 oglplus::ImageUnitFormat::RGBA8);
        workGroups = static_cast<unsigned>(glm::ceil(mipDimension / 8.0f));
        // mipmap from mip level
        gl.DispatchCompute(workGroups, workGroups, workGroups);
        // mipmap radiance resulting volume
        gl.MemoryBarrier(shImage | texFetch);
        // down a mip level
        mipLevel++;
        mipDimension /= 2;
    }
}
void VoxelizerRenderer::DrawVoxels()
{
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();
    static auto &info = Window().Info();

    if (!camera || !scene || !scene->IsLoaded()) { return; }

    static oglplus::Context gl;
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    gl.ColorMask(true, true, true, true);
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl.Viewport(info.width, info.height);
    gl.Clear().ColorBuffer().DepthBuffer();
    // Open GL flags
    gl.ClearDepth(1.0f);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.Enable(oglplus::Capability::CullFace);
    gl.FrontFace(oglplus::FaceOrientation::CCW);
    auto &prog = VoxelDrawerShader();
    CurrentProgram<VoxelDrawerProgram>(prog);
    // voxel grid projection matrices
    auto &sceneBox = scene->rootNode->boundaries;
    auto &viewProjection = camera->ViewProjectionMatrix();
    auto vDimension = static_cast<unsigned>(volumeDimension / pow(2.0f,
                                            drawMipLevel));
    auto vSize = volumeGridSize / vDimension;

    // pass voxel drawer uniforms
    if(drawMipLevel == 0)
    {
        voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                           oglplus::ImageUnitFormat::RGBA8);
        prog.direction.Set(0);
    }
    else
    {
        voxelTexMipmap.BindImage(0, drawMipLevel - 1, true, 0,
                                 oglplus::AccessSpecifier::ReadOnly,
                                 oglplus::ImageUnitFormat::RGBA8);
        prog.direction.Set(drawDirection);
    }

    auto model = translate(sceneBox.MinPoint()) * scale(glm::vec3(vSize));
    prog.volumeDimension.Set(vDimension);
    prog.matrices.modelViewProjection.Set(viewProjection * model);
    // bind vertex buffer array to draw, needed but all geometry is generated
    // in the geometry shader
    voxelDrawerArray.Bind();
    gl.DrawArrays(oglplus::PrimitiveType::Points, 0, voxelCount);
}
void VoxelizerRenderer::UpdateProjectionMatrices(const BoundingBox &sceneBox)
{
    auto axisSize = sceneBox.Extent() * 2.0f;
    auto &center = sceneBox.Center();
    volumeGridSize = glm::max(axisSize.x, glm::max(axisSize.y, axisSize.z));
    voxelSize = volumeGridSize / volumeDimension;
    auto halfSize = volumeGridSize / 2.0f;
    auto projection = glm::ortho(-halfSize, halfSize, -halfSize, halfSize, 0.0f,
                                 volumeGridSize);
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
VoxelizerRenderer::VoxelizerRenderer(RenderWindow &window) : Renderer(window)
{
    drawMipLevel = drawDirection = 0;
    framestep = 5; // only on scene change
    SetupVoxelVolumes(256);
}
void VoxelizerRenderer::SetupVoxelVolumes(const unsigned int &dimension)
{
    using namespace oglplus;
    static Context gl;
    static float zero[] = { 0, 0, 0, 0 };
    volumeDimension = dimension;
    voxelCount = volumeDimension * volumeDimension * volumeDimension;
    voxelSize = volumeGridSize / volumeDimension;

    // update projection matrices on new dimension
    if(Scene::Active())
    {
        UpdateProjectionMatrices(Scene::Active()->rootNode->boundaries);
    }

    auto maxLevel = static_cast<unsigned int>(log2(volumeDimension));
    // generate albedo volume
    voxelTex.Bind(TextureTarget::_3D);
    voxelTex.MinFilter(TextureTarget::_3D, TextureMinFilter::Linear);
    voxelTex.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelTex.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTex.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTex.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTex.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                     dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                     PixelDataType::UnsignedByte, nullptr);
    voxelTex.ClearImage(0, PixelDataFormat::RGBA, zero);
    // generate normal volume for radiance
    voxelNormal.Bind(TextureTarget::_3D);
    voxelNormal.MinFilter(TextureTarget::_3D, TextureMinFilter::Linear);
    voxelNormal.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelNormal.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelNormal.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelNormal.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelNormal.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                        dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                        PixelDataType::UnsignedByte, nullptr);;
    voxelNormal.ClearImage(0, PixelDataFormat::RGBA, zero);
    // mip mapping textures per face
    voxelTexMipmap.Bind(TextureTarget::_3D);
    voxelTexMipmap.MinFilter(TextureTarget::_3D,
                             TextureMinFilter::LinearMipmapLinear);
    voxelTexMipmap.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelTexMipmap.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTexMipmap.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTexMipmap.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTexMipmap.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                           dimension * 3, dimension / 2, dimension / 2, 0,
                           PixelDataFormat::RGBA, PixelDataType::UnsignedByte, nullptr);
    voxelTexMipmap.ClearImage(0, PixelDataFormat::RGBA, zero);
    voxelTexMipmap.GenerateMipmap(TextureTarget::_3D);
}
void VoxelizerRenderer::RevoxelizeScene()
{
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    UpdateProjectionMatrices(scene->rootNode->boundaries);
    // update voxelization
    VoxelizeScene();
}
void VoxelizerRenderer::SetupDrawVoxels(const unsigned &level,
                                        const unsigned &direction)
{
    drawMipLevel = level;
    drawDirection = direction;
}
VoxelizerRenderer::~VoxelizerRenderer()
{
}

const unsigned &VoxelizerRenderer::VolumeDimension() const
{
    return volumeDimension;
}
oglplus::Texture &VoxelizerRenderer::VoxelTexture()
{
    return voxelTex;
}

oglplus::Texture &VoxelizerRenderer::VoxelTextureMipmap()
{
    return voxelTexMipmap;
}

const float &VoxelizerRenderer::VoxelWorldSize() const
{
    return voxelSize;
}

const float &VoxelizerRenderer::VolumeGridSize() const
{
    return volumeGridSize;
}

VoxelizationProgram &VoxelizerRenderer::VoxelizationPass()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<VoxelizationProgram *>
                        (assets->programs["Voxelization"].get());
    return prog;
}
VoxelDrawerProgram &VoxelizerRenderer::VoxelDrawerShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<VoxelDrawerProgram *>
                        (assets->programs["VoxelDrawer"].get());
    return prog;
}
InjectRadianceProgram &VoxelizerRenderer::InjectRadianceShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<InjectRadianceProgram *>
                        (assets->programs["InjectRadiance"].get());
    return prog;
}
MipmappingBaseProgram &VoxelizerRenderer::MipMappingBaseShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<MipmappingBaseProgram *>
                        (assets->programs["MipmappingBase"].get());
    return prog;
}
MipmappingVolumeProgram &VoxelizerRenderer::MipMappingVolumeShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<MipmappingVolumeProgram *>
                        (assets->programs["MipmappingVolume"].get());
    return prog;
}