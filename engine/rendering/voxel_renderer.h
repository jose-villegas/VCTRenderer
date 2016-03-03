#pragma once
#include "../core/renderer.h"

#include <oglplus/texture.hpp>

class VoxelRenderer : Renderer
{
    public:
        void Render() override;

        explicit VoxelRenderer(RenderWindow &window);
        ~VoxelRenderer();
    private:
        oglplus::Texture voxelVolume;
};