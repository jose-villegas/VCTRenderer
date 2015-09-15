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
    // import assets and initialize ext libraries
    this->Initialize();
    // gl context handler
    oglplus::Context gl;
    // black screen initiallly
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // render loop
    while(!glfwWindowShouldClose(renderWindow.Handler()))
    {
        glfwPollEvents();
        // draw custom engine ui
        userInterface.Draw();
        gl.Clear().ColorBuffer().DepthBuffer(); glClear(GL_COLOR_BUFFER_BIT);
        userInterface.Render();
        glfwSwapBuffers(renderWindow.Handler());
    }

    userInterface.Terminate();
}

void VCT_ENGINE::Base::Initialize()
{
    // initialize external dependencies
    initializer.Initialize();
    // open window and set rendering context
    renderWindow.Open();
    renderWindow.SetAsCurrentContext();
    // initialize context dependant external libs
    initializer.InitializeContextDependant();
    // set interface to current renderwindow
    userInterface.Initialize(renderWindow);
    // load engine demo scene assets
    assetLoader.LoadDemoScenes();
    assetLoader.LoadShaders();
}
