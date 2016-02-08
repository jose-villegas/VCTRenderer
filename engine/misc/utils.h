#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Utils
{
    void ConsoleProgressBar(const std::string &title, int barWidth, int index,
                            int last);

    void SkipBOM(std::ifstream &in);

    std::string GetDirectoryPath(const std::string &filePath);

    void PrintDependenciesVersions();

    const GLFWvidmode * Desktop();

    inline std::string GetFileExtension(const std::string &sFilepath)
    {
        auto result = sFilepath.substr(sFilepath.find_last_of(".") + 1);
        return result == sFilepath ? "" : result;
    }
}