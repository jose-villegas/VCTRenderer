#pragma once
#include "../core/renderer.h"

#include <oglplus/texture.hpp>
#include <glm/mat4x4.hpp>

class VoxelizationProgram;

class VoxelRenderer : public Renderer
{
    public:
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
        explicit VoxelRenderer(RenderWindow * window);
        /// <summary>
        /// Finalizes an instance of the <see cref="VoxelRenderer"/> class.
        /// </summary>
        ~VoxelRenderer();
    private:
        /// <summary>
        /// Returns the voxelization program shader.
        /// </summary>
        /// <returns></returns>
        static VoxelizationProgram &VoxelizationPass();
        /// <summary>
        /// Creates the view projection matrices per x, y and z axis.
        /// depending on the voxel volume grid size.
        /// </summary>
        void ProjectionSetup();
        /// <summary>
        /// Creates the voxel volume, a 3D texture meant to contain the
        /// voxelization result.
        /// </summary>
        void GenerateVolumes() const;
        /// <summary>
        /// Voxelizes the scene.
        /// </summary>
        void VoxelizeScene();

        // output textures
        oglplus::Texture voxelAlbedo;
        oglplus::Texture voxelNormal;

        std::array<glm::mat4x4, 3> viewProjectionMatrix;
        unsigned int volumeDimension;
        unsigned int framestep;
        float volumeGridSize;
        bool renderVoxel;

        void SetVoxelizationPassUniforms() const;
};
