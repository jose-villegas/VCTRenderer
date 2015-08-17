// VCT_Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "render_window.h"
#include "initializer.h"

int main(int argc, char* argv[])
{
    RenderWindow renderWindow;
    Initializer initializer;
    // open window and set rendering context
    renderWindow.Open();
    renderWindow.SetAsCurrentContext();
    // initialize context dependant external libs
    initializer.Start();

    // draw
    while(!glfwWindowShouldClose(renderWindow.Handler()))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(renderWindow.Handler(), &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(renderWindow.Handler());
        glfwPollEvents();
    }

    return 0;
}

