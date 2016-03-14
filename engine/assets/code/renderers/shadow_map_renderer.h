#pragma once

#include "../core/renderer.h"

#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>

class Light;

class ShadowMapRenderer : public Renderer
{
    public:
        void Render() override;
        ShadowMapRenderer(RenderWindow &window);
        ~ShadowMapRenderer();
    private:
        void CreateFramebuffer(const unsigned int size) const;
        oglplus::Framebuffer shadowFramebuffer;
        oglplus::Texture renderDepth;

        const Light * shadowCaster;;
};

