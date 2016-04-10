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
    static auto &changes = Transform::TransformChangedMap();
    static auto revoxelize = true;

    for (auto &c : changes)
    {
        if (typeid(*c.first) == typeid(Light) || typeid(*c.first) == typeid(Node))
        {
            revoxelize = true; break;
        }
    }

    if(framestep == -1 && revoxelize)
    {
        // update voxelization
        VoxelizeScene();
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
    voxelTex.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                       oglplus::ImageUnitFormat::R32UI);
    voxelNormal.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    // draw scene triangles
    scene->rootNode->DrawList();
    // sync barrier
    gl.MemoryBarrier(shImage | texFetch);
    // compute shader injects diffuse lighting and shadowing
    InjectRadiance();
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

    auto dimension = injectFirstBounce ? volumeDimension / 2.0f : volumeDimension;
    prog.lightViewProjection.Set(shadowing.LightSpaceMatrix());
    prog.lightBleedingReduction.Set(shadowing.LightBleedingReduction());
    prog.voxelSize.Set(voxelSize);
    prog.worldMinPoint.Set(sceneBox.MinPoint());
    prog.writingLowerMip.Set(injectFirstBounce ? 1 : 0);
    // voxel texture to read
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                       oglplus::ImageUnitFormat::RGBA8);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                          oglplus::ImageUnitFormat::RGBA8);
    (injectFirstBounce ? voxelTexMipmap : voxelTex)
    .BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
               oglplus::ImageUnitFormat::RGBA8);
    auto workGroups = static_cast<unsigned>(glm::ceil(dimension / 8.0f));
    // inject radiance at level 0 of texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // sync safety
    gl.MemoryBarrier(shImage | texFetch);

    if(injectFirstBounce)
    {
        static auto &assets = AssetsManager::Instance();
        static auto &deferred = *static_cast<DeferredRenderer *>
                                (assets->renderers["Deferred"].get());
        static auto &proga = InjectPropagationShader();
        // we will have to mip map twice, here for cone tracing
        GenerateMipmapVolume();
        //// inject direct + "first bounce" into voxel texture
        CurrentProgram<PropagationProgram>(proga);
        // tracing limits
        proga.maxTracingDistanceGlobal.Set(deferred.MaxTracingDistance());
        // voxel textures to read
        voxelTexMipmap.Active(1);
        voxelTexMipmap.Bind(oglplus::TextureTarget::_3D);
        voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                           oglplus::ImageUnitFormat::RGBA8);
        voxelNormal.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                              oglplus::ImageUnitFormat::RGBA8);
        voxelNormal.BindImage(3, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                              oglplus::ImageUnitFormat::RGBA8);
        // inject at level 0 of textur
        workGroups = static_cast<unsigned>(glm::ceil(volumeDimension / 8.0f));
        gl.DispatchCompute(workGroups, workGroups, workGroups);
        // sync safety
        gl.MemoryBarrier(shImage | texFetch);
        // now mipmap result
        GenerateMipmap(voxelNormal);
    }
    else
    {
        GenerateMipmap(voxelTex);
    }
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
    // bind source texture
    voxelTexMipmap.Active(0);
    voxelTexMipmap.Bind(oglplus::TextureTarget::_3D);
    // setup current mip level uniforms
    auto mipDimension = volumeDimension / 4;
    auto mipLevel = 0;

    while(mipDimension >= 1)
    {
        volumeProg.mipDimension.Set(mipDimension);
        volumeProg.mipLevel.Set(mipLevel);
        // bind for writing at mip level
        voxelTexMipmap.BindImage(1, mipLevel + 1, true, 0,
                                 oglplus::AccessSpecifier::WriteOnly,
                                 oglplus::ImageUnitFormat::RGBA8);
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

void VoxelizerRenderer::GenerateMipmap(oglplus::Texture &baseTexture)
{
    GenerateMipmapBase(baseTexture);
    GenerateMipmapVolume();
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
    baseTexture.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                          oglplus::ImageUnitFormat::RGBA8);
    voxelTexMipmap.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                             oglplus::ImageUnitFormat::RGBA8);
    auto workGroups = static_cast<unsigned>(glm::ceil(halfDimension / 8.0f));
    // mipmap from base texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // mipmap radiance resulting volume
    gl.MemoryBarrier(shImage | texFetch);
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
        if(injectFirstBounce)
        {
            voxelNormal.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                                  oglplus::ImageUnitFormat::RGBA8);
        }
        else
        {
            voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                               oglplus::ImageUnitFormat::RGBA8);
        }

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
    injectFirstBounce = false;
    drawMipLevel = drawDirection = 0;
    framestep = -1; // on need
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
    return injectFirstBounce ? voxelNormal : voxelTex;
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