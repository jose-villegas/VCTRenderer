// VCT_Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "core\base.h"


int main(int argc, char* argv[])
{
    VCT_ENGINE::Base * engineCore = VCT_ENGINE::Base::Instance();
    // gl handler
    oglplus::Context gl;
    // black screen initiallly
    gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // render loop
    while(!glfwWindowShouldClose(engineCore->GetRenderWindow().Handler()))
    {
        glfwPollEvents();
        engineCore->GetUI().Draw();
        gl.Clear().ColorBuffer().DepthBuffer(); glClear(GL_COLOR_BUFFER_BIT);
        engineCore->GetUI().Render();
        glfwSwapBuffers(engineCore->GetRenderWindow().Handler());
    }

    return 0;
}

