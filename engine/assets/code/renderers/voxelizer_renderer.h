#pragma once
#include "../core/renderer.h"

#include <oglplus/texture.hpp>
#include <glm/mat4x4.hpp>
#include <oglplus/vertex_array.hpp>

class MipmappingVolumeProgram;
class MipmappingBaseProgram;
class InjectRadianceProgram;
class BoundingBox;
class VoxelDrawerProgram;
class VoxelizationProgram;

class VoxelizerRenderer : public Renderer
{
    public:
        static bool ShowVoxels;
        /// <summary>
        /// Voxelizes the current scene. It can also be used to show the
        /// result volume.
        /// </summary>
        void Render() override;
        /// <summary>
        /// Sets the matrices uniforms.
        /// </summary>
        /// <param name="node">The node.</param>
        void SetMatricesUniforms(const Node &node) const override;
        /// <summary>
        /// Sets the material uniforms.
        /// </summary>
        /// <param name="material">The material.</param>
        void SetMaterialUniforms(const Material &material) const override;
        /// <summary>
        /// Sets the voxelization update frequency, voxelize scene will be called
        /// every framestep number of frames.
        /// </summary>
        /// <param name="framestep">The framestep.</param>
        void SetUpdateFrequency(const unsigned int framestep);
        /// <summary>
        /// Initializes a new instance of the <see cref="VoxelRenderer"/> class.
        /// </summary>
        /// <param name="window">The window.</param>
        explicit VoxelizerRenderer(RenderWindow &window);
        void SetupVoxelVolumes(const unsigned int &dimension);
        void RevoxelizeScene();
        void SetupDrawVoxels(const unsigned &level, const unsigned &direction);
        /// <summary>
        /// Finalizes an instance of the <see cref="VoxelRenderer"/> class.
        /// </summary>
        ~VoxelizerRenderer();
        const glm::mat4x4 &VoxelToWorldMatrix() const;
        const glm::mat4x4 &WorldToVoxelMatrix() const;
        const unsigned int &VolumeDimension() const;
        oglplus::Texture &VoxelTexture();
        oglplus::Texture &VoxelTextureMipmap();
        const float &VoxelWorldSize() const;
        const float &VolumeGridSize() const;
    private:
        /// <summary>
        /// Returns the voxelization program shader.
        /// </summary>
        /// <returns></returns>
        static VoxelizationProgram &VoxelizationPass();
        /// <summary>
        /// Returns the voxelization program shader.
        /// </summary>
        /// <returns></returns>
        static VoxelDrawerProgram &VoxelDrawerShader();
        static InjectRadianceProgram &InjectRadianceShader();
        static MipmappingBaseProgram &MipMappingBaseShader();
        static MipmappingVolumeProgram &MipMappingVolumeShader();
        /// <summary>
        /// Creates the view projection matrices per x, y and z axis.
        /// depending on the voxel volume grid size.
        /// </summary>
        void UpdateProjectionMatrices(const BoundingBox &sceneBox);
        /// <summary>
        /// Voxelizes the scene.
        /// </summary>
        void VoxelizeScene();
        void InjectRadiance();
        void GenerateMipmap();
        /// <summary>
        /// Draws the resulting voxels.
        /// </summary>
        void DrawVoxels();
        // output textures
        oglplus::Texture voxelTex;
        oglplus::Texture voxelTexMipmap;
        oglplus::Texture voxelNormal;

        // vertex buffer object for 3d texture visualization
        oglplus::VertexArray voxelDrawerArray;

        std::array<glm::mat4x4, 3> viewProjectionMatrix;
        unsigned int volumeDimension;
        int framestep;
        unsigned int drawMipLevel;
        unsigned int drawDirection;
        float volumeGridSize;
        float voxelSize;
        unsigned int voxelCount;
};
