#include "stdafx.h"
#include "engine_base.h"


// initializes base engine assets and libs
EngineBase::EngineBase()
{
}


EngineBase::~EngineBase()
{
}

std::shared_ptr<EngineBase> &EngineBase::Instance()
{
    static std::shared_ptr<EngineBase> instance = nullptr;

    if(!instance)
    {
        instance.reset(new EngineBase());
    }

    return instance;
}

#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

namespace oglplus
{
    class CubeExample
    {
        public:
            // wrapper around the current OpenGL context
            Context gl;
            // Vertex shader
            VertexShader vs;
            // Fragment shader
            FragmentShader fs;
            // Program
            Program prog;
            // A vertex array object for the rendered cube
            VertexArray cube;
            // VBOs for the cube's vertices and normals
            Buffer verts;
            Buffer normals;
        public:
            CubeExample(void)
            {
                // Set the vertex shader source
                vs.Source((GLSLString)
                          "#version 330\n"
                          "uniform mat4 ProjectionMatrix, CameraMatrix;"
                          "in vec4 Position;"
                          "in vec3 Normal;"
                          "out vec3 vertNormal;"
                          "void main(void)"
                          "{"
                          " vertNormal = Normal;"
                          " gl_Position = ProjectionMatrix *"
                          " CameraMatrix *"
                          " Position;"
                          "}"
                         );
                // compile it
                vs.Compile();
                // set the fragment shader source
                // (uses the absolute value of normal as color)
                fs.Source((GLSLString)
                          "#version 330\n"
                          "in vec3 vertNormal;"
                          "out vec4 fragColor;"
                          "void main(void)"
                          "{"
                          " fragColor = vec4(abs(vertNormal), 1.0);"
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
                // bind the VAO for the cube
                cube.Bind();
                const GLfloat c[8][3] =
                {
                    { -0.5f, -0.5f, +0.5f },
                    { -0.5f, -0.5f, -0.5f },
                    { -0.5f, +0.5f, -0.5f },
                    { -0.5f, +0.5f, +0.5f },
                    { +0.5f, -0.5f, +0.5f },
                    { +0.5f, -0.5f, -0.5f },
                    { +0.5f, +0.5f, -0.5f },
                    { +0.5f, +0.5f, +0.5f }
                };
                const GLuint vertex_count = 6 * 2 * 3;
                const GLfloat cube_vertices[vertex_count * 3] =
                {
                    c[0][0], c[0][1], c[0][2],
                    c[2][0], c[2][1], c[2][2],
                    c[1][0], c[1][1], c[1][2],
                    c[0][0], c[0][1], c[0][2],
                    c[3][0], c[3][1], c[3][2],
                    c[2][0], c[2][1], c[2][2],
                    c[0][0], c[0][1], c[0][2],
                    c[1][0], c[1][1], c[1][2],
                    c[4][0], c[4][1], c[4][2],
                    c[1][0], c[1][1], c[1][2],
                    c[5][0], c[5][1], c[5][2],
                    c[4][0], c[4][1], c[4][2],
                    c[1][0], c[1][1], c[1][2],
                    c[2][0], c[2][1], c[2][2],
                    c[5][0], c[5][1], c[5][2],
                    c[2][0], c[2][1], c[2][2],
                    c[6][0], c[6][1], c[6][2],
                    c[5][0], c[5][1], c[5][2],
                    c[4][0], c[4][1], c[4][2],
                    c[5][0], c[5][1], c[5][2],
                    c[6][0], c[6][1], c[6][2],
                    c[4][0], c[4][1], c[4][2],
                    c[6][0], c[6][1], c[6][2],
                    c[7][0], c[7][1], c[7][2],
                    c[2][0], c[2][1], c[2][2],
                    c[3][0], c[3][1], c[3][2],
                    c[7][0], c[7][1], c[7][2],
                    c[2][0], c[2][1], c[2][2],
                    c[7][0], c[7][1], c[7][2],
                    c[6][0], c[6][1], c[6][2],
                    c[0][0], c[0][1], c[0][2],
                    c[4][0], c[4][1], c[4][2],
                    c[3][0], c[3][1], c[3][2],
                    c[3][0], c[3][1], c[3][2],
                    c[4][0], c[4][1], c[4][2],
                    c[7][0], c[7][1], c[7][2]
                };
                // bind the VBO for the cube vertices
                verts.Bind(Buffer::Target::Array);
                // upload the data
                Buffer::Data(
                    Buffer::Target::Array,
                    vertex_count * 3,
                    cube_vertices
                );
                // setup the vertex attribs array for the vertices
                VertexArrayAttrib vert_attr(prog, "Position");
                vert_attr.Setup<Vec3f>();
                vert_attr.Enable();
                const GLfloat n[6][3] =
                {
                    { -1.0f, 0.0f, 0.0f },
                    { 0.0f, -1.0f, 0.0f },
                    { 0.0f, 0.0f, -1.0f },
                    { 1.0f, 0.0f, 0.0f },
                    { 0.0f, 1.0f, 0.0f },
                    { 0.0f, 0.0f, 1.0f }
                };
                GLfloat cube_normals[vertex_count * 3];

                for(GLuint f = 0; f != 6; ++f)
                    for(GLuint v = 0; v != 6; ++v)
                        for(GLuint ci = 0; ci != 3; ++ci)
                            cube_normals[(f * 6 + v) * 3 + ci] = n[f][ci];

                // bind the VBO for the cube normals
                normals.Bind(Buffer::Target::Array);
                // upload the data
                Buffer::Data(
                    Buffer::Target::Array,
                    vertex_count * 3,
                    cube_normals
                );
                // setup the vertex attribs array for the vertices
                VertexArrayAttrib normal_attr(prog, "Normal");
                normal_attr.Setup<Vec3f>();
                normal_attr.Enable();
                //
                // set the matrix for camera at (1,1,1) looking at origin
                Uniform<Mat4f>(prog, "CameraMatrix").Set(
                    CamMatrixf::LookingAt(
                        Vec3f(2.0f, 2.0f, 2.0f),
                        Vec3f()
                    )
                );
                //
                gl.ClearColor(1.0f, 1.0f, 1.0f, 0.0f);
                gl.ClearDepth(1.0f);
                gl.Enable(Capability::DepthTest);
            }
            void Reshape(GLuint width, GLuint height)
            {
                gl.Viewport(width, height);
                prog.Use();
                Uniform<Mat4f>(prog, "ProjectionMatrix").Set(
                    CamMatrixf::PerspectiveX(
                        Degrees(48),
                        GLfloat(width) / height,
                        1, 100
                    )
                );
                Uniform<Mat4f>(prog, "CameraMatrix").Set(
                    CamMatrixf::LookingAt(
                        Vec3f(2.0f, 2.0f, 2.0f),
                        Vec3f()
                    )
                );
            }
            void Render()
            {
                //gl.Clear().ColorBuffer().DepthBuffer();
                gl.DrawArrays(PrimitiveType::Triangles, 0, 6 * 2 * 3);
            }
    };
} // namespace oglplus

void EngineBase::MainLoop()
{
    using namespace oglplus;
    // import assets and initialize ext libraries
    this->Initialize();
    // gl context handler
    static Context gl;
    gl.Viewport(1280, 720);
    int nAvailableScenes = assetLoader->GetNumberOfAvailableScenes();
    // ex
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

        if(execInfo.activeScene >= 0 && execInfo.activeScene < nAvailableScenes)
        {
            renderer->Render(assetLoader->GetScene(execInfo.activeScene));
        }

        //cExample.Reshape(1280, 720);
        //cExample.cube.Bind();
        //cExample.Render();
        // ui render over scene
        userInterface.Render();
        // finally swap current frame
        glfwSwapBuffers(renderWindow.Handler());
    }

    userInterface.Terminate();
    // release reserved data early (context dependant)
    assetLoader.reset(nullptr);
    renderer.reset(nullptr);
}

void EngineBase::Initialize()
{
    // open window and set rendering context
    renderWindow.Open(); // creates rendering context
    renderWindow.SetAsCurrentContext();
    // initialize context dependant external libs
    initializer.InitializeContextDependant();
    // set interface to current renderwindow
    userInterface.Initialize(renderWindow);
    // load engine demo scene assets
    assetLoader = std::unique_ptr<EngineAssets>(new EngineAssets());
    assetLoader->LoadAssets();
    // initialize deferred shading renderer / manager
    renderer = std::unique_ptr<Renderer>(new Renderer());
    renderer->Initialize();
}
