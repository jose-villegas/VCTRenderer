#pragma once

#include "../core/renderer.h"

#include "../../../scene/camera.h"

#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>

class DepthProgram;
class Light;

class ShadowMapRenderer : public Renderer
{
    public:
        void SetMatricesUniforms(const Node &node) const override;
        void Render() override;
        void Caster(const Light * caster);
        explicit ShadowMapRenderer(RenderWindow &window);
        ~ShadowMapRenderer();
    private:
        static DepthProgram &DepthShader();
        void CreateFramebuffer(const unsigned &w, const unsigned &h);
        oglplus::Framebuffer shadowFramebuffer;
        oglplus::Texture renderDepth;

        glm::uvec2 shadowMapSize;
        Camera lightView;
        const Light * shadowCaster;
};

