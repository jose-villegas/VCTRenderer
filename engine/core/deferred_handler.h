#pragma once

#include "../scene/texture.h"

#include <oglplus/framebuffer.hpp>
#include <oglplus/context.hpp>
#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>

enum class GBufferTextureId;
class LightingProgram;
class GeometryProgram;

class DeferredHandler
{
    public:
        std::unique_ptr<GeometryProgram> geometryProgram;
        std::unique_ptr<LightingProgram> lightingProgram;

        DeferredHandler(unsigned int windowWith, unsigned int windowHeight);
        virtual ~DeferredHandler();
        void BindGeometryBuffer(const oglplus::FramebufferTarget &bindingMode);
        void ReadGeometryBuffer(const GBufferTextureId &bufferTexId);
        void ActivateBindTextureTargets();
        void RenderFullscreenQuad();

        /// <summary>
        /// Returns an array with all the geometry buffer textures
        /// </summary>
        /// <returns>Array of textures</returns>
        const std::vector<oglplus::Texture> &BufferTextures() const;
    private:
        oglplus::Framebuffer geometryBuffer;
        std::vector<oglplus::Texture> bufferTextures;
        std::vector<oglplus::Context::ColorBuffer> colorBuffers;
        // full screen quad
        oglplus::VertexArray fullscreenQuadVertexArray;
        oglplus::Buffer fullscreenQuadVertexBuffer;
        oglplus::Buffer fullscreenQuadElementBuffer;
        void LoadShaders();
        void SetupGBuffer(unsigned int windowWith, unsigned int windowHeight);
        void CreateFullscreenQuad();
};