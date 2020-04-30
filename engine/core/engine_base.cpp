#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "engine_base.h"

#include "behavior.h"
#include "interface.h"
#include "renderer.h"
#include "assets_manager.h"
#include "../rendering/render_window.h"
#include "../types/transform.h"

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

void EngineBase::MainLoop() const
{
    static oglplus::Context gl;
    static auto enteredMainLoop = false;

    if (enteredMainLoop) { return; }

    // import assets and initialize ext libraries
    this->Initialize();
    // entered main loop, this function cannot be called again
    enteredMainLoop = true;

    // render loop
    while (!renderWindow->ShouldClose())
    {
        gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        gl.Clear().ColorBuffer().DepthBuffer();
        // poll window inputs
        RenderWindow::Events();
        // setup interface renderer for a new frame
        InterfaceRenderer::NewFrame();
        // interfaces logic update
        Interface::DrawAll();
        // behaviors update
        Behavior::UpdateAll();
        // call renderers
        Renderer::RenderAll();
        // ui render over scene
        InterfaceRenderer::Render();
        // finally swap current frame
        renderWindow->SwapBuffers();
        // clean up
        Transform::CleanEventMap();
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

RenderWindow &EngineBase::Window() const
{
    return *renderWindow;
}

void EngineBase::Initialize() const
{
    if(!renderWindow->IsOpen())
    {
        auto monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode(monitor);
        // open window
        renderWindow->WindowHint(FramebufferHints::RedBits, mode->redBits);
        renderWindow->WindowHint(FramebufferHints::GreenBits, mode->greenBits);
        renderWindow->WindowHint(FramebufferHints::BlueBits, mode->blueBits);
        renderWindow->WindowHint(FramebufferHints::RefreshRate, mode->refreshRate);
        renderWindow->WindowHint(FramebufferHints::DoubleBuffer, true);
        renderWindow->WindowHint(WindowHints::AutoIconify, false);
        renderWindow->WindowHint(WindowHints::Resizable, false);
        renderWindow->WindowHint(ContextHints::ContextVersionMajor, 4);
        renderWindow->WindowHint(ContextHints::ContextVersionMinor, 3);
        renderWindow->WindowHint(ContextHints::OpenGLProfile, Hint::OpenGLCoreProfile);
        renderWindow->WindowHint(ContextHints::OpenGLForwardCompatibility, Hint::False);
        renderWindow->Open(WindowInfo(1280, 720, 0, 0, "Voxel Cone Tracing"), false,
            nullptr, nullptr);
    }

    // and set window as rendering context
    renderWindow->SetAsCurrentContext();
    // initialize OpenGL API
    oglplus::GLAPIInitializer();
    // set interface to current renderwindow
    InterfaceRenderer::Initialize(*renderWindow);
    // print libs version info
    PrintDependenciesVersions();
    // initialize assets manager, holds all engine assets
    AssetsManager::Instance();
}
