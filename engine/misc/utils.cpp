#include "utils.h"

#include "../3rdparty/imgui/imgui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/version.h>
#include <FreeImage.h>
#include <iostream>
#include <fstream>

void Utils::ConsoleProgressBar(const std::string &title, int barWidth,
                               int index,
                               int last)
{
    std::cout << title + " [";
    auto progress = (1.0f / last) * (index + 1);
    auto pos = int(barWidth * progress);

    for (auto i = 0; i < barWidth; ++i)
    {
        if (i < pos) { std::cout << "="; }
        else if (i == pos) { std::cout << ">"; }
        else { std::cout << " "; }
    }

    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

std::string Utils::GetDirectoryPath(const std::string &filePath)
{
    auto pos = filePath.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : filePath.substr(0, pos);
}

void Utils::PrintDependenciesVersions()
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

const GLFWvidmode * Utils::Desktop()
{
    return glfwGetVideoMode(glfwGetPrimaryMonitor());
}

void Utils::SkipBOM(std::ifstream &in)
{
    char test[3] = { 0 };
    in.read(test, 3);

    if (static_cast<unsigned char>(test[0]) == 0xEF &&
            static_cast<unsigned char>(test[1]) == 0xBB &&
            static_cast<unsigned char>(test[2]) == 0xBF)
    {
        return;
    }

    in.seekg(0);
}
