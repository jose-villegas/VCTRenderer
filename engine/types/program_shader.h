#pragma once

#include <vector>
#include <GL/glew.h>
#include <oglplus/program.hpp>
#include <oglplus/uniform.hpp>

/// <summary>
/// Base class for each program shader used in deferred rendering
/// </summary>
class ProgramShader
{
    protected:
        ~ProgramShader() {}
        friend class DeferredHandler;
        /// <summary>
        /// The fragment shader
        /// </summary>
        oglplus::FragmentShader fragmentShader;
        /// <summary>
        /// The vertex shader
        /// </summary>
        oglplus::VertexShader vertexShader;
        /// <summary>
        /// The program shader
        /// </summary>
        oglplus::Program program;
        /// <summary>
        /// Meant to be implemented in inheriting classes
        /// to extract uniform info such as binding location
        /// type, etc
        /// </summary>
        /// <param name="uType">Type of the uniform.</param>
        /// <param name="uName">Name of the uniform.</param>
        virtual void ExtractUniform(oglplus::SLDataType uType,
                                    std::string uName) = 0;
        void ExtractActiveUniforms();
    public:
        ProgramShader() {};
        void Use() const { program.Use(); }
};
