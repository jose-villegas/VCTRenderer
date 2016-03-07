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
#include <oglplus/vertex_attrib.hpp>
#include <glm/gtx/transform.hpp>
#include <tbb/parallel_for.h>

bool VoxelRenderer::ShowVoxels = false;

void VoxelRenderer::Render()
{
    static Scene * previous = nullptr;
    static auto frameCount = 0;
    static auto &scene = Scene::Active();

    if (!scene || !scene->IsLoaded()) { return; }

    SetAsActive();

    // scene changed or loaded
    if (previous != scene.get())
    {
        UpdateProjectionMatrices(scene->rootNode->boundaries);
        updateVoxelGrid = true;
        // update voxelization
        VoxelizeScene();
    }

    if (ShowVoxels)
    {
        if (updateVoxelGrid)
        {
            UpdateVoxelGrid(scene->rootNode->boundaries);
            updateVoxelGrid = false;
        }

        DrawVoxels();
    }

    // store current for next call
    previous = scene.get();

    // another frame called on render, if framestep is > 0
    // Voxelization will happen every framestep frame
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
    prog.material.diffuse.Set(material.Diffuse());
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
    static auto zero = 0;

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
    voxelAlbedo.ClearImage(0, oglplus::PixelDataFormat::RedInteger, &zero);
    atomicCounter.BindBase(oglplus::BufferIndexedTarget::AtomicCounter, 0);
    voxelAlbedo.BindImage(0, 0, true, 0, oglplus::AccessSpecifier::WriteOnly,
                          oglplus::ImageUnitFormat::R32UI);
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
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl.Clear().ColorBuffer().DepthBuffer();
    // Open GL flags
    gl.ClearDepth(1.0f);
    gl.Enable(oglplus::Capability::DepthTest);
    gl.Enable(oglplus::Capability::CullFace);
    gl.FrontFace(oglplus::FaceOrientation::CCW);
    gl.CullFace(oglplus::Face::Back);
    auto &prog = VoxelDrawerShader();
    CurrentProgram<VoxelDrawerProgram>(prog);
    // activate voxel albedo texture for reading
    voxelAlbedo.Active(0);
    voxelAlbedo.Bind(oglplus::TextureTarget::_3D);
    // voxel grid projection matrices
    auto &viewMatrix = camera->ViewMatrix();
    auto &projectionMatrix = camera->ProjectionMatrix();
    // pass voxel drawer uniforms
    prog.voxelAlbedo.Set(0);
    prog.halfVoxelSize.Set((volumeGridSize / volumeDimension) / 2.0f);
    prog.matrices.viewProjection.Set(projectionMatrix * viewMatrix);
    // bind vertex buffer array to draw, needed but all geometry is generated
    // in the geometry shader
    voxelDrawerArray.Bind();
    gl.DrawArrays(oglplus::PrimitiveType::Points, 0, voxelCount);
}

void VoxelRenderer::UpdateProjectionMatrices(const BoundingBox &sceneBox)
{
    auto diagonal = sceneBox.MaxPoint() - sceneBox.MinPoint();
    auto &center = sceneBox.Center();
    volumeGridSize = glm::max(diagonal.x, glm::max(diagonal.y, diagonal.z));
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

void VoxelRenderer::UpdateVoxelGrid(const BoundingBox &sceneBox) const
{
    // not using vertex class because it has unnecesary data
    // we need to reserve a big chunk of memory
    struct Point
    {
        glm::vec3 position;
        glm::vec3 uv;
    };
    auto voxelSize = volumeGridSize / volumeDimension;
    auto voxelGridData = std::vector<Point>();
    auto &center = sceneBox.Center();
    auto vRes = static_cast<float>(volumeDimension);
    auto halfRes = vRes / 2.0f;
    voxelGridData.resize(volumeDimension * volumeDimension *
                         volumeDimension);
    tbb::parallel_for(0, int(volumeDimension), [&](int x)
    {
        for (auto y = 0; y < volumeDimension; ++y)
        {
            for (auto z = 0; z < volumeDimension; ++z)
            {
                Point point;
                point.position = glm::vec3
                                 (
                                     (x - halfRes) * voxelSize,
                                     (y - halfRes) * voxelSize,
                                     (z - halfRes) * voxelSize
                                 ) + glm::vec3(voxelSize / 2) + center;
                point.uv = glm::vec3
                           (
                               x / vRes + 1.0f / (2.0f * vRes),
                               y / vRes + 1.0f / (2.0f * vRes),
                               z / vRes + 1.0f / (2.0f * vRes)
                           );
                auto index = z + y * volumeDimension + x * volumeDimension *
                             volumeDimension;
                voxelGridData[index] = std::move(point);
            }
        }
    });
    using namespace oglplus;
    voxelDrawerArray.Bind();
    // created here so it goes out of scope and deletes
    // itself, buffer array not needed
    Buffer voxelGridPoints;
    voxelGridPoints.Bind(BufferTarget::Array);
    voxelGridPoints.Data(BufferTarget::Array, voxelGridData);
    VertexArrayAttrib(VertexAttribSlot(0)).Enable()
    .Pointer(3, DataType::Float, false, sizeof(Point), // position
             reinterpret_cast<const GLvoid *>(0));
    VertexArrayAttrib(VertexAttribSlot(1)).Enable()
    .Pointer(3, DataType::Float, false, sizeof(Point), // uvs
             reinterpret_cast<const GLvoid *>(12));
    voxelGridData.clear();
    NoVertexArray().Bind();
}

void VoxelRenderer::GenerateVolumes() const
{
    using namespace oglplus;
    auto voxelData = new float[volumeDimension * volumeDimension * volumeDimension] {0};
    voxelAlbedo.Bind(TextureTarget::_3D);
    voxelAlbedo.Image3D(TextureTarget::_3D, 0,
                        PixelDataInternalFormat::R32UI,
                        volumeDimension, volumeDimension, volumeDimension, 0,
                        PixelDataFormat::RedInteger, PixelDataType::UnsignedInt,
                        voxelData);
    voxelAlbedo.BaseLevel(TextureTarget::_3D, 0);
    voxelAlbedo.MaxLevel(TextureTarget::_3D, 0);
    voxelAlbedo.SwizzleRGBA(TextureTarget::_3D,
                            TextureSwizzle::Red,
                            TextureSwizzle::Green,
                            TextureSwizzle::Blue,
                            TextureSwizzle::Alpha);
    voxelAlbedo.MinFilter(TextureTarget::_3D, TextureMinFilter::Linear);
    voxelAlbedo.MagFilter(TextureTarget::_3D, TextureMagFilter::Linear);
    voxelAlbedo.GenerateMipmap(TextureTarget::_3D);
    delete[] voxelData;
}

void VoxelRenderer::GenerateAtomicBuffer()
{
    unsigned int initVal = 0;
    atomicCounter.Bind(oglplus::BufferTarget::AtomicCounter);
    atomicCounter.Data(oglplus::BufferTarget::AtomicCounter, sizeof(unsigned int),
                       &initVal);
}

VoxelRenderer::VoxelRenderer(RenderWindow * window) : Renderer(window)
{
    framestep = 0; // only on scene change
    volumeDimension = 256;
    voxelCount = volumeDimension * volumeDimension * volumeDimension;
    GenerateVolumes();
    GenerateAtomicBuffer();
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
