#include "stdafx.h"
#include "shader_program.h"

void ShaderInfo::CleanSource()
{
    this->shaderSource.clear();
}

void ShaderInfo::SourceFromFile(ShaderClass shaderType,
                                const std::string &sFilepath)
{
    this->shaderType = shaderType;
    std::ifstream shaderFile(sFilepath);
    shaderFile.seekg(0, std::ios::end);
    this->shaderSource.reserve(shaderFile.tellg());
    shaderFile.seekg(0, std::ios::beg);
    this->shaderSource.assign((std::istreambuf_iterator<char>(shaderFile)),
                              std::istreambuf_iterator<char>());
}

void ShaderInfo::SourceFromString(ShaderClass shaderType,
                                  const std::string &sourceString)
{
    this->shaderType = shaderType;
    this->shaderSource.reserve(sourceString.size());
    this->shaderSource.assign(sourceString);
}

void OGLShader::Compile()
{
    if(this->shaderHandler || this->shaderSource.empty()) return;

    using namespace oglplus;
    ShaderType sType =
        this->shaderType == ShaderClass::Compute ? ShaderType::Compute
        : this->shaderType == ShaderClass::Fragment ? ShaderType::Fragment
        : this->shaderType == ShaderClass::Vertex ? ShaderType::Vertex
        : this->shaderType == ShaderClass::TessControl ? ShaderType::TessControl
        : this->shaderType == ShaderClass::TessEvaluation ? ShaderType::TessEvaluation
        : ShaderType::Geometry;
    // create shader with source
    this->shaderHandler.reset(new oglplus::Shader(sType,
                              (GLSLSource)this->shaderSource));
}
