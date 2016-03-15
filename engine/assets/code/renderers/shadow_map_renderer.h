#pragma once

#include "../core/renderer.h"

#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>
#include "../../../scene/camera.h"

class Light;

class ShadowMapRenderer : public Renderer
{
    public:
        void Render() override;
        ShadowMapRenderer(RenderWindow &window);
        ~ShadowMapRenderer();
    private:
        void CreateFramebuffer(const unsigned &w, const unsigned &h) const;
        oglplus::Framebuffer shadowFramebuffer;
        oglplus::Texture renderDepth;

        Camera lightView;
        const Light * shadowCaster;;
};

