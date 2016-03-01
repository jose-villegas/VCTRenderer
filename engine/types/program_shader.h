#pragma once

#include <oglplus/program.hpp>

/// <summary>
/// Base class for each program shader used in deferred rendering
/// </summary>
class ProgramShader
{
    public:
        /// <summary>
        /// Finalizes an instance of the <see cref="ProgramShader"/> class.
        /// </summary>
        virtual ~ProgramShader() {}
        /// <summary>
        /// Uses this program.
        /// </summary>
        void Use() const;
        /// <summary>
        /// Links the shader program given all the attached shaders.
        /// </summary>
        void Link();
        /// <summary>
        /// Attaches a new shader with the given type and source code.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <param name="filepath">The filepath.</param>
        void AttachShader(oglplus::ShaderType type, const std::string &filepath);
        /// <summary>
        /// Initializes a new instance of the <see cref="ProgramShader"/> class.
        /// </summary>
        ProgramShader() = default;
        // non copyable
        ProgramShader(ProgramShader const &r) = delete;
        ProgramShader &operator=(ProgramShader const &r) = delete;
        /// <summary>
        /// Extracts the uniforms associated with this program.
        /// The method is meant to be implemented by inheriting classes
        /// which represent different shaders.
        /// </summary>
        virtual void ExtractUniforms() = 0;
    protected:
        /// <summary>
        /// The program shader
        /// </summary>
        oglplus::Program program;
    private:
        /// <summary>
        /// The fragment shader
        /// </summary>
        std::vector<std::unique_ptr<oglplus::Shader>> shaders;
};
