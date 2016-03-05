#pragma once
#include "../core/renderer.h"

#include <oglplus/texture.hpp>
#include <oglplus/buffer.hpp>
#include <glm/mat4x4.hpp>

class VoxelizationProgram;

class VoxelRenderer : public Renderer
{
    public:
        void Render() override;
        void VoxelizeScene();
        void SetDimension(const unsigned int size);
        void SetMatricesUniforms(const Node &node) const override;
        void SetMaterialUniforms(const Material &material) const override;
        void SetUpdateFrequency(const unsigned int framestep);
        explicit VoxelRenderer(RenderWindow * window);
        ~VoxelRenderer();
    private:
        static VoxelizationProgram &VoxelizationPass();
        void ProjectionSetup();
        void BuildAtomicBuffer() const;
        void BuildVoxelList();
        void RenderVoxel();
        static void BuildLinearBuffer
        (
            const oglplus::Buffer &buffer,
            const oglplus::Texture &texture,
            const oglplus::PixelDataInternalFormat &format,
            size_t size
        );

        oglplus::Buffer atomicBuffer;
        // output textures
        oglplus::Texture voxelVolume;
        oglplus::Texture voxelPosition;
        oglplus::Texture voxelAlbedo;
        oglplus::Texture voxelNormal;
        // texture buffer objects
        oglplus::Buffer voxelPositionTBuffer;
        oglplus::Buffer voxelAlbedoTBuffer;
        oglplus::Buffer voxelNormalTBuffer;

        std::array<glm::mat4x4, 3> viewProjectionMatrix;
        unsigned int voxelDimension;
        unsigned int storeMode;
        unsigned int voxelFragmentCount;
        unsigned int framestep;
        bool renderVoxel;

        void SetVoxelizationPassUniforms() const;
};
