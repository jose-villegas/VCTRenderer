#pragma once

#include <oglplus/program.hpp>

class ProgramShader;
class Material;
class Node;

class Renderer
{
    protected:
        template<class T>
        typename std::enable_if<std::is_base_of<ProgramShader, T>::value, void>::type
        CurrentProgram(T &program, bool use = true);
        template<class T>
        typename std::enable_if<std::is_base_of<ProgramShader, T>::value, T>::type
        &CurrentProgram() const;
        ~Renderer();
    public:
        virtual void Render() = 0;
        virtual void SetMatricesUniforms(const Node &node) const;
        virtual void SetMaterialUniforms(const Material &material) const;
        Renderer();
    private:
        ProgramShader * program;
};

template <class T>
typename std::enable_if<std::is_base_of<ProgramShader, T>::value, void>::type
Renderer::CurrentProgram(T &program, bool use)
{
    this->program = &program;

    if (use) { program.Use(); }
}

template <class T>
typename std::enable_if<std::is_base_of<ProgramShader, T>::value, T>::type
&Renderer::CurrentProgram() const
{
    return *static_cast<T *>(program);
}
