#pragma once

#include <vector>
#include <oglplus/program.hpp>

/// <summary>
/// Base class for each program shader used in deferred rendering
/// </summary>
class ProgramShader
{
    public:
        ProgramShader() {};
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
        /// <summary>
        /// Meant to be implemented in inheriting classes
        /// to extract uniform info such as binding location
        /// type, etc
        /// </summary>
        /// <param name="uType">Type of the uniform.</param>
        /// <param name="uName">Name of the uniform.</param>
        void Build(const std::string &vsFilePath, const std::string &fsFilePath);
        void Use() const;
        // non copyable
        ProgramShader(ProgramShader const &r) = delete;
        ProgramShader &operator=(ProgramShader const &r) = delete;
    protected:
        virtual void ExtractUniforms() = 0;
    private:
        static std::string ProgramShader::SourceFromFile(const std::string &filePath);
};
