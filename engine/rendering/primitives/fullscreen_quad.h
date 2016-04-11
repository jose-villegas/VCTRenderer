#pragma once

#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>
#include "../../scene/mesh.h"

class FullscreenQuad : public MeshDrawer
{
    public:
        void Load() override;
        void DrawElements() const override;
        FullscreenQuad();
        ~FullscreenQuad();
    private:
        /// <summary>
        /// The full screen quad vertex array object
        /// </summary>
        oglplus::VertexArray fsQuadVertexArray;
        /// <summary>
        /// The full screen quad vertex buffer
        /// </summary>
        oglplus::Buffer fsQuadVertexBuffer;
        /// <summary>
        /// The full screen quad element index buffer
        /// </summary>
        oglplus::Buffer fsQuadElementBuffer;
};

