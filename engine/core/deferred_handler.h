#pragma once

#include "../scene/texture.h"
#include "geometry_buffer.h"

#include <oglplus/framebuffer.hpp>
#include <oglplus/context.hpp>
#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>

class LightingProgram;
class GeometryProgram;

class DeferredHandler
{
    public:
        std::unique_ptr<GeometryProgram> geometryProgram;
        std::unique_ptr<LightingProgram> lightingProgram;
        GeometryBuffer geometryBuffer;

        DeferredHandler(unsigned int windowWith, unsigned int windowHeight);
        virtual ~DeferredHandler();
        void RenderFullscreenQuad() const;
    private:
        // full screen quad
        oglplus::VertexArray fullscreenQuadVertexArray;
        oglplus::Buffer fullscreenQuadVertexBuffer;
        oglplus::Buffer fullscreenQuadElementBuffer;
        void LoadShaders();
        void SetupGeometryBuffer(unsigned int windowWith, unsigned int windowHeight);
        void CreateFullscreenQuad() const;
};