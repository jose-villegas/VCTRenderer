#pragma once

#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

/// <summary>
/// Contains all necessary uniforms for the <see cref="DeferredHandler"/>'s
/// geometry program for geometry pass.
/// </summary>
/// <seealso cref="ProgramShader" />
class GeometryProgram : public ProgramShader
{
    protected:
        struct UniformMaterial
        {
            oglplus::Uniform<glm::vec3> ambient;
            oglplus::Uniform<glm::vec3> diffuse;
            oglplus::Uniform<glm::vec3> specular;
            oglplus::Uniform<float> shininess;
            oglplus::Uniform<unsigned int> useNormalsMap;
        };
        struct UniformMatrices
        {
            oglplus::Uniform<glm::mat4> normal;
            oglplus::Uniform<glm::mat4> modelViewProjection;
        };
    public:
        // fragment shader uniforms
        UniformMaterial material;
        oglplus::UniformSampler diffuseMap;
        oglplus::UniformSampler specularMap;
        oglplus::UniformSampler normalsMap;
        oglplus::Uniform<float> alphaCutoff;
        // vertex shader uniforms
        UniformMatrices matrices;

        void ExtractUniforms() override;

        GeometryProgram() = default;
        virtual ~GeometryProgram();
        GeometryProgram(GeometryProgram const &r) = delete;
        GeometryProgram &operator=(GeometryProgram const &r) = delete;
};
