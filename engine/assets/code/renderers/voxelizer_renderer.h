#pragma once
#include "../core/renderer.h"

#include <oglplus/texture.hpp>
#include <glm/mat4x4.hpp>
#include <oglplus/vertex_array.hpp>
#include "../programs/propagation_program.h"

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
        void SetUpdateFrequency(const int framestep);
        /// <summary>
        /// Initializes a new instance of the <see cref="VoxelRenderer"/> class.
        /// </summary>
        /// <param name="window">The window.</param>
        explicit VoxelizerRenderer(RenderWindow &window);
        void SetupVoxelVolumes(const unsigned int &dimension);
        void RevoxelizeScene();
        void SetupDrawVoxels(const unsigned &level, const unsigned &direction,
                             const glm::vec4 colors);
        /// <summary>
        /// Finalizes an instance of the <see cref="VoxelRenderer"/> class.
        /// </summary>
        ~VoxelizerRenderer();
        const unsigned int &VolumeDimension() const;
        oglplus::Texture &VoxelRadiance();
        std::array<oglplus::Texture, 6> &VoxelTextureMipmap();
        oglplus::Texture &VoxelNormalVisibility();
        const float &VoxelWorldSize() const;
        const float &VolumeGridSize() const;
        bool TraceShadowCones() const;
        void TraceShadowCones(bool val);
        bool InjectFirstBounce() const;
        void InjectFirstBounce(bool val);
        void ShadowConeTolerance(const float &umbra);
        const float &ShadowConeUmbra() const;
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
        static PropagationProgram &InjectPropagationShader();
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
        void GenerateMipmapVolume();
        void GenerateMipmapBase(oglplus::Texture &baseTexture);
        /// <summary>
        /// Draws the resulting voxels.
        /// </summary>
        void DrawVoxels();
        // output texture
        oglplus::Texture voxelAlbedo;
        oglplus::Texture voxelNormal;
        oglplus::Texture voxelRadiance;
        std::array<oglplus::Texture, 6> voxelTexMipmap;
        // vertex buffer object for 3d texture visualization
        oglplus::VertexArray voxelDrawerArray;
        std::array<glm::mat4x4, 3> viewProjectionMatrix;
        std::array<glm::mat4x4, 3> viewProjectionMatrixI;

        unsigned int volumeDimension;
        unsigned int voxelCount;
        bool injectFirstBounce;
        float volumeGridSize;
        float voxelSize;
        int framestep;
        bool traceShadowCones;
        float coneShadowTolerance;

        unsigned int drawMipLevel;
        unsigned int drawDirection;
        glm::vec4 drawColorChannels;
};
