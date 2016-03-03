#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "engine_base.h"

#include "behavior.h"
#include "interface.h"
#include "assets_manager.h"
#include "../rendering/render_window.h"
#include "../rendering/deferred_renderer.h"

#include <oglplus/gl.hpp>
#include <oglplus/context.hpp>
#include <iostream>

#include <FreeImagePlus.h>
#include <assimp/Importer.hpp>
#include <assimp/version.h>

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
    // deferred renderer as current active to it can be accessed by nodes
    renderer->SetAsActive();

    // render loop
    while (!renderWindow->ShouldClose())
    {
        gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        gl.Clear().ColorBuffer().DepthBuffer();
        // poll window inputs
        RenderWindow::Events();
        // setup interface renderer for a new frame
        InterfaceRenderer::NewFrame();
        // behaviors update
        Behavior::UpdateAll();
        // interfaces update
        Interface::DrawAll();
        // render main scene
        renderer->Render();
        // ui render over scene
        InterfaceRenderer::Render();
        // finally swap current frame
        renderWindow->SwapBuffers();
    }
}

inline void PrintDependenciesVersions()
{
    std::cout << "GLFW " << glfwGetVersionString() << std::endl;
    std::cout << "Assimp " << aiGetVersionMajor() << "." << aiGetVersionMinor()
              << "." << aiGetVersionRevision() << std::endl;
    std::cout << "FreeImage " << FreeImage_GetVersion() << std::endl;
    std::cout << "OpenGL " << glGetString(GL_VERSION) << "s, GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Ocornut's IMGUI " << ImGui::GetVersion() << std::endl;
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
    PrintDependenciesVersions();
    // deferred shading renderer / manager
    renderer = std::make_unique<DeferredRenderer>(*renderWindow);
    // initialize assets manager, holds all engine assets
    AssetsManager::Instance();
}
