/*
 *  .file doc/tutorial/oglplus/004_newton_fractal.cpp
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#if OGLPLUS_DOCUMENTATION_ONLY

/** @page oglplus_tut_004_rect RGB color mixing (lazy uniforms)
 *
 *  This tutorial shows the usage of lazily-initialized uniforms.
 *  Unlike regular @ref oglplus::Uniform "Uniforms" which require
 *  the program, which they reference to be compiled and linked
 *  before the @c Uniform is constructed, lazy uniforms only require
 *  that the @ref oglplus::Program "Program" instance exists - it does
 *  not have to be built yet. @ref oglplus::LazyUniform "LazyUniforms"
 *  postpone the initialization until the value of the uniform is set
 *  or some other operation is executed.
 *
 *  If you are not familiar with the basics of @OGLplus, it is recommended
 *  that you read the @ref oglplus_tut_002_rgb_triangle first for a more
 *  detailed introduction. You can also see the @ref oglplus_tut_001_glut_glew
 *  for a complete standalone example including the code responsible
 *  for window initialization, OpenGL initialization and event handling.
 *
 *  The new things that this tutorial covers are:
 *  - Lazy uniform variables
 *  - Untyped shaders
 *
 *
 *  For a full working code see the @oglplus_example{oglplus,004_rect}
 *  file in the example directory.
 *
 *  @dontinclude oglplus/004_rect.cpp
 *
 *  Like in the previous tutorials we first include the header that
 *  makes sure that the OpenGL symbols are defined and the one that
 *  includes everything from @OGLplus,
 *  @until #include <oglplus/all.hpp>
 *
 *  and also the header declaring the base class for @OGLplus examples.
 *  @skipline #include "example.hpp"
 *
 *  Just like the other examples that come with the library this one is also implemented
 *  inside of the @c oglplus namespace.
 *  @skipline namespace oglplus {
 *  Real-life applications should instead do something like:
 *  @code
 *  using namespace oglplus;
 *  @endcode
 *
 *  As before, the example code is encapsulated in the @c RectangleExample
 *  class which is derived from @c oglplus::Example.
 *  @until {
 *
 *  Most of the member variables are the same as in the previous tutorials.
 *  There is an instance of the @c oglplus::Context class wrapping the current
 *  context functions:
 *
 *  @skipline Context gl;
 *
 *  The following line is a little different from the previous tutorials.
 *  We could have used @c oglplus::VertexShader and a @c oglplus::FragmentShader
 *  types, but sometimes it is necessary to decide what kind of shader we want
 *  at run-time (based for example on some input data). This tutorial is not one
 *  of those time, but we take this opportunity to show the usage of untyped
 *  shaders:
 *
 *  @skipline Shader vs, fs;
 *
 *  next there is a @c oglplus::Program a @c oglplus::VertexArray object managing
 *  the vertex data for the rendered rectangle stored in @c oglplus::Buffer.
 *
 *  @until Buffer verts;
 *
 *  Now we declare three oglplus::LazyUniform variables which set
 *  the values of the @c RedCenter, @c BlueCender and @c GreenCenter uniform
 *  variables in the fragment shader below. The uniforms in the shader have
 *  the @c vec2 type - hence the @c Vec2f template parameter for @c LazyUniform.
 *
 *  @skipline LazyUniform
 *
 *  The public interface again consists of a constructor and a couple of member
 *  functions.
 *  @until public:
 *
 *  The constructor constructs the vertex and fragment shaders, using the
 *  oglplus::ShaderType:
 *
 *  @until , fs(
 *
 *  The following line is technically not necessary, but it is here to
 *  remind us that the program must be constructed before we try to initialize
 *  the uniforms:
 *
 *  @until , prog(
 *
 *  Now we can initialize the uniforms, the constructor takes a reference
 *  to a Program and the identifier of the variable that we wish to reference
 *  by the @c LazyUniform:
 *
 *  @until , blue_center(
 *
 *  Now we can set the source of the vertex shader. There is one new thing
 *  here - we are wrapping the source string in the @c StrLit class to tell
 *  the shader that it is initialized from a literal (which allows OGLplus and
 *  the compiler to do some optimizations):
 *
 *  @until "));
 *
 *  The vertex shader is complete and can be compiled.
 *  @skipline vs.Compile
 *
 *  The fragment shader is similar to the one from the previous tutorials.
 *  It has one input variable - the vertex coordinate that will be used
 *  in the newton fractal computations.
 *  @skipline fs.Source(
 *  @until in vec2 vertCoord
 *
 *  Also note the three uniform variables, that specify the centers
 *  of the red, green and blue 'lights' on the surface of the rendered
 *  rectangle:
 *  @until uniform vec2 RedCenter, GreenCenter, BlueCenter;
 *
 *  As before, the output of the shader is a vec4 representing the color
 *  of the fragment.
 *  @until out vec4 fragColor;
 *
 *  Then we calculate the distances of the fragment from the centers
 *  of the three 'lights' stored in the uniforms, store them in a vector
 *  and then use the values to colorize the fragment.
 *
 *  @until "));
 *
 *  We can now compile the fragment shader source code, attach both shaders to
 *  the shading program and try to link and use it.
 *  @skipline fs.Compile
 *  @until prog.Use()
 *
 *  Now we can start to specify the data for the individual vertex attributes
 *  of the rectangle we're going to render. The first step as before is to
 *  @em bind the vertex array object managing the vertex data.
 *  @skipline rectangle.Bind();
 *
 *  Again, since this is a simple example the coordinates are hardcoded.
 *  @skipline GLfloat rectangle_verts
 *  @until };
 *
 *  We bind the VBO for vertex positions to the @c ARRAY_BUFFER target.
 *  @skipline verts.Bind
 *
 *  The data are uploaded from client's memory to the server's memory
 *  by using the @c Buffer 's @c Data static function.
 *  @skipline Buffer::Data(
 *
 *  Then we use a oglplus::VertexAttribArray object referencing the @c Position
 *  input variable in the @c prog program, to tell OpenGL about the structure
 *  of the data in the currently bound VBO and to enable this vertex attribute.
 *  @skipline VertexAttribArray vert_attr
 *  @until vert_attr.Enable
 *
 *  Now we can specify the values of the uniform variables. Note that because
 *  we used lazily-initialized uniforms, the uniform variables in the compiled
 *  program are referenced only now. We use the @c Set function with two
 *  parameters since the underlying uniform type is @c Vec2f.
 *
 *  @skipline red_center.Set(
 *  @until blue_center.Set(
 *
 *  The rest of the tutorial is basically the same as before. We finish
 *  the constructor and we implement the @c Reshape and @c Render member
 *  functions and the @c makeExample function for creating an instance
 *  of the @c RectangleExample class.
 *
 *  @until } // namespace oglplus
 */

#endif // OGLPLUS_DOCUMENTATION_ONLY
