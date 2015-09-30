/*
 *  .file doc/tutorial/oglplus/002_rgb_triangle.cpp
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#if OGLPLUS_DOCUMENTATION_ONLY

/** @page oglplus_tut_002_rgb_triangle RGB triangle
 *
 *  This tutorial is an extension of the @ref oglplus_tut_001_glut_glew
 *  tutorial, but it does not go through the code responsible for window
 *  initialization, OpenGL initialization and event handling.
 *
 *  For a full working code see the @oglplus_example{oglplus,002_triangle}
 *  file in the example directory.
 *
 *  @dontinclude oglplus/002_triangle.cpp
 *
 *  First we include a locale helper header that in turn includes
 *  @c GL3/gl3.h , GLEW or makes sure that the OpenGL symbols are defined.
 *  It is however not required for your applications to do it this way and if
 *  you wish you can use other means of defining the necessary OpenGL 3
 *  functions, constants, etc.
 *  @skipline #include <oglplus/gl.hpp>
 *
 *  Include everything from @OGLplus, except the image loaders
 *  and generators and geometric shape data generators.
 *  @skipline #include <oglplus/all.hpp>
 *
 *  The header declaring the base class for @OGLplus examples.
 *  @skipline #include "example.hpp"
 *
 *  Since this is an example that comes with the library it is implemented
 *  inside of the @c oglplus namespace.
 *  @skipline namespace oglplus {
 *  Real-life applications should instead do something like:
 *  @code
 *  using namespace oglplus;
 *  @endcode
 *  Doing this on the global scope may cause name clashes with other
 *  libraries especially in larger applications, so it may be a better idea
 *  to use fully qualified names or apply the @c using directive only in local
 *  scopes.
 *
 *  As before, the rendering code is encapsulated in the @c TriangleExample
 *  class which is derived from @c oglplus::Example.
 *  @until {
 *
 *  Most of the member variables are the same as in the previous tutorial.
 *  There is an instance of the @c oglplus::Context class wrapping the current context
 *  functions, a @c oglplus::VertexShader and @c oglplus::FragmentShader,
 *  a @c oglplus::Program which define the custom functionality of the rendering
 *  pipeline and a @c oglplus::VertexArray object managing the vertex
 *  data for the rendered triangle.
 *  @until VertexArray triangle;
 *
 *  The first difference between this and the previous tutorial
 *  is that there are two @c oglplus::Buffer objects, one
 *  for the vertex positions and the second for vertex colors.
 *  @until Buffer colors;
 *
 *  The public interface consists of a constructor and a couple of member
 *  functions.
 *  @until public:
 *
 *  The constructor takes no arguments and again sets the vertex shader source:
 *  @until vs.Source(
 *
 *  As before we specify the version of GLSL we wish to use with the @c version
 *  directive.
 *  @until #version
 *
 *  Declare the input variables of the vertex shader; @c Position
 *  for vertex positions and @c Color for vertex colors.
 *  @until in vec3 Color
 *
 *  This time there is also an explicitly declared output variable called
 *  @c vertColor.
 *  @until out vec4 vertColor
 *
 *  The main function again passes the value of the @c Position input variable
 *  to the next stages of the rendering pipeline via the implicitly declared
 *  @c gl_Position output variable:
 *  @until gl_Position =
 *
 *  We also pass the color value to the fragment shader, converting the RGB
 *  value into RGBA, with the alpha component set to 1.0.
 *  @until vertColor =
 *
 *  That's it for the vertex shader and now it can be compiled:
 *  @until vs.Compile
 *
 *  The fragment shader is similar to the one from the previous tutorial:
 *  @until #version
 *
 *  This time there is a input variable @c vertColor, matching the output
 *  variable from vertex shader in both name and type. The value of this
 *  variable will be smoothly interpolated across the surface of the rendered
 *  triangle.
 *  @until in vec4 vertColor
 *
 *  The only thing the shader does is that is assigns the @c vertColor value
 *  to the @c fragColor output variable:
 *  @until ");
 *
 *  We can now compile the fragment shader source code, attach both shaders to
 *  the shading program and try to link and use it.
 *  @until prog.Use()
 *
 *  Now we can start to specify the data for the individual vertex attributes
 *  of the triangle we're going to render. The first step as before is to
 *  @em bind the vertex array object managing the vertex data.
 *  @skipline triangle.Bind();
 *
 *  Since this is a simple example the vertex positions are hardcoded:
 *  @skipline GLfloat triangle_verts
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
 *  @skipline VertexArrayAttrib(prog
 *
 *  Now we repeat the steps above, for the second vertex attribute, specifying
 *  the colors of the individual vertices:
 *  @skipline GLfloat triangle_colors
 *  @until VertexArrayAttrib(prog
 *
 *  That's it for the vertex data specification. As the last step of initialization
 *  we specify the clear value for the color buffer and disable the depth test.
 *  @until }
 *
 *  The @c Reshape function gets called when the window is created and everytime
 *  the window's size changes. Here we tell the GL that the rendering viewport
 *  has changed.
 *  @skipline void Reshape
 *  @until }
 *
 *  This function redraws our scene and is basically the same as in the previous
 *  tutorial; we clear the color buffer and we tell the GL to draw
 *  triangle from the vertex data stored in the buffer objects tied to the currently
 *  bound VAO, which is still @c triangle, because we didn't bind any other VAO since
 *  initialization. More precisely we draw the triangle composed from 3 vertices
 *  starting at index 0 in the buffers.
 *  @skipline void Render
 *  @until }
 *
 *  Now the @c TriangleExample class is complete:
 *  @until };
 *
 *  The last thing in this example's source is the @c makeExample function.
 *  This function is called by shared internal piece of code (that comes
 *  with the examples) which does the initialization and event processing
 *  common to all the examples. @c makeExample creates an instance of our
 *  @c TriangleExample class. The common code then calls the event handler functions
 *  like @c Reshape and @c Render when appropriate. To see how this can be incorporated
 *  into a complete application see the standalone examples and the related tutorials.
 *  @skipline makeExample(
 */

#endif // OGLPLUS_DOCUMENTATION_ONLY
