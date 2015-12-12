#include "engine_shader_program.h"

/// <summary>
/// Calls <see cref="ExtractUniform"> virtual method per
/// every active uniform in the shader program
/// </summary>
void EngineShaderProgram::ExtractActiveUniforms()
{
    auto uRange = program.ActiveUniforms();

    for (unsigned int i = 0; i < uRange.Size(); i++)
    {
        auto uName = uRange.At(i).Name();
        auto uType = uRange.At(i).Type();
        // call virtual implemented virtual method
        ExtractUniform(uType, uName);
    }
}

