#include "stdafx.h"
#include "base.h"
using namespace VCT_ENGINE;

// initializes base engine assets and libs
Base::Base()
{
}


Base::~Base()
{
}

std::shared_ptr<Base> &VCT_ENGINE::Base::Instance()
{
    static std::shared_ptr<Base> instance = nullptr;

    if(!instance)
    {
        instance.reset(new Base());
    }

    return instance;
}

#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <cmath>
namespace oglplus
{
    class CubeExample
    {
        public:
            // helper object building cube vertex attributes
            shapes::Cube make_cube;
            // helper object encapsulating cube drawing instructions
            shapes::DrawingInstructions cube_instr;
            // indices pointing to cube primitive elements
            shapes::Cube::IndexArray cube_indices;
            // wrapper around the current OpenGL context
            Context gl;
            // Vertex shader
            VertexShader vs;
            // Fragment shader
            FragmentShader fs;
            // Program
            Program prog;
            // Uniforms
            Uniform<Mat4f> projection_matrix, camera_matrix;
            // A vertex array object for the rendered cube
            VertexArray cube;
            // VBOs for the cube's vertices
            Buffer verts;
        public:
            CubeExample(void)
                : cube_instr(make_cube.Instructions())
                , cube_indices(make_cube.Indices())
                , projection_matrix(prog)
                , camera_matrix(prog)
            {
                // Set the vertex shader source
                vs.Source((GLSLString)
                          "#version 330\n"
                          "uniform mat4 ProjectionMatrix, CameraMatrix;"
                          "in vec4 Position;"
                          "out vec3 vertColor;"
                          "void main(void)"
                          "{"
                          " float angle = gl_InstanceID * 10 * 2 * 3.14159 / 360.0;"
                          " float cx = cos(angle);"
                          " float sx = sin(angle);"
                          " mat4 ModelMatrix = mat4("
                          " cx, 0.0, sx, 0.0,"
                          " 0.0, 1.0, 0.0, 0.0,"
                          " -sx, 0.0, cx, 0.0,"
                          " 0.0, 0.0, 0.0, 1.0 "
                          " ) * mat4("
                          " 1.0, 0.0, 0.0, 0.0,"
                          " 0.0, 1.0, 0.0, 0.0,"
                          " 0.0, 0.0, 1.0, 0.0,"
                          " 12.0, 0.0, 0.0, 1.0 "
                          " );"
                          " gl_Position = "
                          " ProjectionMatrix *"
                          " CameraMatrix *"
                          " ModelMatrix *"
                          " Position;"
                          " vertColor = abs(normalize((ModelMatrix*Position).xyz));"
                          "}"
                         );
                // compile it
                vs.Compile();
                // set the fragment shader source
                fs.Source((GLSLString)
                          "#version 330\n"
                          "in vec3 vertColor;"
                          "out vec4 fragColor;"
                          "void main(void)"
                          "{"
                          " fragColor = vec4(vertColor, 1.0);"
                          "}"
                         );
                // compile it
                fs.Compile();
                // attach the shaders to the program
                prog.AttachShader(vs);
                prog.AttachShader(fs);
                // link and use it
                prog.Link();
                prog.Use();
                projection_matrix.BindTo("ProjectionMatrix");
                camera_matrix.BindTo("CameraMatrix");
                // bind the VAO for the cube
                cube.Bind();
                // bind the VBO for the cube vertices
                verts.Bind(Buffer::Target::Array);
                {
                    std::vector<GLfloat> data;
                    GLuint n_per_vertex = make_cube.Positions(data);
                    // upload the data
                    Buffer::Data(Buffer::Target::Array, data);
                    // setup the vertex attribs array for the vertices
                    VertexArrayAttrib attr(prog, "Position");
                    attr.Setup<GLfloat>(n_per_vertex);
                    attr.Enable();
                }
                //
                gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
                gl.ClearDepth(1.0f);
                gl.Enable(Capability::DepthTest);
            }
            void Reshape(GLuint width, GLuint height)
            {
                gl.Viewport(width, height);
                prog.Use();
                projection_matrix.Set(
                    CamMatrixf::PerspectiveX(
                        Degrees(70),
                        double(width) / height,
                        1, 50
                    )
                );
            }
            void Render(double time)
            {
                gl.Clear().ColorBuffer().DepthBuffer();
                //
                // set the matrix for camera orbiting the origin
                camera_matrix.Set(
                    CamMatrixf::Orbiting(
                        Vec3f(),
                        18.5,
                        Degrees(time * 135),
                        Degrees(SineWave(time / 20.0) * 30)
                    )
                );
                // draw 36 instances of the cube
                // the vertex shader will take care of their placement
                cube_instr.Draw(cube_indices, 36);
            }
            bool Continue(double time)
            {
                return time < 30.0;
            }
    };
} // namespace oglplus

void VCT_ENGINE::Base::MainLoop()
{
    using namespace oglplus;
    // import assets and initialize ext libraries
    this->Initialize();
    // gl context handler
    Context gl;
    // ogl flags setup
    gl.Viewport(1280, 720);
    oglplus::CubeExample cExample;
    cExample.Reshape(1280, 720);

    // render loop
    while(!glfwWindowShouldClose(renderWindow.Handler()))
    {
        gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        gl.Clear().ColorBuffer().DepthBuffer();
        // poll window inputs
        glfwPollEvents();
        // draw custom engine ui
        userInterface.NewFrame();
        userInterface.Draw();
        // start geometry pass for deferred rendering
        assetLoader->GetGeometryPassShader().Use();
        // reset gl state flags after imgui draw

        if(execInfo.activeScene >= 0 &&
           execInfo.activeScene < assetLoader->GetNumberOfAvailableScenes())
        {
            assetLoader->GetScene(execInfo.activeScene)
            .rootNode.DrawRecursive();
        }

        cExample.prog.Use();
        cExample.Reshape(1280, 720);
        cExample.cube.Bind();
        cExample.Render(glfwGetTime());
        // ui render over scene
        userInterface.Render();
        // finally swap current frame
        glfwSwapBuffers(renderWindow.Handler());
    }

    userInterface.Terminate();
    // release reserved data early (context dependant)
    assetLoader.reset(nullptr);
}

void VCT_ENGINE::Base::Initialize()
{
    // open window and set rendering context
    renderWindow.Open(); // creates rendering context
    renderWindow.SetAsCurrentContext();
    // initialize context dependant external libs
    initializer.InitializeContextDependant();
    // set interface to current renderwindow
    userInterface.Initialize(renderWindow);
    // load engine demo scene assets
    assetLoader = std::move(std::unique_ptr<Assets>(new Assets()));
    assetLoader->LoadDeferredShaders();
    assetLoader->LoadDemoScenes();
}
