#include "render_window.h"
#include <iostream>

/// <summary>
/// Called when an error occurs using the GLFW window
/// </summary>
/// <param name="code">The error code.</param>
/// <param name="description">The error description.</param>
void RenderWindow::OnErrorCallback(int code, const char * description)
{
    throw std::runtime_error(description);
}


WindowInfo::WindowInfo() : WindowInfo(1280, 720, 0, 0, "Default") {}

/// <summary>
/// Initializes a new instance of the <see cref="WindowInfo"/> class.
/// </summary>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="title">The title.</param>
WindowInfo::WindowInfo(const unsigned width, const unsigned height, const int x,
                       const int y, const std::string &title)
    : width{width}, height{height}, x{x}, y{y}, title{title}
{
}

/// <summary>
/// Sets a <see cref="WindowHints" /> hint value. Has to be called
/// before calling <see cref="WindowInfoRenderWindow.Open" />
/// </summary>
/// <param name="target">The target.</param>
/// <param name="value">The value.</param>
void RenderWindow::WindowHint(const WindowHints &target, const int value)
{
    if (static_cast<Hint>(value) == Hint::True ||
            static_cast<Hint>(value) == Hint::False)
    {
        glfwWindowHint(static_cast<int>(target), value);
    }
}

/// <summary>
/// Sets a <see cref="FramebufferHints" /> hint value. Has to be called
/// before calling <see cref="RenderWindow.Open" />
/// </summary>
/// <param name="target">The target.</param>
/// <param name="value">The value.</param>
void RenderWindow::WindowHint(const FramebufferHints &target, const int value)
{
    if (target == FramebufferHints::Stereo ||
            target == FramebufferHints::SRGBCapable ||
            target == FramebufferHints::DoubleBuffer)
    {
        if (static_cast<Hint>(value) == Hint::True ||
                static_cast<Hint>(value) == Hint::False)
        {
            glfwWindowHint(static_cast<int>(target), value);
        }
    }
    else if (value > 0 || value < std::numeric_limits<int>::max() ||
             static_cast<Hint>(value) == Hint::DontCare)
    {
        glfwWindowHint(static_cast<int>(target), value);
    }
}

/// <summary>
/// Sets a <see cref="ContextHints" /> hint value. Has to be called
/// before calling <see cref="RenderWindow.Open" />
/// </summary>
/// <param name="target">The target.</param>
/// <param name="value">The value.</param>
void RenderWindow::WindowHint(const ContextHints &target, const int value)
{
    if (target == ContextHints::ClientAPI)
    {
        if (static_cast<Hint>(value) == Hint::OpenGLAPI ||
                static_cast<Hint>(value) == Hint::OpenGLESAPI)
        {
            glfwWindowHint(static_cast<int>(target), value);
        }
    }
    else if (target == ContextHints::ContextRobustness)
    {
        if (static_cast<Hint>(value) == Hint::NoRobustness ||
                static_cast<Hint>(value) == Hint::NoResetNotification ||
                static_cast<Hint>(value) == Hint::LoseContextOnReset)
        {
            glfwWindowHint(static_cast<int>(target), value);
        }
    }
    else if (target == ContextHints::OpenGLForwardCompatibility ||
             target == ContextHints::OpenGLDebugContext)
    {
        if (static_cast<Hint>(value) == Hint::True ||
                static_cast<Hint>(value) == Hint::False)
        {
            glfwWindowHint(static_cast<int>(target), value);
        }
    }
    else if (target == ContextHints::OpenGLProfile)
    {
        if (static_cast<Hint>(value) == Hint::OpenGLAnyProfile ||
                static_cast<Hint>(value) == Hint::OpenGLCoreProfile ||
                static_cast<Hint>(value) == Hint::OpenGLCompatibilityProfile)
        {
            glfwWindowHint(static_cast<int>(target), value);
        }
    }
    else if (target == ContextHints::ContextReleaseBehavior)
    {
        if (static_cast<Hint>(value) == Hint::AnyReleaseBehavior ||
                static_cast<Hint>(value) == Hint::FlushReleaseBehavior ||
                static_cast<Hint>(value) == Hint::NoneReleaseBehavior)
        {
            glfwWindowHint(static_cast<int>(target), value);
        }
    }
    else if (target == ContextHints::ContextVersionMajor ||
             target == ContextHints::ContextVersionMinor ||
             target == ContextHints::ContextRevision)
    {
        glfwWindowHint(static_cast<int>(target), value);
    }
}

/// <summary>
/// Opens a GLFW window with the specified configuration.
/// </summary>
/// <param name="windowConfig">The window configuration.</param>
/// <param name="setPosition">
/// if set to <c>true</c> will set the specified
/// window position.
/// </param>
void RenderWindow::Open(WindowInfo windowConfig, bool setPosition,
                        GLFWmonitor * monitor, GLFWwindow * share)
{
    if (isOpen || !glfwInit()) { return; }

    windowInfo = std::move(windowConfig);
    windowHandler = glfwCreateWindow(windowInfo.width, windowInfo.height,
                                     windowInfo.title.c_str(), monitor, share);

    if (setPosition)
    {
        glfwSetWindowPos(windowHandler, windowInfo.x, windowInfo.y);
    }

    glfwSetWindowSize(windowHandler, windowInfo.width, windowInfo.height);
    glfwSetWindowTitle(windowHandler, windowInfo.title.c_str());
    glfwGetWindowPos(windowHandler, &windowInfo.x, &windowInfo.y);

    if (!windowHandler)
    {
        glfwTerminate();
        throw std::runtime_error("Couldn't create GLFW window.");
    }

    // successfull window open
    isOpen = true;
}

/// <summary>
/// Destroys the GLFW Window
/// </summary>
void RenderWindow::Destroy() const
{
    glfwDestroyWindow(windowHandler);
}

void RenderWindow::SetPosition(const int x, const int y)
{
    glfwSetWindowPos(windowHandler, x, y);
    windowInfo.x = x;
    windowInfo.y = y;
}

void RenderWindow::SetWindowSize(const int w, const int h)
{
    glfwSetWindowSize(windowHandler, w, h);
    windowInfo.width = w;
    windowInfo.height = h;
}

void RenderWindow::SetWindowTitle(const std::string &title)
{
    glfwSetWindowTitle(windowHandler, title.c_str());
    windowInfo.title = title;
}

void RenderWindow::SetAsCurrentContext() const
{
    glfwMakeContextCurrent(windowHandler);
}

/// <summary>
/// Looks up for input events.
/// </summary>
/// <param name="mode">The mode. Waiting or polling mode.</param>
void RenderWindow::Events(EventMode mode)
{
    switch (mode)
    {
        case EventMode::Poll: glfwPollEvents(); break;

        case EventMode::Wait: glfwWaitEvents(); break;

        default: break;
    }
}

/// <summary>
/// Indicates if the render window should close
/// or sets it to be closed.
/// </summary>
/// <param name="sendClose">
/// if set to <c>true</c> [send close event].
/// </param>
/// <returns>Close flag</returns>
int RenderWindow::ShouldClose(bool sendClose) const
{
    if (sendClose) { glfwSetWindowShouldClose(windowHandler, true); }

    return glfwWindowShouldClose(windowHandler);
}

void RenderWindow::SwapBuffers() const
{
    glfwSwapBuffers(windowHandler);
}

RenderWindow::RenderWindow() : windowHandler(nullptr), isOpen(false)
{
    // glfw error catching
    glfwSetErrorCallback(OnErrorCallback);

    if (!glfwInit())
    {
        std::cerr << "Error initializing GLFW..." << std::endl;
    }
}

RenderWindow::~RenderWindow()
{
    glfwDestroyWindow(windowHandler);
}
