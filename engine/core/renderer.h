#pragma once

#include "../util/single_active.h"

class RenderWindow;
class ProgramShader;
class Material;
class Node;

/// <summary>
/// A base class for renderers. All the rendering logic should reside in the
/// abstract method <see cref="Render">
/// </summary>
/// <seealso cref="SingleActive{Renderer}" />
class Renderer : public SingleActive <Renderer>
{
    protected:
        /// <summary>
        /// Returns the rendering window associated to this renderer. Usually
        /// the main window where the context was created.
        /// </summary>
        /// <returns></returns>
        RenderWindow &Window() const;
        /// <summary>
        /// Sets this program as the current active program in this renderer.
        /// </summary>
        /// <param name="program">The program.</param>
        /// <param name="use">if set to <c>true</c> [use].</param>
        /// <returns></returns>
        template<class T>
        typename std::enable_if<std::is_base_of<ProgramShader, T>::value, void>::type
        CurrentProgram(T &program, bool use = true);
        /// <summary>
        /// Returns the current active program associated to this renderer.
        /// </summary>
        /// <returns></returns>
        template<class T>
        typename std::enable_if<std::is_base_of<ProgramShader, T>::value, T>::type
        &CurrentProgram() const;
    public:
        static bool UseFrustumCulling;
        /// <summary>
        /// Rendering logic
        /// </summary>
        virtual void Render() = 0;
        /// <summary>
        /// Sets the program matrices uniforms. Empty method, optionally
        /// implemented by inheriting classes.
        /// </summary>
        /// <param name="node">The node.</param>
        virtual void SetMatricesUniforms(const Node &node) const;
        /// <summary>
        /// Sets the program material uniforms. Empty method, optionally
        /// implemented by inheriting classes.
        /// </summary>
        /// <param name="material">The material.</param>
        virtual void SetMaterialUniforms(const Material &material) const;
        /// <summary>
        /// Initializes a new instance of the <see cref="Renderer"/> class.
        /// </summary>
        Renderer();
        explicit Renderer(RenderWindow * window);
        /// <summary>
        /// Finalizes an instance of the <see cref="Renderer"/> class.
        /// </summary>
        virtual ~Renderer();
    private:
        /// <summary>
        /// The rendering window associated to this renderer. Usually the main
        /// window where the context was created.
        /// </summary>
        RenderWindow * window;
        /// <summary>
        /// A direct reference to an active program shader so it can be
        /// easily used in any inheriting class's logic.
        /// </summary>
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
