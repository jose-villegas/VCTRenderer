#pragma once

namespace utils
{
    void ConsoleProgressBar(const std::string &title, int barWidth, int index,
                            int last);

    void SkipBOM(std::ifstream &in);

    std::string GetDirectoryPath(const std::string &filePath);

    void PrintDependenciesVersions();

    const GLFWvidmode * Desktop();
}