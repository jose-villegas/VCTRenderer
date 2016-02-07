#include "program_shader.h"

void ProgramShader::Build(const std::string &vsFilePath,
                          const std::string &fsFilePath)
{
    vertexShader.Source(SourceFromFile(vsFilePath));
    vertexShader.Compile();
    fragmentShader.Source(SourceFromFile(fsFilePath));
    fragmentShader.Compile();
    program.AttachShader(vertexShader);
    program.AttachShader(fragmentShader);
    program.Link().Use();
    PassActiveUniforms();
}

std::string ProgramShader::SourceFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    std::string result((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    return result;
}

void ProgramShader::PassActiveUniforms()
{
    auto uRange = program.ActiveUniforms();

    for (unsigned int i = 0; i < uRange.Size(); i++)
    {
        // call virtual implemented virtual method
        ExtractUniform(uRange.At(i));
    }
}
