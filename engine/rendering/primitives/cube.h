#pragma once
#include "../../scene/mesh.h"
#include <oglplus/shapes/cube.hpp>

class Cube : public MeshDrawer
{
    public:
        void Load() override;
        void DrawElements() const override;
        Cube();
        ~Cube();
    private:
        // helper object building cube vertex attributes
        oglplus::shapes::Cube cubeShape;
        // helper object encapsulating cube drawing instructions
        oglplus::shapes::DrawingInstructions cubeInstructions;
        // indices pointing to cube primitive elements
        oglplus::shapes::Cube::IndexArray cubeIndices;
        // A vertex array object for the rendered cube
        oglplus::VertexArray cube;
        // VBOs for the cube's vertex attributes
        oglplus::Buffer verts;
        oglplus::Buffer normals;
        oglplus::Buffer texcoords;
};

