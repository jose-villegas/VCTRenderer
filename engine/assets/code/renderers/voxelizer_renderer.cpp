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
#include "../programs/propagation_program.h"
#include "../programs/clear_dynamic_program.h"

#include <oglplus/context.hpp>
#include <oglplus/framebuffer.hpp>
#include <glm/gtx/transform.hpp>
#include "gi_deferred_renderer.h"

bool VoxelizerRenderer::ShowVoxels = false;

void VoxelizerRenderer::Render()
{
    static Scene * previous = nullptr;
    static auto frameCount = 1;
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    // scene changed or loaded
    if (previous != scene.get())
    {
        UpdateProjectionMatrices(scene->rootNode->boundaries);
        // update voxelization
        VoxelizeStaticScene();
    }

    // store current for next call
    previous = scene.get();
    static auto &changes = Transform::TransformChangedMap();

    if (framestep == -1)
    {
        for (auto &c : changes)
        {
            auto const * node = dynamic_cast<const Node *>(c.first);

            if (node && node->TransformChanged())
            {
                // update scene voxelization
                VoxelizeDynamicScene(); break;
            }
            else if(dynamic_cast<const Light *>(c.first))
            {
                // only radiance needs to be updated
                UpdateRadiance(); break;
            }
        }
    }
    // dyanmic process voxelization will happen every framestep frame
    else if (framestep >= 1 && frameCount++ % framestep == 0)
    {
        frameCount = 1;
        // update voxelization
        VoxelizeDynamicScene();
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
    prog.material.emissive.Set(material.Emissive());
    // set textures
    Texture::Active(4);
    material.BindTexture(RawTexture::Diffuse);
    Texture::Active(5);
    material.BindTexture(RawTexture::Opacity);
}

void VoxelizerRenderer::SetUpdateFrequency(const int framestep)
{
    this->framestep = framestep;
}

void VoxelizerRenderer::VoxelizeStaticScene()
{
    static oglplus::Context gl;
    static auto &scene = Scene::Active();
    static float zero[] = { 0, 0, 0, 0 };
    static float sZero = 0.0f;
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
    gl.Viewport(0, 0, volumeDimension, volumeDimension);
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
    prog.flagStaticVoxels.Set(1);
    // clear images before voxelization
    staticFlag.ClearImage(0, oglplus::PixelDataFormat::Red, &sZero);
    voxelAlbedo.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelNormal.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelEmissive.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    // bind the volume texture to be writen in shaders
    voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    voxelEmissive.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                            oglplus::ImageUnitFormat::R32UI);
    staticFlag.BindImage(3, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                         oglplus::ImageUnitFormat::R8);
    // draw scene triangles
    scene->rootNode->DrawListState(Node::Static);
    // sync barrier
    gl.MemoryBarrier(shImage | texFetch);
    // update dynamic scene also
    VoxelizeDynamicScene();
}

void VoxelizerRenderer::VoxelizeDynamicScene()
{
    static oglplus::Context gl;
    static auto &scene = Scene::Active();
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);

    if (!scene || !scene->IsLoaded()) { return; }

    auto &prog = VoxelizationPass();
    auto &sceneBox = scene->rootNode->boundaries;
    // current renderer as active
    SetAsActive();
    // first clear writing available voxels
    ClearDynamicVoxels();
    // unbind fbos use default
    oglplus::DefaultFramebuffer().Bind(oglplus::FramebufferTarget::Draw);
    // clear and setup viewport
    gl.ColorMask(false, false, false, false);
    gl.Viewport(0, 0, volumeDimension, volumeDimension);
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
    prog.flagStaticVoxels.Set(0);
    // bind the volume texture to be writen in shaders
    voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::R32UI);
    voxelEmissive.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                            oglplus::ImageUnitFormat::R32UI);
    staticFlag.BindImage(3, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                         oglplus::ImageUnitFormat::R8);
    // draw only dynamic objects
    scene->rootNode->DrawListState(Node::Dynamic);
    // sync barrier
    gl.MemoryBarrier(shImage | texFetch);
    // on voxel basis change, radiance needs to be updated
    UpdateRadiance();
}

void VoxelizerRenderer::UpdateRadiance()
{
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
        shadowingMethod = shadowingMethod == 2 && !traceShadowCones
                          ? 0 : shadowingMethod;
        // current light uniform
        auto &uLight = light->Type() == Light::Directional
                       ? uDirectionals[typeIndex.x++]
                       : light->Type() == Light::Point
                       ? uPoints[typeIndex.y++]
                       : uSpots[typeIndex.z++];
        // shared uniforms between types
        uLight.diffuse.Set(light->Diffuse() * factor.y);
        uLight.shadowingMethod.Set(shadowingMethod);

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
    prog.volumeDimension.Set(volumeDimension);
    prog.voxelScale.Set(1.0f / volumeGridSize);
    prog.traceShadowHit.Set(traceShadowHit);
    prog.normalWeightedLambert.Set(normalWeightedLambert);
    // clear radiance volume
    static float zero[] = { 0, 0, 0, 0 };
    voxelRadiance.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    // voxel texture to read
    voxelAlbedo.Active(0);
    voxelAlbedo.Bind(oglplus::TextureTarget::_3D);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::RGBA8);;
    voxelRadiance.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                            oglplus::ImageUnitFormat::RGBA8);
    voxelEmissive.BindImage(3, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
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
        static auto &deferred = *static_cast<GIDeferredRenderer *>
                                (assets->renderers["Deferred"].get());
        static auto &proga = InjectPropagationShader();
        // inject direct + "first bounce" into voxel texture
        CurrentProgram<PropagationProgram>(proga);
        // tracing limits
        proga.maxTracingDistanceGlobal.Set(deferred.MaxTracingDistance());
        proga.volumeDimension.Set(volumeDimension);
        proga.checkBoundaries.Set(deferred.CheckVolumeBoundaries() ? 1 : 0);
        // albedo
        voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                              oglplus::ImageUnitFormat::RGBA8);
        // normals
        voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                              oglplus::ImageUnitFormat::RGBA8);
        voxelRadiance.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                                oglplus::ImageUnitFormat::RGBA8);

        // anisotropic mipmap volume
        for (int i = 0; i < voxelTexMipmap.size(); ++i)
        {
            voxelTexMipmap[i].Active(3 + i);
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

    for (int i = 0; i < voxelTexMipmap.size(); ++i)
    {
        voxelTexMipmap[i]
        .BindImage(i, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                   oglplus::ImageUnitFormat::RGBA8);
    }

    baseTexture.Active(5);
    baseTexture.Bind(oglplus::TextureTarget::_3D);
    auto workGroups = static_cast<unsigned int>(ceil(halfDimension / 8));
    // mipmap from base texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // mipmap radiance resulting volume
    gl.MemoryBarrier(shImage | texFetch);
}

void VoxelizerRenderer::ClearDynamicVoxels()
{
    static oglplus::Context gl;
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);
    static auto &prog = VoxelCleanerShader();
    auto workGroups = static_cast<unsigned>(glm::ceil(volumeDimension / 8.0f));
    // inject radiance into voxel texture
    CurrentProgram<ClearDynamicProgram>(prog);
    voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                          oglplus::ImageUnitFormat::RGBA8);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                          oglplus::ImageUnitFormat::RGBA8);
    voxelEmissive.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                            oglplus::ImageUnitFormat::RGBA8);
    staticFlag.Active(3);
    staticFlag.Bind(oglplus::TextureTarget::_3D);
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // sync safety
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
        volumeProg.mipLevel.Set(mipLevel);

        // bind for writing at mip level
        for (auto i = 0; i < voxelTexMipmap.size(); ++i)
        {
            voxelTexMipmap[i].Active(i + 5);
            voxelTexMipmap[i].Bind(oglplus::TextureTarget::_3D);
            voxelTexMipmap[i].BindImage(i, mipLevel + 1, true, 0,
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
    gl.Viewport(info.framebufferWidth, info.framebufferHeight);
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
    if(drawDirection == 7)
    {
        voxelNormal.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                              oglplus::ImageUnitFormat::RGBA8);
    }
    else if (drawDirection == 8)
    {
        voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                              oglplus::ImageUnitFormat::RGBA8);
    }
    else if(drawMipLevel == 0)
    {
        voxelRadiance.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                                oglplus::ImageUnitFormat::RGBA8);
    }
    else if(drawMipLevel > 0)
    {
        voxelTexMipmap[drawDirection]
        .BindImage(0, drawMipLevel - 1, true, 0, oglplus::AccessSpecifier::ReadOnly,
                   oglplus::ImageUnitFormat::RGBA8);
    }

    auto model = translate(sceneBox.MinPoint()) * scale(glm::vec3(vSize));
    prog.volumeDimension.Set(vDimension);
    prog.matrices.modelViewProjection.Set(viewProjection * model);
    prog.colorChannels.Set(drawColorChannels);
    auto &planes = camera->Frustum().Planes();

    for (auto i = 0; i < 6; i++)
    {
        prog.frustumPlanes[i].Set(planes[i]);
    }

    prog.voxelSize.Set(voxelSize);
    prog.worldMinPoint.Set(sceneBox.MinPoint());
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
    traceShadowCones = true;
    traceShadowHit = 0.5f;
    drawColorChannels = glm::vec4(1.0f);
    normalWeightedLambert = true;
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
    voxelAlbedo.MinFilter(TextureTarget::_3D, TextureMinFilter::Linear);
    voxelAlbedo.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelAlbedo.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelAlbedo.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelAlbedo.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelAlbedo.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                        dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                        PixelDataType::UnsignedByte, nullptr);
    // generate normal volume for radiance injection
    voxelNormal.Bind(TextureTarget::_3D);
    voxelNormal.MinFilter(TextureTarget::_3D, TextureMinFilter::Linear);
    voxelNormal.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
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

    // emission volume
    voxelEmissive.Bind(TextureTarget::_3D);
    voxelEmissive.MinFilter(TextureTarget::_3D, TextureMinFilter::Linear);
    voxelEmissive.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelEmissive.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelEmissive.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelEmissive.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelEmissive.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                          dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                          PixelDataType::UnsignedByte, nullptr);
    // static flagging voxel
    staticFlag.Bind(TextureTarget::_3D);
    staticFlag.MinFilter(TextureTarget::_3D, TextureMinFilter::Nearest);
    staticFlag.MagFilter(TextureTarget::_3D, TextureMagFilter::Nearest);
    staticFlag.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    staticFlag.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    staticFlag.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    staticFlag.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::R8,
                       dimension, dimension, dimension, 0, PixelDataFormat::Red,
                       PixelDataType::UnsignedByte, nullptr);
}
void VoxelizerRenderer::RevoxelizeScene()
{
    // whole process happens per frame anyway
    if (framestep == 1) return;

    static auto scene = static_cast<Scene *>(nullptr);

    // active scene changed
    if (scene != Scene::Active().get())
    {
        scene = Scene::Active().get();
        UpdateProjectionMatrices(scene->rootNode->boundaries);
    }

    if (!scene || !scene->IsLoaded()) { return; }

    // update voxelization
    VoxelizeStaticScene();
}

void VoxelizerRenderer::SetupDrawVoxels(const unsigned &level,
                                        const unsigned &direction,
                                        const glm::vec4 colors)
{
    drawMipLevel = level;
    drawDirection = direction;
    drawColorChannels = colors;
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

oglplus::Texture &VoxelizerRenderer::VoxelNormalVisibility()
{
    return voxelNormal;
}

const float &VoxelizerRenderer::VoxelWorldSize() const
{
    return voxelSize;
}
const float &VoxelizerRenderer::VolumeGridSize() const
{
    return volumeGridSize;
}

bool VoxelizerRenderer::TraceShadowCones() const
{
    return traceShadowCones;
}

void VoxelizerRenderer::TraceShadowCones(bool val)
{
    traceShadowCones = val;
}

bool VoxelizerRenderer::InjectFirstBounce() const
{
    return injectFirstBounce;
}

void VoxelizerRenderer::TraceShadowHit(const float &umbra)
{
    traceShadowHit = umbra;
}

const float &VoxelizerRenderer::TraceShadowHit() const
{
    return traceShadowHit;
}

void VoxelizerRenderer::InjectFirstBounce(bool val)
{
    injectFirstBounce = val;
}

bool VoxelizerRenderer::WeightedLambert() const
{
    return normalWeightedLambert;
}

void VoxelizerRenderer::WeightedLambert(bool val)
{
    normalWeightedLambert = val;
}

VoxelizationProgram &VoxelizerRenderer::VoxelizationPass()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<VoxelizationProgram *>
                        (assets->programs["Voxelization"].get());
    return prog;
}

ClearDynamicProgram &VoxelizerRenderer::VoxelCleanerShader()
{
    static auto &assets = AssetsManager::Instance();
    static auto &prog = *static_cast<ClearDynamicProgram *>
                        (assets->programs["ClearDynamic"].get());
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