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

void VCT_ENGINE::Base::MainLoop()
{
    using namespace oglplus;
    // import assets and initialize ext libraries
    this->Initialize();
    // gl context handler
    Context gl;
    // ogl flags setup
    gl.Viewport(1280, 720);

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
        //// start geometry pass for deferred rendering
        assetLoader->GetGeometryPassShader().Use();
        // reset gl state flags after imgui draw
        gl.ClearDepth(1.0f);
        gl.Enable(Capability::DepthTest);
        gl.Enable(Capability::CullFace);
        gl.FrontFace(FaceOrientation::CCW);
        gl.CullFace(oglplus::Face::Back);

        if(execInfo.activeScene >= 0 &&
           execInfo.activeScene < assetLoader->GetNumberOfAvailableScenes())
        {
            assetLoader->GetScene(execInfo.activeScene).rootNode.DrawRecursive();
        }

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
