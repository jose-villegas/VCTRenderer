#include "stdafx.h"
#include "external_initializer.h"
#include "..\interface\imgui\imgui.h"
#include "FreeImage.h"
#include "assimp\Importer.hpp"
#include "assimp\version.h"

ExternalInitializer::ExternalInitializer()
{
}

void ExternalInitializer::InitializeContextDependant(bool
        printLibInfo /*= true*/)
{
    // Initialzie GLEW
    GLenum err = glewInit();

    if(err != GLEW_OK)
    {
        //Problem: glewInit failed, something is seriously wrong.
        std::cout << "glewInit failed, aborting: " << glewGetErrorString(err)
                  << std::endl;
    }

    // glew context initialize pronunces enum_error, catch it before oglplus
    glGetError();

    // version strings
    if(printLibInfo)
    {
        std::cout << "OpenGL " << glGetString(GL_VERSION) << "s, GLSL "
                  << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "GLEW " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Ocornut's IMGUI " << ImGui::GetVersion() << std::endl;
    }
}

void ExternalInitializer::Initialize(bool printLibInfo /*= true*/)
{
    // GLFW
    if(!glfwInit()) return;

    if(printLibInfo)
    {
        std::cout << "GLFW " << glfwGetVersionString() << std::endl;
        std::cout << "Assimp " << aiGetVersionMajor() << "." << aiGetVersionMinor() <<
                  std::endl;
        std::cout << "FreeImage " << FreeImage_GetVersion() << std::endl;
    }
}

ExternalInitializer::~ExternalInitializer()
{
}
