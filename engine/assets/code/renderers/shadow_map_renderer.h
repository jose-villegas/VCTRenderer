#pragma once

#include "../core/renderer.h"

#include "../../../scene/camera.h"

#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/renderbuffer.hpp>

class DepthProgram;
class Light;

class ShadowMapRenderer : public Renderer
{
    public:
        void SetMatricesUniforms(const Node &node) const override;
        void Render() override;
        void Caster(const Light * caster);
        const glm::mat4x4 &LightSpaceMatrix();
        const Light * Caster() const;
        void BindReading(unsigned int unit) const;
        const Camera &LightCamera() const;
        const oglplus::Texture &ShadowMap() const;
        explicit ShadowMapRenderer(RenderWindow &window);
        ~ShadowMapRenderer();
    private:
        static DepthProgram &DepthShader();
        void CreateFramebuffer(const unsigned &w, const unsigned &h);
        oglplus::Framebuffer shadowFramebuffer;
        oglplus::Renderbuffer depthRender;
        oglplus::Texture shadowMap;

        glm::uvec2 shadowMapSize;
        Camera lightView;
        const Light * shadowCaster;
        glm::mat4x4 lightSpaceMatrix;
};

