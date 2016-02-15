#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "engine_base.h"

#include "engine_assets.h"
#include "../misc/utils.h"
#include "../behavior/behavior.h"
#include "../interface/interface.h"
#include "../rendering/render_window.h"
#include "../rendering/deferred_renderer.h"

#include <oglplus/gl.hpp>
#include <oglplus/context.hpp>

EngineBase::EngineBase()
{
    renderWindow = std::make_unique<RenderWindow>();
}

EngineBase::~EngineBase()
{
    // release reserved data early (context dependent)
    InterfaceRenderer::Terminate();
    AssetsManager::Terminate();
    delete renderer.release();
}

std::unique_ptr<EngineBase> &EngineBase::Instance()
{
    static std::unique_ptr<EngineBase> instance = nullptr;

    if (!instance)
    {
        instance.reset(new EngineBase());
    }

    return instance;
}

const DeferredRenderer &EngineBase::Renderer()
{
    return *Instance()->renderer;
}

void EngineBase::Terminate()
{
    delete Instance().release();
}

void EngineBase::MainLoop()
{
    static oglplus::Context gl;
    // import assets and initialize ext libraries
    this->Initialize();
    // set rendering view port
    gl.Viewport(renderWindow->Info().width, renderWindow->Info().height);

    // render loop
    while (!renderWindow->ShouldClose())
    {
        gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        gl.Clear().ColorBuffer().DepthBuffer();
        // poll window inputs
        RenderWindow::Events();
        // behaviors update
        Behavior::UpdateAll();
        // draw custom engine uis
        InterfaceRenderer::NewFrame();
        Interface::DrawAll();
        // render main scene
        renderer->Render();
        // ui render over scene
        InterfaceRenderer::Render();
        // finally swap current frame
        renderWindow->SwapBuffers();
    }
}

void EngineBase::Initialize()
{
    // open window and set rendering context
    renderWindow->WindowHint(RenderWindow::WindowHints::Resizable, false);
    renderWindow->Open(WindowInfo(1280, 720, 0, 0, "VCTRenderer"), false);
    renderWindow->SetAsCurrentContext();
    // initialize OpenGL API
    oglplus::GLAPIInitializer();
    // set interface to current renderwindow
    InterfaceRenderer::Initialize(*renderWindow);
    // print libs version info
    Utils::PrintDependenciesVersions();
    // deferred shading renderer / manager
    renderer = std::make_unique<DeferredRenderer>(*renderWindow);
    // initialize assets manager, holds all engine assets
    AssetsManager::Instance();
}
