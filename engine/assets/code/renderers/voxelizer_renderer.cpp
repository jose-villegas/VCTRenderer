#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "voxelizer_renderer.h"
#include "shadow_map_renderer.h"
#include "../scene/scene.h"
#include "../core/assets_manager.h"
#include "../scene/camera.h"
#include "../scene/texture.h"
#include "../scene/material.h"
#include "../../../rendering/render_window.h"

#include "../programs/voxelization_program.h"
#include "../programs/voxel_drawer_program.h"

#include <oglplus/context.hpp>
#include <oglplus/framebuffer.hpp>
#include <glm/gtx/transform.hpp>
#include "../../../scene/light.h"
#include "../programs/radiance_program.h"

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
    prog.matrices.model.Set(node.ModelMatrix());
    prog.matrices.normal.Set(inverse(transpose(node.ModelMatrix())));
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
    // voxelization pass uniforms
    prog.viewProjections[0].Set(viewProjectionMatrix[0]);
    prog.viewProjections[1].Set(viewProjectionMatrix[1]);
    prog.viewProjections[2].Set(viewProjectionMatrix[2]);
    prog.volumeDimension.Set(volumeDimension);
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
}

void VoxelizerRenderer::InjectRadiance()
{
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();
    static oglplus::Context gl;
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);
    static auto &prog = InjectRadianceShader();
    auto &shadowing = *static_cast<ShadowMapRenderer *>
                      (AssetsManager::Instance()->renderers["Shadowmapping"].get());
    auto &caster = *shadowing.Caster();
    // inject radiance into voxel texture and also mip-map
    CurrentProgram<InjectRadianceProgram>(prog);
    // control vars
    auto sceneBox = scene->rootNode->boundaries;
    auto voxelSize = volumeGridSize / volumeDimension;
    // voxel grid projection matrices
    auto model = translate(sceneBox.Center()) * scale(glm::vec3(voxelSize));;
    // pass compute shader uniform
    prog.matrices.model.Set(model);
    prog.lightViewProjection.Set(shadowing.LightSpaceMatrix());
    shadowing.BindReading(6);
    prog.shadowMap.Set(6);
    prog.exponents.Set(shadowing.Exponents());
    prog.lightBleedingReduction.Set(shadowing.LightBleedingReduction());
    prog.directionalLight.diffuse.Set(caster.Diffuse() * caster.Intensities().y);
    prog.directionalLight.direction.Set(caster.Direction());
    // voxel texture to read
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                       oglplus::ImageUnitFormat::R32UI);
    voxelNormal.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                          oglplus::ImageUnitFormat::R32UI);
    voxelTex.BindImage(2, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                       oglplus::ImageUnitFormat::RGBA8);
    auto workGroups = glm::ceil(volumeDimension / 8.0f);
    // inject radiance at level 0 of texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // mipmap radiance resulting volume
    voxelTex.Bind(oglplus::TextureTarget::_3D);
    voxelTex.GenerateMipmap(oglplus::TextureTarget::_3D);
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
    // control vars
    auto sceneBox = scene->rootNode->boundaries;
    auto voxelSize = volumeGridSize / volumeDimension;
    // voxel grid projection matrices
    auto model = translate(sceneBox.Center()) * scale(glm::vec3(voxelSize));
    auto &viewMatrix = camera->ViewMatrix();
    auto &projectionMatrix = camera->ProjectionMatrix();
    // pass voxel drawer uniforms
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                       oglplus::ImageUnitFormat::RGBA8);;
    prog.volumeDimension.Set(volumeDimension);
    prog.matrices.modelViewProjection.Set(projectionMatrix * viewMatrix * model);
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
    auto maxLevel = static_cast<unsigned int>(log2(volumeDimension));
    // generate albedo volume
    voxelTex.Bind(TextureTarget::_3D);
    voxelTex.MinFilter(TextureTarget::_3D, TextureMinFilter::LinearMipmapLinear);
    voxelTex.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelTex.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTex.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTex.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    voxelTex.Image3D(TextureTarget::_3D, 0, PixelDataInternalFormat::RGBA8,
                     dimension, dimension, dimension, 0, PixelDataFormat::RGBA,
                     PixelDataType::UnsignedByte, nullptr);
    voxelTex.ClearImage(0, PixelDataFormat::RGBA, zero);
    voxelTex.GenerateMipmap(TextureTarget::_3D);
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
}

void VoxelizerRenderer::RevoxelizeScene()
{
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    UpdateProjectionMatrices(scene->rootNode->boundaries);
    // update voxelization
    VoxelizeScene();
}

VoxelizerRenderer::~VoxelizerRenderer()
{
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