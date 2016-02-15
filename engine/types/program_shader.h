#pragma once

#include <vector>
#include <oglplus/program.hpp>

/// <summary>
/// Base class for each program shader used in deferred rendering
/// </summary>
class ProgramShader
{
    public:
        virtual ~ProgramShader() {}
        friend class DeferredHandler;
        /// <summary>
        /// The fragment shader
        /// </summary>
        std::unique_ptr<oglplus::Shader> fragmentShader;
        /// <summary>
        /// The vertex shader
        /// </summary>
        std::unique_ptr<oglplus::Shader> vertexShader;
        /// <summary>
        /// The program shader
        /// </summary>
        std::unique_ptr<oglplus::Program> program;
        virtual void ExtractUniforms() = 0;
        void Use() const;
        // non copyable
        ProgramShader(const std::string &vsFilePath,
                      const std::string &fsFilePath);
        ProgramShader(ProgramShader const &r) = delete;
        ProgramShader &operator=(ProgramShader const &r) = delete;
    private:
        static std::string ProgramShader::SourceFromFile(const std::string &filePath);
};
