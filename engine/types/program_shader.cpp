#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "program_shader.h"

void ProgramShader::Use() const
{
    program->Use();
}

ProgramShader::ProgramShader(const std::string &vsFilePath,
                             const std::string &fsFilePath)
{
    using namespace oglplus;

    if (!vertexShader)
    {
        vertexShader = std::make_unique<Shader>(ShaderType::Vertex);
    }

    if (!fragmentShader)
    {
        fragmentShader = std::make_unique<Shader>(ShaderType::Fragment);
    }

    if (!program) { program = std::make_unique<Program>(); }

    vertexShader->Source(SourceFromFile(vsFilePath));
    vertexShader->Compile();
    fragmentShader->Source(SourceFromFile(fsFilePath));
    fragmentShader->Compile();
    program->AttachShader(*vertexShader);
    program->AttachShader(*fragmentShader);
    program->Link();
}

std::string ProgramShader::SourceFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    std::string result((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    return result;
}
