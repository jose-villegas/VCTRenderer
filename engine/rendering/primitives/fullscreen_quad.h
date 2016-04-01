#pragma once

#include <oglplus/vertex_array.hpp>
#include <oglplus/buffer.hpp>

class FullscreenQuad
{
    public:
        void Draw() const;
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

