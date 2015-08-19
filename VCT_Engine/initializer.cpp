#include "stdafx.h"
#include "initializer.h"


Initializer::Initializer()
{
}

void Initializer::Start()
{
    std::cout << "Using: " << std::endl;

    // GLFW
    if(!glfwInit()) return;

    std::cout << "GLFW " << glfwGetVersionString() << std::endl;
    // Initialzie GLEW
    GLenum err = glewInit();

    if(err != GLEW_OK)
    {
        //Problem: glewInit failed, something is seriously wrong.
        std::cout << "glewInit failed, aborting: " << glewGetErrorString(err)
                  << std::endl;
    }

    std::cout << "GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    // OpenGL version
    std::cout << "OpenGL " << glGetString(GL_VERSION) << "s, GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

Initializer::~Initializer()
{
}
