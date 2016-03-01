#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "program_shader.h"

void ProgramShader::Use() const
{
    program.Use();
}

void ProgramShader::Link()
{
    program.Link();
}

inline std::string SourceFromFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    std::string result((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    return result;
}

void ProgramShader::AttachShader(oglplus::ShaderType type,
                                 const std::string &filepath)
{
    const auto &source = SourceFromFile(filepath);
    auto shader = std::make_unique<oglplus::Shader>(type, source);
    shader->Compile();
    program.AttachShader(*shader);
    shaders.push_back(move(shader));
}
