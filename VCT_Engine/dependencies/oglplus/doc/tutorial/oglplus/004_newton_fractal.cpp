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

/** @page oglplus_tut_004_newton_fractal Newton fractal
 *
 *  This tutorial shows how to render a rectangle with a newton fractal
 *  shader covering the whole screen.
 *  If you are not familiar with the basics of @OGLplus, it is recommended
 *  that you read the @ref oglplus_tut_002_rgb_triangle first for a more
 *  detailed introduction. You can also see the @ref oglplus_tut_001_glut_glew
 *  for a complete standalone example including the code responsible
 *  for window initialization, OpenGL initialization and event handling.
 *
 *  The new things that this tutorial covers are:
 *  - More complex fragment shader
 *  - Uniform variables
 *
 *
 *  For a full working code see the @oglplus_example{oglplus,004_newton}
 *  file in the example directory.
 *
 *  @dontinclude oglplus/004_newton.cpp
 *
 *  First we include a helper header that in turn includes
 *  @c GL3/gl3.h or makes sure that the OpenGL symbols are defined.
 *  @skipline #include <oglplus/gl.hpp>
 *
 *  The next line includes everything from @OGLplus, except the image loaders
 *  and generators and geometric shape data generators.
 *  @skipline #include <oglplus/all.hpp>
 *
 *  Now we include the header declaring the base class for @OGLplus examples.
 *  @skipline #include "example.hpp"
 *
 *  Just like the other examples that come with the library this one is also implemented
 *  inside of the @c oglplus namespace.
 *  @skipline namespace oglplus {
 *  Real-life applications should instead do something like:
 *  @code
 *  using namespace oglplus;
 *  @endcode
 *  Doing this however on the global scope may cause name clashes with other
 *  libraries especially in larger applications, so it may be a better idea
 *  to use fully qualified names or apply the @c using directive only in local
 *  scopes.
 *
 *  As before, the example code is encapsulated in the @c RectangleExample
 *  class which is derived from @c oglplus::Example.
 *  @until {
 *
 *  Most of the member variables are the same as in the previous tutorials.
 *  There is an instance of the @c oglplus::Context class wrapping the current context
 *  functions, a @c oglplus::VertexShader and @c oglplus::FragmentShader,
 *  a @c oglplus::Program which define the custom functionality of the rendering
 *  pipeline and a @c oglplus::VertexArray object managing the vertex
 *  data for the rendered rectangle.
 *  @until VertexArray rectangle;
 *
 *  In this example we'll need just one @c Buffer object - to store
 *  the rectangle vertex positions.
 *  @until Buffer verts;
 *
 *  The public interface consists of a constructor and a couple of member
 *  functions.
 *  @until public:
 *
 *  The constructor takes no arguments and again sets the vertex shader source,
 *  which is same as in the previous tutorials. It just passes the vertex positions
 *  down the pipeline without any significant transformations.
 *  @until ");
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
 *  This time there are two uniform variables, that specify the colors
 *  of the gradient used for the colorization of the fractal.
 *  @until uniform vec3 Color1, Color2;
 *
 *  As before, the output of the shader is a vec4 representing the color
 *  of the fragment.
 *  @until vec4 fragColor;
 *
 *  We are going to visualise the newton fractal for the f(x) = x^3 - 1 polynomial.
 *  The next function computes the value of this polynomial at the specified
 *  coordinate on the @em complex plane (we use a vec2 as a complex number, for
 *  both the argument and the result).
 *  @skipline vec2 f
 *  @until }
 *
 *  We'll also need the values of the derivative of this polynomial (which is
 *  f'(x) = 3x^2) at specified complex coordinates which is what the @c df function
 *  computes.
 *  @skipline vec2 df
 *  @until }
 *
 *  And we'll need to divide two comples numbers - hence the @c cdiv function:
 *  @skipline vec2 cdiv
 *  @until }
 *
 *  The Newton or (Newton-Raphson) method was originaly devised for the finding
 *  of successivelly better approximations to the roots of real valued functions.
 *  The iterative nature of the algorithm can also be used for fractal image
 *  rendering. There are various methods for the colorization of the final image
 *  but the basic algorithm is usually the same:
 *  -# Assign a complex-value coordinate @c Z to every pixel on a 2d surface
 *  -# For every coordinate @c z calculate a sequence of numbers such that
 *    - Z0 = Z
 *    - Zi+1 = Zi - f(Zi)/f'(Zi)
 *  -# Stop when
 *    - the distance between Zi+1 and Zi is smaller than a specified constant
 *    - the number of iterations has reached a specified maximum
 *
 *  @until fragColor = vec4
 *
 *  This example uses the number of iterations used to reach a good enough approximation
 *  to create a color gradient between the @c Color1 and @c Color2 values
 *  specified as uniform variables above.
 *
 *  @until ");
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
 *  Again since this is a simple example the coordinates are hardcoded.
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
 *  Then we use a oglplus::VertexArrayAttrib object referencing the @c Position
 *  input variable in the @c prog program, to tell OpenGL about the structure
 *  of the data in the currently bound VBO and to enable this vertex attribute.
 *  @skipline VertexArrayAttrib vert_attr
 *  @until vert_attr.Enable
 *
 *  That's it for the vertex data specification. Now we specify the values of
 *  the two uniform variables referenced by the fragment shader. The uniforms
 *  were both declared as @c vec3 in the shader code (i.e. a vector of three float
 *  values). The classes that @OGLplus provides for manipulation of shader
 *  input variables are templated and allow to set only values of the specified
 *  type. This provides additional compile-time type checking and allows to avoid
 *  type mismatches between the C++ program and the GLSL shader code which result
 *  in runtime errors. There are several ways how to set the value of a uniform
 *  variable. The @c Uniform specializations for simple types like @c float, @c int,
 *  @c uint, etc. (besides setting values of the specified scalar types)
 *  also allow to set the values of vectors and arrays of the same type.
 *  There are also specializations for the @c Vector and @c Matrix classes
 *  that can be used to specify only the values of vectors, matrices and arrays
 *  thereof and they are more generic.
 *  This tutorial uses a specialization of @c Uniform for @c GLfloat and its
 *  @c SetVector member function to set the value of the 3D float vectors that
 *  store the color values.
 *  @skipline Uniform<Vec3f>(prog, "Color1")
 *  @until Uniform<Vec3f>(prog, "Color2")
 *
 *  As the last step of initialization we disable depth testing since
 *  we do not need it in this example:
 *  @skipline gl.Disable(Capability::DepthTest)
 *  @until }
 *
 *  The @c Reshape function get called when the window is created and everytime
 *  the window's size changes. Here we tell the GL that the rendering viewport
 *  has changed.
 *  @skipline void Reshape
 *  @until }
 *
 *  This function redraws our scene and is basically the same as in the previous
 *  tutorials; (we don't clear any buffers here since it is not necessary) so
 *  we just tell the GL to draw a rectangle
 *  from the vertex data stored in the buffer objects tied to the currently
 *  bound VAO, which is still @c rectangle, because we didn't bind any other VAO since
 *  initialization. More preciselly we draw the rectangle composed from 4 vertices
 *  starting at index 0 in the buffers.
 *  @skipline void Render
 *  @until }
 *
 *  Now the @c RectangleExample class is complete:
 *  @until };
 *
 *  The last thing in this example's source is the @c makeExample function.
 *  This function is called by shared internal piece of code (that comes together
 *  with the examples) which does the initialization and event processing
 *  common to all the examples. @c makeExample creates an instance of our
 *  @c RectangleExample class. The common code then calls the event handler functions
 *  like @c Reshape and @c Render when appropriate. To see how this can be incorporated
 *  into a complete application see the standalone examples and the related tutorials.
 *  @skipline makeExample(
 *  @until }
 */

#endif // OGLPLUS_DOCUMENTATION_ONLY
