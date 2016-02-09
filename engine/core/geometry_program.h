#pragma once

#include "../types/program_shader.h"

#include <oglplus/uniform.hpp>
#include <oglplus/interop/glm.hpp>

class GeometryProgram : public ProgramShader
{
    protected:
        struct UniformMaterial
        {
            oglplus::Uniform<glm::vec3> ambient;
            oglplus::Uniform<glm::vec3> diffuse;
            oglplus::Uniform<glm::vec3> specular;
            oglplus::Uniform<glm::vec3> emissive;
            oglplus::Uniform<glm::vec3> transparent;

            oglplus::Uniform<float> opacity;
            oglplus::Uniform<float> shininess;
            oglplus::Uniform<float> shininessStrength;
            oglplus::Uniform<float> refractionIndex;

            oglplus::Uniform<unsigned int> useNormalsMap;
        };
        struct UniformMatrices
        {
            oglplus::Uniform<glm::mat4> modelView;
            oglplus::Uniform<glm::mat4> modelViewProjection;
            oglplus::Uniform<glm::mat4> model;
            oglplus::Uniform<glm::mat4> view;
            oglplus::Uniform<glm::mat4> projection;
            oglplus::Uniform<glm::mat4> normal;
        };
        void ExtractUniforms() override;
    public:
        // fragment shader uniforms
        UniformMaterial material;
        oglplus::UniformSampler diffuseMap;
        oglplus::UniformSampler specularMap;
        oglplus::UniformSampler normalsMap;
        oglplus::Uniform<float> alphaCutoff;
        // vertex shader uniforms
        UniformMatrices matrices;

        GeometryProgram();
        virtual ~GeometryProgram() {}
        GeometryProgram(GeometryProgram const &r) = delete;
        GeometryProgram &operator=(GeometryProgram const &r) = delete;
};
