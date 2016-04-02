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
    voxelTex.ClearImage(0, oglplus::PixelDataFormat::RGBA, zero);
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
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
    static auto &camera = Camera::Active();
    static auto &scene = Scene::Active();
    auto &shadowing = *static_cast<ShadowMapRenderer *>
                      (AssetsManager::Instance()->renderers["Shadowmapping"].get());
    static oglplus::Context gl;
    static auto shImage = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                          (oglplus::MemoryBarrierBit::ShaderImageAccess);
    static auto texFetch = oglplus::Bitfield<oglplus::MemoryBarrierBit>
                           (oglplus::MemoryBarrierBit::TextureFetch);
    static auto &prog = InjectRadianceShader();
    // inject radiance into voxel texture and also mip-map
    CurrentProgram<InjectRadianceProgram>(prog);
    // control vars
    auto sceneBox = scene->rootNode->boundaries;
    auto voxelSize = volumeGridSize / volumeDimension;
    // voxel grid projection matrices
    auto model = translate(sceneBox.Center()) * scale(glm::vec3(voxelSize));
    // pass compute shader uniform
    prog.matrices.model.Set(model);
    prog.lightViewProjection.Set(shadowing.LightSpaceMatrix());
    shadowing.BindReading(6);
    prog.shadowMap.Set(6);
    prog.exponents.Set(shadowing.Exponents());
    prog.lightBleedingReduction.Set(shadowing.LightBleedingReduction());
    // voxel texture to read
    voxelTex.Active(0);
    voxelTex.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::ReadOnly,
                       oglplus::ImageUnitFormat::R32UI);
    voxelTex.BindImage(1, 0, true, 0, oglplus::AccessSpecifier::ReadWrite,
                       oglplus::ImageUnitFormat::RGBA8);
    auto workGroups = glm::ceil(volumeDimension / 4.0f);
    // inject radiance at level 0 of texture
    gl.DispatchCompute(workGroups, workGroups, workGroups);
    // advantage of using 3d texture is easy mip-mapping
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

void VoxelizerRenderer::CreateVolume(oglplus::Texture &texture) const
{
    using namespace oglplus;
    static float zero[] = {0, 0, 0, 0};
    auto maxLevel = static_cast<unsigned int>(log2(volumeDimension));
    texture.Bind(TextureTarget::_3D);
    texture.MinFilter(TextureTarget::_3D, TextureMinFilter::LinearMipmapLinear);
    texture.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    texture.WrapR(TextureTarget::_3D, TextureWrap::ClampToEdge);
    texture.WrapS(TextureTarget::_3D, TextureWrap::ClampToEdge);
    texture.WrapT(TextureTarget::_3D, TextureWrap::ClampToEdge);
    texture.Storage3D(TextureTarget::_3D, maxLevel, PixelDataInternalFormat::RGBA8,
                      volumeDimension, volumeDimension, volumeDimension);
    texture.ClearImage(0, PixelDataFormat::RGBA, zero);
    texture.GenerateMipmap(TextureTarget::_3D);
}

VoxelizerRenderer::VoxelizerRenderer(RenderWindow &window) : Renderer(window)
{
    framestep = 5; // only on scene change
    volumeDimension = 256;
    voxelCount = volumeDimension * volumeDimension * volumeDimension;
    CreateVolume(voxelTex);
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