// VCT_Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "render_window.h"
#include "initializer.h"
#include "scene/scene.h"
#include "interface/interface.h"
#include "interface/imgui/imgui.h"

int main(int argc, char* argv[])
{
    RenderWindow renderWindow;
    Interface gui;
    Initializer initializer;
    // open window and set rendering context
    renderWindow.Open();
    renderWindow.SetAsCurrentContext();
    // initialize context dependant external libs
    initializer.Start();
    // set interface to current renderwindow
    gui.Initialize(renderWindow);
    ImVec4 clear_color = ImColor(114, 144, 154);
    bool show_test_window = true;
    bool show_another_window = false;

    // draw
    while(!glfwWindowShouldClose(renderWindow.Handler()))
    {
        glfwPollEvents();
        gui.NewFrame();
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);

            if(ImGui::Button("Test Window")) show_test_window ^= 1;

            if(ImGui::Button("Another Window")) show_another_window ^= 1;

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        gui.Render();
        glfwSwapBuffers(renderWindow.Handler());
    }

    return 0;
}

