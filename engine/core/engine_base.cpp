#include "stdafx.h"
#include "engine_base.h"
#include "misc/utils.h"


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

    if (!instance)
    {
        instance.reset(new EngineBase());
    }

    return instance;
}

void EngineBase::MainLoop()
{
    using namespace oglplus;
    // import assets and initialize ext libraries
    this->Initialize();
    gl.Viewport(1280, 720);
    int sceneCount = assetLoader->SceneCount();

    // render loop
    while (!renderWindow.ShouldClose())
    {
        gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        gl.Clear().ColorBuffer().DepthBuffer();
        // poll window inputs
        renderWindow.Events();
        // draw custom engine ui
        userInterface.NewFrame();
        userInterface.Draw();

        if (execInfo.activeScene >= 0 && execInfo.activeScene < sceneCount)
        {
            auto &activeScene = assetLoader->GetScene(execInfo.activeScene);
            renderer->Render(activeScene);
        }

        // ui render over scene
        userInterface.Render();
        // finally swap current frame
        renderWindow.SwapBuffers();
    }

    userInterface.Terminate();
    // release reserved data early (context dependent)
    assetLoader.reset(nullptr);
    renderer.reset(nullptr);
}

void EngineBase::Initialize()
{
    // open window and set rendering context
    renderWindow.WindowHint(RenderWindow::WindowHints::Resizable, false);
    renderWindow.Open(WindowInfo(1280, 720, 0, 0, "VCTRenderer"), false);
    renderWindow.SetAsCurrentContext();
    // initialize OpenGL API
    oglplus::GLAPIInitializer();
    // set interface to current renderwindow
    userInterface.Initialize(renderWindow);
    // print libs version info
    utils::PrintDependenciesVersions();
    // load engine demo scene assets
    assetLoader = std::make_unique<EngineAssets>();
    assetLoader->LoadAssets();
    // initialize deferred shading renderer / manager
    renderer = std::make_unique<DeferredRenderer>(renderWindow);
    renderer->Initialize();
}
