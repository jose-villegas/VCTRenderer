/*
 *  .file doc/tutorial/oglplus/001_glut_glew.cpp
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#if OGLPLUS_DOCUMENTATION_ONLY

/** @page oglplus_tut_001_glut_glew Basic usage with GLUT and GLEW
 *
 *  This tutorial shows a simple @OGLplus application that is using the GLUT
 *  (GL Utility Toolkit) library to initialize OpenGL, create a window,
 *  the default rendering context and the default framebuffer and the GLEW
 *  (GL Extension Wrangler) library which allows to use
 *  functions from the  OpenGL 3 (or higher) API and the OpenGL extensions.
 *  The application creates a single window and renders into it a red triangle
 *  on a black background. It involves only the very basic things necessary
 *  to draw something.
 *  For a full working code see the @oglplus_example{standalone,001_triangle_glut_glew}
 *  file in the example directory.
 *
 *  @dontinclude standalone/001_triangle_glut_glew.cpp
 *
 *  First we include two headers from the C++ standard library; we are going
 *  to use C assertions and C++ input/output streams.
 *  @until #include <iostream>
 *
 *  Include the main header of the GLEW library, which declares the
 *  OpenGL 3 function prototypes, typedefs, enumerations, etc.
 *  @until #include <GL/glew.h>
 *
 *  Include the main header of the GLUT library. GLUT defines functions
 *  for GL initialization, window creation, user input and event handling, etc.
 *  @until #include <GL/glut.h>
 *
 *  Include the @OGLplus's main header file. Before this
 *  file is included the OpenGL 3 function prototypes and other symbols must
 *  already be declared. As said above, we use the GLEW library to achieve
 *  it in this tutorial, there are however also other ways of doing it.
 *  @until #include <oglplus/all.hpp>
 *
 *  The @OGLplus -related code that does the actual rendering is encapsulated
 *  in the @c Example class. It contains objects that store the shading
 *  program and its individual shaders, objects storing and managing the
 *  vertex data and a wrapper around the current OpenGL context.
 *  @until {
 *
 *  First let's have a look at the data members of the @c Example class:
 *  @until private:
 *
 *  The @c Context class encapsulates the current context functionality.
 *  It mainly wraps functions that are not related to other OpenGL objects
 *  like shaders, programs, buffers, textures, queries and so on.
 *  @until oglplus::Context gl;
 *
 *  There is a vertex shader object that usually handles vertex
 *  attribute transformations and may pass or emit other per-vertex data
 *  to the following stages of the OpenGL's pipeline.
 *  The shader will be invoked once per every vertex in the rendered primitives.
 *  @until oglplus::VertexShader vs;
 *
 *  Since this is a very simple example that does not involve hardware tesselation
 *  nor any on-the-fly geometry modification or construction the next stage
 *  in the shading program is the fragment shader. In a simplistic view
 *  (which is sufficient for the purposes of this tutorial) a fragment shader is
 *  responsible for determining the color of a single pixel on
 *  the currently rendered polygon, line or point.
 *  A fragment shader is basically invoked once per every pixel on the rendered
 *  primitive.
 *  @until oglplus::FragmentShader fs;
 *
 *  The vertex and fragment shader are combined into a shading program. Such
 *  program is executed by the GPU and its individual @em stages handle the
 *  whole rendering process from the point where vertex attributes are consumed,
 *  transformed, possibly tessellated, assembled into the final primitives
 *  clipped, etc. until the point where they are drawn into a framebuffer.
 *  Many of these tasks are driven by the program's shaders.
 *  @until oglplus::Program prog;
 *
 *  Declare a vertex array object (VAO) for the triangle that
 *  will be rendered. A VAO encapsulates the state related to the definition
 *  of data that will be used by the vertex processor to draw a sequence of
 *  primitives that usually form a more complex geometric shape.
 *  @until oglplus::VertexArray triangle;
 *
 *  The actual vertex data is stored in (vertex) buffer objects (VBOs).
 *  This example uses only the positions of the vertices, thus there
 *  is only a single buffer. In cases where a single vertex has multiple
 *  attributes (position, colors, texture coordinates, normal, tangential,
 *  and binormal vectors, etc.) there would be multiple buffers and these
 *  buffers would be collected in a VAO. The buffer objects represent data
 *  stored in the server's memory.
 *  @until oglplus::Buffer verts;
 *
 *  The public interface of the @c Example consists of a constructor and
 *  one member function.
 *  @until public:
 *
 *  The constructor does not take any arguments and it is responsible for
 *  initializing the private members described above.
 *  @until {
 *
 *  The constructor uses several functions and types from the @c oglplus
 *  namespace. One might do this on the global scope in simple applications
 *  but in complex ones it is usually a better idea to do this only in
 *  limited scopes or use fully qualified names to limit the possibility
 *  of name clashes with symbols from other libraries.
 *  @until using namespace oglplus;
 *
 *  Set the source code for out vertex shader.
 *  @skipline vs.Source("
 *
 *  Specify which version of the OpenGL Shading language (GLSL)
 *  we are using;
 *  @until #version
 *
 *  Specify the @c Position input variable of the vertex shader.
 *  Since the vertex shader is the first stage of the rendering pipeline,
 *  the values of the input variables of vertex shaders are consumed from the
 *  vertex attribute values which can be stored in vertex buffer objects (VBOs).
 *  @until in vec3 Position;
 *
 *  The @c main function of the vertex shader has the following signature:
 *  @until {
 *
 *  This example does not do any explicit transformations of the rendered
 *  primitives and thus the vertex shader only passes the value of the input
 *  variable @c Position to the pre-defined output variable @c gl_Position,
 *  @until gl_Position
 *
 *  and this concludes the vertex shader source. Of course the source text
 *  does not have to be specified inline in the C++ source code, it can be
 *  for example loaded from a separate file.
 *  @until ");
 *
 *  The next step is to compile the shader. If something goes wrong the
 *  @c Compile() member function throws an exception with full diagnostic
 *  output from the shader compiler.
 *  @skipline vs.Compile();
 *
 *  Now we set the fragment shader source,
 *  @skipline fs.Source(" \
 *
 *  where we specify the version of GLSL that we wish to use,
 *  @until #version
 *
 *  and we define an output variable of type vec4 named @c fragColor:
 *  @until out vec4
 *  Since we didn't specify anything else, GL will take the value
 *  of this variable written by every invocation of the shader
 *  and use it as the color of the individual fragments for which
 *  the shader was invoked.
 *
 *  The @c main function has again the same signature,
 *  @until {
 *  and again, since this is a simple example the only thing that this
 *  shader does is, that is "paints" every fragment red. We do not do any
 *  lighting, texturing, etc. here.
 *  @until fragColor =
 *
 *  This concludes the shader source,
 *  @until ");
 *
 *  and we can compile it too, just like the vertex shader above:
 *  @skipline fs.Compile();
 *
 *  If the functions above did not throw any exceptions, we can now attach
 *  the compiled shaders to the shading program:
 *  @skipline prog.AttachShader(vs);
 *  @until    prog.AttachShader(fs);
 *
 *  Try to link and use the program. If the linking process fails
 *  it throws an exception with output from the shading program linker
 *  with information about the cause of the error.
 *  @skipline prog.Link();
 *  @until    prog.Use();
 *
 *  Input the vertex data for the rendered triangle. We start by
 *  @em binding the VAO. This will cause that the following operations
 *  related to vertex data specification will affect the @c triangle object.
 *  @skipline triangle.Bind();
 *
 *  Define an array of float values for vertex positions.
 *  There are 3 values per each vertex (the x, y and z coodrinate) and since
 *  we are rendering a triangle there are three vertices, lying on the z-plane
 *  with the following x and y coordinates [0, 0], [1, 0] and [0, 1]:
 *  @until };
 *
 *  @em Bind the VBO that will hold the vertex data in server memory
 *  to the @c ARRAY_BUFFER @em target (or binding point). Since we've currently
 *  bound the @c triangle VAO, this will attach the @c verts buffer object
 *  to the @c triangle vertex array object.
 *  @skipline verts.Bind(Buffer::Target::Array);
 *
 *  The data from the @c triangle_verts array in client memory can be uploaded
 *  to the server memory managed by the @c verts VBO by calling the static
 *  function Buffer::Data on the same binding point that the @c verts buffer
 *  is currently bound to.
 *  @skipline Buffer::Data
 *  @until );
 *
 *  To tell OpenGL that the value of the @c Position input variable of the
 *  vertex shader should be taken from the @c verts VBO and to supply it
 *  with some additional information about the organization of the data
 *  in the VBO we use a temporary @c VertexAttribArray object. Such objects
 *  reference input variables of shading programs, thus during construction
 *  of @c vert_attr we say that we want to reference a variable named
 *  @c Position in the shading program @c prog. Since @c verts is the currently
 *  bound VBO to the @c ARRAY_BUFFER target, the operations on the @c vert_attr
 *  vertex attribute array object will also operate on this buffer.
 *  @skipline VertexArrayAttrib vert_attr(prog, "Position");
 *
 *  Specify that there are three values per-vertex (3 coordinates)
 *  and that the data is of @c GLfloat type.
 *  @until vert_attr.Setup<GLfloat>(3);
 *
 *  Enable this vertex attribute array,
 *  @until vert_attr.Enable();
 *  and we are done specifying the vertex data.
 *
 *  The constructor ends with specifying the clear color and the clear depth
 *  value, i.e. the values that will be written to all pixels in the color
 *  and depth buffers when preparing them to render a new frame.
 *  @skipline gl.ClearColor
 *  @until }
 *
 *  The @c Display member function is called periodically and is responsible
 *  for redrawing the window contents.
 *  @skipline void Display(void)
 *  @until {
 *
 *  We'll be again using some symbols from the @c oglplus namespace:
 *  @skipline using namespace oglplus;
 *
 *  Before rendering the triangle we clear the color and depth buffers.
 *  Note that clearing the depth buffer is technically not necessary for this
 *  example since we are drawing a single primitive and thus we do not require
 *  a z-buffer, its here just to bring us up to speed and show how to clear
 *  multiple buffers at once.
 *
 *  @skipline gl.Clear().ColorBuffer().DepthBuffer();
 *
 *  @note Those who are concerned that the line above translates into
 *  something (rather inefficient) like,
 *  @code
 *  glClear(GL_COLOR_BUFFER_BIT);
 *  glClear(GL_DEPTH_BUFFER_BIT);
 *  @endcode
 *  don't worry. It in fact translates into something like this:
 *  @code
 *  GLenum bits = 0;
 *  bits |= GL_COLOR_BUFFER_BIT;
 *  bits |= GL_DEPTH_BUFFER_BIT;
 *  glClear(bits);
 *  @endcode
 *  which a good compiler can very easily optimize into:
 *  @code
 *  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 *  @endcode
 *
 *  Use the @c DrawArrays command to draw some @c Triangles from the
 *  vertex data managed by the currently bound VAO (which is still @c triangle
 *  since we did not bind any other VAO). More precisely we want to draw
 *  3 vertices starting at index 0 from the data stored in the buffers attached
 *  to this VAO.
 *  @skipline gl.DrawArrays
 *
 *  With this is we finish the @c Display function and the @c Example class.
 *  @until };
 *
 *  A class that manages a single instance of the @c Example class, defined
 *  above. It is necessary because the GLUT library is a little weird and
 *  does not allow to specify contextual data for its individual callbacks
 *  (functions handling various events like user input, window redraw, etc.)
 *  directly.
 *  @skipline class SingleExample
 *  @until {
 *
 *  The @c SingleInstance function manages and returns a reference to a pointer
 *  to the  @c Example type and when this pointer is initialized it points
 *  to the single instance of @c Example that does the rendering.
 *  @until }
 *
 *  Disable the copy-construction for this class, which
 *  would only mess things up.
 *  @until SingleExample
 *
 *  Define a constructor that instantiates a single @c Example and
 *  stores the address of the new instace in the pointer managed by
 *  @c SingleInstance. Note that only one instance of @c SingleExample
 *  can exist at a time.
 *  @skipline SingleExample
 *  @until }
 *
 *  The destructor deallocates the single instance of @c Example and resets
 *  the static pointer, when @c SingleExample is destroyed.
 *  @skipline ~SingleExample
 *  @until }
 *
 *  The static @c Display function is used as a callback for GLUT; it
 *  requires that the single instance is initialized and it call its
 *  Display member function and then tells GLUT to swap the front and back
 *  buffers once the rendering is finished.
 *  @skipline static void Display
 *  @until }
 *
 *  This finishes the declaration of the @c SingleExample helper class
 *  @until };
 *
 *  The main function of this example application starts by initializing the
 *  GLUT library:
 *  @skipline int main
 *  @until glutInit
 *
 *  Let it initialize the default framebuffer. We want double buffering,
 *  RGBA color buffer and a depth buffer.
 *  @skipline glutInitDisplayMode
 *
 *  Set the dimensions and the position and we let GLUT create the main
 *  window for this application.
 *  @until glutCreateWindow
 *
 *  Try to initialize the GLEW library. Note that a real-life application
 *  using GLEW needs to check if the desired functions are available. Otherwise
 *  it may crash with a segmentation fault or result in undefined behavior. Here
 *  the checks are omitted for the sake of simplicity.
 *  @skipline glewInit
 *  @until {
 *
 *  We use @c SingleExample to instantiate the single instance of the
 *  @c Example class (this will cause the @c Example 's constructor to be
 *  called, initializing the shaders, program, VAO and VBO, etc. as described
 *  above).
 *  @skipline SingleExample example
 *
 *  Tell GLUT to use @c SingleExample 's @c Display function as the
 *  display callback function and we start the main loop, which will start
 *  event processing and redrawing of the window, which in turn will result
 *  in calling of the @c Example 's @c Display function.
 *  If everything goes OK we exit the program with code 0.
 *  @until return 0;
 *
 *  If an exception was thrown
 *  by @c oglplus during the initialization or rendering, it will be caught here, some diagnostic info
 *  will be printed to error output and the application will
 *  exit with code 1.
 *  We end up here also if the initialization of the GLEW library failed
 *  for some reason.
 *  @until return 1;
 *
 *  This concludes the @c main function and our example's souce code.
 *  @until }
 */

#endif // OGLPLUS_DOCUMENTATION_ONLY
