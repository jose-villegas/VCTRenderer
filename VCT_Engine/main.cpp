// VCT_Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "scene/scene.h"
#include "util/initializer.h"
#include "interface/render_window.h"
#include "interface/vct_interface.h"

int main(int argc, char* argv[])
{
    RenderWindow renderWindow;
    VCTInterface gui;
    Initializer initializer;
    oglplus::Context gl;
    // initialize external dependencies
    initializer.ExternalLibs();
    // open window and set rendering context
    renderWindow.Open();
    renderWindow.SetAsCurrentContext();
    // initialize context dependant external libs
    initializer.ContextDependant();
    // set interface to current renderwindow
    gui.Initialize(renderWindow);
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // draw
    while(!glfwWindowShouldClose(renderWindow.Handler()))
    {
        glfwPollEvents();
        gui.Draw();
        gl.Clear().ColorBuffer().DepthBuffer(); glClear(GL_COLOR_BUFFER_BIT);
        gui.Render();
        glfwSwapBuffers(renderWindow.Handler());
    }

    return 0;
}

