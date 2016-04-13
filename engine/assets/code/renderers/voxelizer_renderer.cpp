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
#include "../programs/propagation_program.h"
#include "deferred_renderer.h"

bool VoxelizerRenderer::ShowVoxels = false;

void VoxelizerRenderer::Render()
{
    static Scene * previous = nullptr;
    static auto frameCount = 1;
    static auto &scene = Scene::Active();
    static auto &camera = Camera::Active();

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
    static auto &changes = Transform::TransformChangedMap();
    static auto revoxelize = true;

    if (framestep == -1)
    {
        for (auto &c : changes)
        {
            if (c.first != camera.get())
            {
                revoxelize = true;
                break;
            }
        }

        if(revoxelize)
        {
            // update voxelization
            VoxelizeScene();
        }
    }
    // voxelization will happen every framestep frame
    else if (framestep >= 1 && frameCount++ % framestep == 0)
    {
        frameCount = 1;
        // update voxelization
        VoxelizeScene();
    }

    if (ShowVoxels)
    {
        DrawVoxels();
    }

    revoxelize = false;
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
    prog.material.emissive.Set(material.Emissive());
    // set textures
    Texture::Active(RawTexture::Diffuse);
    material.BindTexture(RawTexture::Diffuse);
    prog.diffuseMap.Set(RawTexture::Diffuse);
    Texture::Active(RawTexture::Specular);
    material.BindTexture(RawTexture::Specular);
}

void VoxelizerRenderer::SetUpdateFrequency(const int framestep)
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
    prog.viewProjectionsI[0].Set(viewProjectionMatrixI[0]);
    prog.viewProjectionsI[1].Set(viewProjectionMatrixI[1]);
    prog.viewProjectionsI[2].Set(viewProjectionMatrixI[2]);
    prog.volumeDimension.Set(volumeDimension);
    prog.worldMinPoint.Set(sceneBox.MinPoint());
    prog.voxelScale.Set(1.0f / volumeGridSize);
    // clear images before voxelization
    voxelAlbedo.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelNormal.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelRadiance.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    // bind the volume texture to be writen in shaders
    voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    voxelRadiance.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                            oglplus::ImageUnitFormat::RGBA8);
    // draw scene triangles
    scene->rootNode->DrawList();
    // sync barrier
    gl.MemoryBarrier(shImage | texFetch);
    // compute shader injects diffuse lighting and shadowing
    InjectRadiance();
    // finally generate mip map values (and light propagation if needed)
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
    // inject radiance into voxel texture
    CurrentProgram<InjectRadianceProgram>(prog);
    // pass compute shader uniform

    if (shadowing.Caster() != nullptr)
    {
        // pass uniform texture for shadowing
        shadowing.BindReading(6);
        prog.shadowMap.Set(6);
        prog.exponents.Set(shadowing.Exponents());
        prog.shadowMapping.Set(1);
    }
    else
    {
        prog.shadowMapping.Set(0);
    }

    // uniform arrays of lights
    auto &uDirectionals = prog.directionalLight;
    auto &uPoints = prog.pointLight;
    auto &uSpots = prog.spotLight;
    auto &lights = scene->lights;
    // index of directional-point-spot lights
    auto typeIndex = glm::uvec3(0);
    // pass number of lights per type
    prog.lightTypeCount[0].Set(static_cast<const unsigned int>
                               (Light::Directionals().size()));
    prog.lightTypeCount[1].Set(static_cast<const unsigned int>
                               (Light::Points().size()));
    prog.lightTypeCount[2].Set(static_cast<const unsigned int>
                               (Light::Spots().size()));

    for (int i = 0; i < lights.size(); ++i)
    {
        auto &light = lights[i];
        auto &factor = light->Intensities();
        auto shadowingMethod = light->mode[0].to_ulong();
        // current light uniform
        auto &uLight = light->Type() == Light::Directional
                       ? uDirectionals[typeIndex.x++]
                       : light->Type() == Light::Point
                       ? uPoints[typeIndex.y++]
                       : uSpots[typeIndex.z++];
        // shared uniforms between types
        uLight.diffuse.Set(light->Diffuse() * factor.y);

        if (light->Type() == Light::Spot || light->Type() == Light::Point)
        {
            uLight.position.Set(light->Position());
        }

        if (light->Type() == Light::Spot || light->Type() == Light::Directional)
        {
            uLight.direction.Set(light->Direction());
        }

        if (light->Type() == Light::Spot || light->Type() == Light::Point)
        {
            uLight.attenuation.constant.Set(light->attenuation.Constant());
            uLight.attenuation.linear.Set(light->attenuation.Linear());
            uLight.attenuation.quadratic.Set(light->attenuation.Quadratic());
        }

        if (light->Type() == Light::Spot)
        {
            uLight.angleInnerCone.Set(cos(light->AngleInnerCone()));
            uLight.angleOuterCone.Set(cos(light->AngleOuterCone()));
        }
    }

    prog.lightViewProjection.Set(shadowing.LightSpaceMatrix());
    prog.lightBleedingReduction.Set(shadowing.LightBleedingReduction());
    prog.voxelSize.Set(voxelSize);
    prog.worldMinPoint.Set(sceneBox.MinPoint());
    // voxel texture to read
    voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                          oglplus::ImageUnitFormat::RGBA8);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                          oglplus::ImageUnitFormat::RGBA8);;
    voxelRadiance.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                            oglplus::ImageUnitFormat::RGBA8);
    voxelRadiance.BindImage(3, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                            oglplus::ImageUnitFormat::RGBA8);
    auto workGroups = static_cast<unsigned>(glm::ceil(volumeDimension / 8.0f));
    // inject radiance at level 0 of texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // sync safety
    gl.MemoryBarrier(shImage | texFetch);
}

void VoxelizerRenderer::GenerateMipmap()
{
    GenerateMipmapBase(voxelRadiance);
    GenerateMipmapVolume();

    if (injectFirstBounce)
    {
        static oglplus::Context gl;
        static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                              (oglplus::MemoryBarrierBit::ShaderImageAccess);
        static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                               (oglplus::MemoryBarrierBit::TextureFetch);
        static auto &assets = AssetsManager::Instance();
        static auto &deferred = *static_cast<DeferredRenderer *>
                                (assets->renderers["Deferred"].get());
        static auto &proga = InjectPropagationShader();
        // inject direct + "first bounce" into voxel texture
        CurrentProgram<PropagationProgram>(proga);
        // tracing limits
        proga.maxTracingDistanceGlobal.Set(deferred.MaxTracingDistance());
        // albedo
        voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                              oglplus::ImageUnitFormat::RGBA8);
        // normals
        voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                              oglplus::ImageUnitFormat::RGBA8);
        voxelRadiance.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                                oglplus::ImageUnitFormat::RGBA8);
        // base volume - direct light
        voxelRadiance.Active(3);
        voxelRadiance.Bind(oglplus::TextureTarget::_3D);

        // anisotropic mipmap volume
        for (int i = 0; i < voxelTexMipmap.size(); ++i)
        {
            voxelTexMipmap[i].Active(4 + i);
            voxelTexMipmap[i].Bind(oglplus::TextureTarget::_3D);
        }

        // inject at level 0 of textur
        auto workGroups = static_cast<unsigned>(glm::ceil(volumeDimension / 8.0f));
        gl.DispatchCompute(workGroups, workGroups, workGroups);
        // sync safety
        gl.MemoryBarrier(shImage | texFetch);
        // now mipmap result
        GenerateMipmapBase(voxelRadiance);
        GenerateMipmapVolume();
    }
}

void VoxelizerRenderer::GenerateMipmapBase(oglplus::Texture &baseTexture)
{
    static oglplus::Context gl;
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);
    static auto &baseProg = MipMappingBaseShader();
    auto halfDimension = volumeDimension / 2;
    // base volume mipmap from voxel tex to first mip level
    CurrentProgram<MipmappingBaseProgram>(baseProg);
    // bind images for reading / writing
    baseProg.mipDimension.Set(halfDimension);
    baseTexture.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                          oglplus::ImageUnitFormat::RGBA8);

    for (int i = 0; i < voxelTexMipmap.size(); ++i)
    {
        voxelTexMipmap[i]
        .BindImage(i + 1, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                   oglplus::ImageUnitFormat::RGBA8);
    }

    auto workGroups = static_cast<unsigned int>(ceil(halfDimension / 8));
    // mipmap from base texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // mipmap radiance resulting volume
    gl.MemoryBarrier(shImage | texFetch);
}

void VoxelizerRenderer::GenerateMipmapVolume()
{
    static oglplus::Context gl;
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);
    static auto &volumeProg = MipMappingVolumeShader();
    // from first mip level to the rest aniso mipmapping
    CurrentProgram<MipmappingVolumeProgram>(volumeProg);
    // setup current mip level uniforms
    auto mipDimension = volumeDimension / 4;
    auto mipLevel = 0;

    while (mipDimension >= 1)
    {
        auto volumeSize = glm::vec3(mipDimension, mipDimension, mipDimension);
        volumeProg.mipDimension.Set(volumeSize);

        // bind for writing at mip level
        for (int i = 0; i < voxelTexMipmap.size(); ++i)
        {
            voxelTexMipmap[i].BindImage(i, mipLevel, true, 0,
                                        oglplus::AccessSpecifier::ReadOnly,
                                        oglplus::ImageUnitFormat::RGBA8);
            voxelTexMipmap[i].BindImage(i + 6, mipLevel + 1, true, 0,
                                        oglplus::AccessSpecifier::WriteOnly,
                                        oglplus::ImageUnitFormat::RGBA8);
        }

        auto workGroups = static_cast<unsigned>(glm::ceil(mipDimension / 8.0f));
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
        voxelRadiance.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                                oglplus::ImageUnitFormat::RGBA8);
    }
    else
    {
        voxelTexMipmap[drawDirection]
        .BindImage(0, drawMipLevel - 1, true, 0, oglplus::AccessSpecifier::ReadOnly,
                   oglplus::ImageUnitFormat::RGBA8);
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
    int i = 0;

    for (auto &matrix : viewProjectionMatrix)
    {
        matrix = projection * matrix;
        viewProjectionMatrixI[i++] = inverse(matrix);
    }
}
VoxelizerRenderer::VoxelizerRenderer(RenderWindow &window) : Renderer(window)
{
    injectFirstBounce = false;
    drawMipLevel = drawDirection = 0;
    framestep = -1; // on need
    SetupVoxelVolumes(256);
}
void VoxelizerRenderer::SetupVoxelVolumes(const unsigned int &dimension)
{
    using namespace oglplus;
    volumeDimension = dimension;
    voxelCount = volumeDimension * volumeDimension * volumeDimension;
    voxelSize = volumeGridSize / volumeDimension;

    // update projection matrices on new dimension
    if (Scene::Active())
    {
        UpdateProjectionMatrices(Scene::Active()->rootNode->boundaries);
    }

    auto maxLevel = static_cast<unsigned int>(log2(volumeDimension));
    // albedo volume
    voxelAlbedo.Bind(TextureTarget::_3D);
    voxelAlbedo.MinFilter(TextureTarget::_3D, TextureMinFilter::Nearest);
    voxelAlbedo.MagFilter(TextureTarget::_3D, TextureMagFilter::Nearest);
    voxelAlbedo.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelAlbedo.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelAlbedo.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelAlbedo.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                        dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                        PixelDataType::UnsignedByte, nullptr);
    // generate normal volume for radiance injection
    voxelNormal.Bind(TextureTarget::_3D);
    voxelNormal.MinFilter(TextureTarget::_3D, TextureMinFilter::Nearest);
    voxelNormal.MagFilter(TextureTarget::_3D, TextureMagFilter::Nearest);
    voxelNormal.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelNormal.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelNormal.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelNormal.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                        dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                        PixelDataType::UnsignedByte, nullptr);
    // generate radiance volume
    voxelRadiance.Bind(TextureTarget::_3D);
    voxelRadiance.MinFilter(TextureTarget::_3D, TextureMinFilter::Linear);
    voxelRadiance.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelRadiance.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelRadiance.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelRadiance.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelRadiance.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                          dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                          PixelDataType::UnsignedByte, nullptr);

    // mip mapping textures per face
    for (int i = 0; i < 6; i++)
    {
        voxelTexMipmap[i].Bind(TextureTarget::_3D);
        voxelTexMipmap[i].MinFilter(TextureTarget::_3D,
                                    TextureMinFilter::LinearMipmapLinear);
        voxelTexMipmap[i].MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
        voxelTexMipmap[i].WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
        voxelTexMipmap[i].WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
        voxelTexMipmap[i].WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
        voxelTexMipmap[i].Image3D(TextureTarget::_3D, 0,
                                  PixelDataInternalFormat::RGBA8,
                                  dimension / 2, dimension / 2,
                                  dimension / 2, 0,
                                  PixelDataFormat::RGBA,
                                  PixelDataType::UnsignedByte, nullptr);
        voxelTexMipmap[i].GenerateMipmap(TextureTarget::_3D);
    }
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
oglplus::Texture &VoxelizerRenderer::VoxelRadiance()
{
    return voxelRadiance;
}
std::array<oglplus::Texture, 6> &VoxelizerRenderer::VoxelTextureMipmap()
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
bool VoxelizerRenderer::InjectFirstBounce() const
{
    return injectFirstBounce;
}
void VoxelizerRenderer::InjectFirstBounce(bool val)
{
    injectFirstBounce = val;
    RevoxelizeScene();
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
PropagationProgram &VoxelizerRenderer::InjectPropagationShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<PropagationProgram *>
                        (assets->programs["InjectPropagation"].get());
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