/**
 *  @file oglplus/doc.hpp
 *  @brief Documentation-only declarations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_DOC_1107121519_HPP
#define OGLPLUS_DOC_1107121519_HPP

#include <oglplus/config.hpp>

#if OGLPLUS_DOCUMENTATION_ONLY

/// All definitions of OGLplus are nested in the oglplus namespace
namespace oglplus {

/** @mainpage OGLplus - a C++ wrapper for modern OpenGL®.
 *
 *  @image html oglplus.png
 *
 *  <A HREF="../index.html">OGLplus</A> is a header-only library which implements
 *  a thin object-oriented facade over the OpenGL® (version 3 and higher)
 *  C-language API. It provides wrappers which automate
 *  resource and object management and make the use of OpenGL
 *  in C++ safer and easier.
 *
 *  - @ref oglplus_features
 *    - @ref feat_auto_resource_management
 *    - @ref feat_type_safety
 *    - @ref feat_error_handling
 *    - @ref feat_interoperability_with_opengl
 *    - @ref feat_related_classes
 *    - @ref oglplus_object_description
 *    - @ref oglplus_configurability
 *  - <A HREF="../index.html">Home page</A>
 *  - @ref oglplus_what_it_is_not
 *  - @ref oglplus_screenshot_gallery
 *  - @ref oglplus_download
 *    - <A HREF="http://sourceforge.net/projects/oglplus/files/">Download page</A>
 *  - @ref oglplus_getting_started
 *    - @ref oglplus_tutorials
 *  - @ref oglplus_getting_it_going
 *  - @ref oglplus_supported_compilers
 *  - @ref oglplus_example_rationale
 *
 *  @section oglplus_features Features
 *
 *  @OGLplus adds the following functionality to the OpenGL® C API.
 *
 *  @subsection feat_auto_resource_management Automatic resource management
 *
 *  Applications using the OpenGL® library typically allocate and use, during their
 *  run-time, resources like buffers, queries, textures, shaders, programs,
 *  etc. from this library.
 *  @OGLplus uses the RAII idiom to automatically manage the lifetime
 *  of these resources. Classes like @ref oglplus::Buffer "Buffer",
 *  @ref oglplus::Texture "Texture", @ref oglplus::Query "Query",
 *  @ref oglplus::Shader "Shader", @ref oglplus::Program "Program",
 *  @ref oglplus::Framebuffer "Framebuffer",
 *  @ref oglplus::Renderbuffer "Renderbuffer",
 *  etc. automatically get a new OpenGL name
 *  for the wrapped object when an instance is constructed and it is freed
 *  when the instance is destroyed. Besides this basic functionality,
 *  the individual classes also implement member functions wrapping those
 *  calls which are related to the resource managed by the particular
 *  class.
 *
 *  When several instances of the same resource type should be created
 *  and freed at the same time, the @ref oglplus::Array "Array" template provides
 *  efficient means for doing so.
 *
 *  @subsection feat_type_safety Type safety
 *
 *  OpenGL uses the @c GLuint type as the namespace for instances of different
 *  types of resources (textures, shaders, programs, etc.) and thus it is very
 *  easy to make an error and use a name originally allocated for a shader
 *  as a parameter for a call expecting for example a name of a occlusion query,
 *  and such errors will not be caught by the compiler.
 *
 *  By implementing the wrapper classes @OGLplus gives a special meaning
 *  to the unsigned integer names and by their encapsulating and hiding
 *  from the library client it allows to avoid such errors.
 *
 *  Similarly the various enumerations are basically values of the @c GLenum
 *  type and are prone to the same kind of error. You can pass an incompatible
 *  enumerated value to a API function call. @OGLplus again allows to avoid these
 *  errors by separating the values into strongly typed enums and wrapping the
 *  basic OpenGL® API calls by functions accepting only parameters with matching
 *  enumerated types.
 *
 *  @subsection feat_error_handling Error handling and diagnostic
 *
 *  In cases where errors cannot be caught at compile-time by using
 *  the type system or by other means, @OGLplus checks for errors after
 *  the calls to OpenGL and throws an exception if an error occurs.
 *  All exceptions are derived from the @ref oglplus::Error "Error" class
 *  which provides information about the throw site of the exception
 *  (source file, line, @OGLplus function name, the OpenGL® C API call,
 *  etc. where the error occured) and optionally also some properties
 *  (string key and value) attached at the throw site or during the propagation
 *  of the exception. If the error is related to a
 *  @ref oglplus_object_description "described" object, then the object description
 *  can also be obtained for a more precise diagnostic.
 *
 *  @subsection feat_interoperability_with_opengl Interoperability with OpenGL® C API
 *
 *  As said above, @OGLplus by default encapsulates and hides the OpenGL names
 *  assigned to various objects (resources allocated at the OpenGL server).
 *  There are however situations where it might be necessary to get this
 *  "raw" resource handle.
 *
 *  @subsection feat_related_classes Additional CG related classes
 *
 *  Besides the classes wrapping OpenGL functionality @OGLplus also provides
 *  several classes which are related to computer graphics and indirectly
 *  to OpenGL, like @ref oglplus::Angle "Angle", @ref oglplus::Vector "Vector",
 *  @ref oglplus::Matrix "Matrix", @ref oglplus::BezierCurves "BezierCurves",
 *  and several others, but does not force you to use them if you don't want to.
 *
 *  @subsection oglplus_object_description Optional object description
 *
 *  Objects with OpenGL names (unsigned integers) can optionally have a textual
 *  description. This is useful for diagnostic purposes, for example in an application
 *  with lots of different shaders it is easier to track the source of compilation,
 *  linking or validation errors when the individual shader or program objects
 *  have a human-readable description which is attached to the exception raised
 *  by the error.
 *
 *  @subsection oglplus_configurability Configurability
 *
 *  If the overhead caused by the object descriptions or other run-time checks
 *  (which are useful during the development and/or debugging) is unacceptable
 *  for release builds of an application using @OGLplus, they can all be compiled
 *  away by setting the value of the @c #OGLPLUS_LOW_PROFILE preprocessor symbol.
 *  Individual features can be also enabled or disabled independently as required
 *  by setting the appropriate @ref oglplus_configuration_options.
 *
 *
 *  @section oglplus_what_it_is_not What OGLplus isn't
 *
 *  Like the OpenGL® C API, @OGLplus does not officially provide nor force you to use code
 *  related to creating of the default framebuffer. There are already
 *  several other libraries and frameworks which do it in both platform
 *  dependent and platform independent ways.
 *
 *  @OGLplus also does not include header file(s) declaring the OpenGL® 3 (or higher)
 *  function prototypes and enumerated values. It is the responsibility of the
 *  application to do so. There are several ways how to do it, see
 *  @ref oglplus_getting_it_going for some suggestions.
 *
 *   @section oglplus_getting_started Getting started
 *
 *  One of the best ways how to get yourself acquainted with the library is to
 *  look at the examples and to browse through the reference. Many of the examples
 *  are thoroughly commented and for some of them step-by-step
 *  @ref oglplus_tutorials "tutorials" are being added. Also see @ref oglplus_getting_it_going
 *  for a detailed help on building the examples and your own applications.
 */

/** @page oglplus_download Source download and repository
 *
 *  The source code can be downloaded from SourceForge at the following URL:
 *  <A HREF="http://sourceforge.net/projects/oglplus/files/">http://sourceforge.net/projects/oglplus/files/</A>.
 *
 *  Git users can get anonymous read-only access to the source repository
 *  by using the following command:
 *  @code
 *  git clone https://github.com/matus-chochlik/oglplus.git oglplus
 *  @endcode
 */

/** @page oglplus_getting_it_going Getting it going
 *
 *  @section oglplus_build_system Build system
 *
 *  OGLplus uses a CMake-based build/configuration @ref oglplus_cmake_config "system".
 *  The library itself is header only, which means that applications using it do not need to link
 *  to a compiled library, but need just to include the header files.
 *
 *  The build system handles several important tasks:
 *
 *   - Detects if the necessary things are installed and makes a site-configuration
 *     header file
 *   - Builds several additional, automatically generated headers
 *   - Installs all header files to a directory specified by the install prefix
 *   - Builds the examples and textures used by the examples (optional)
 *   - Builds and installs the documentation (optional)
 *
 *  @section oglplus_build_requirements Requirements
 *
 *   - C++11 compatible compiler.
 *
 *   - CMake.
 *
 *   - Doxygen is also required to build the documentation.
 *
 *   - Inkscape is required to convert textures used in examples from SVG to PNG.
 *     This is required only if the textures are not pre-built (typically
 *     when checked out from the repository, packaged releases are shipped with
 *     pre-built textures). Building of the textures is optional, they are not
 *     necessary when the building of examples is disabled.
 *
 *   - The GL/glcorearb.h header or GLEW. OGLplus does not define the OpenGL symbols
 *     (types, constants, functions, etc.) itself and therfore applications using
 *     it need to define them themselves (before including OGLplus). The examples
 *     currently need GLEW (at least version 1.9) or the GL/glcorearb.h header (available
 *     for download from http://www.opengl.org/registry/api/glcorearb.h) and a GL binary
 *     library exporting the OpenGL (3 or higher) functions.
 *     The build system detects the presence of GLEW or glcorearb.h and configures
 *     compilation and linking of the examples accordingly. If both are installed
 *     and the user does not specify otherwise GLEW is used.
 *
 *  -  libPNG. Some examples and some classes provided by OGLplus use libPNG to load
 *     PNG files. These are however not required for the general use of OGLplus,
 *     applications may use other means to load binary image files.
 *
 *
 *  @section oglplus_cmake_config CMake-based build configuration
 *
 *  The CMake script defines and uses several variables to modify the build
 *  configuration, which can be specified on the command-line when invoking
 *  @c cmake (with the -D option. see cmake manual for details):
 *
 *   - HEADER_SEARCH_PATHS [empty]: (semicolon-separated) list of paths
 *     to additional directories to search when looking for 3rd-party headers
 *     like GL/glew.h, GL/glcorearb.h, GL3/gl3.h, etc.
 *
 *   - OGLPLUS_NO_EXAMPLES [Off]: Do not build the examples and the textures.
 *
 *   - OGLPLUS_NO_DOCS [Off]: Do not build and install the documentation.
 *
 *
 *   @section oglplus_config_script User-friendly configuration script
 *
 *  The 'configure' script is a more user-friendly way to invoke cmake and specify
 *  additional parameters for the configuration process.
 *
 *  Some of the more important command-line options are described below:
 *
 *  - @c --prefix PATH: Specifies the installation prefix path for cmake (sets
 *     the value of the CMAKE_INSTALL_PREFIX variable). If this option is not
 *     specified, cmake's default prefix is used.
 *
 *  - @c --include-dir PATH: This options allows to specify additional directiories
 *     to search when looking for header files. It may be used multiple times
 *     to specify multiple directories.
 *
 *  - @c --library-dir PATH: This options allows to specify additional directiories
 *     to search when looking for binary libraries. It may be used multiple times
 *     to specify multiple directories.
 *
 *  See the @c --help option for the full description and detailed info on the usage
 *  of this script.
 *
 *  @section oglplus_building_own_apps Building your own applications
 *
 *  The library itself is header-only, i.e. all an application using it
 *  has to do is to include the necessary header files. The library does not need to be
 *  built nor linked to. The cmake-based build system can be used to install
 *  the necessary files to a platform-default or a user-specified directory.
 *
 *  One important thing that an application using @OGLplus needs to take care of
 *  is to make sure that the OpenGL® 3 (or higher) function prototypes, typedefs,
 *  preprocessor symbols, etc. are defined before including any @OGLplus header.
 *
 *  There are several different ways how to do this; one of the most convenient
 *  is to download the @c GL/glcorearb.h header file from
 *  <A HREF="http://www.opengl.org/registry/api/glcorearb.h">www.opengl.org/registry/api/glcorearb.h</A>
 *  and place it to a directory implicitly searched by the compiler or to add the
 *  @c -I/path/to/include directive to your @c CXXFLAGS. If the OpengGL dynamic library
 *  or shared object is located in a nonstandard location also update the @c LDFLAGS.
 *
 *  On Linux and similar systems using Bash as shell you can for example put
 *  the following to your ~/.bashrc file:
 *
 *  @code
 *  # if GL/glcorearb.h is in $HOME/include
 *  export CXXFLAGS="$CXXFLAGS -I$HOME/include"
 *  # if libGL.so is in $HOME/lib
 *  export LDFLAGS="$LDFLAGS -L$HOME/lib"
 *  @endcode
 *
 *  Some other alternatives include using GLEW or a similar library or framework
 *  which includes glcorearb.h or defines the necessary symbols itself. See the standalone
 *  examples for working applications using these alternative libraries.
 *
 *  @section oglplus_supported_compilers Supported compilers
 *
 *  @OGLplus is known to work with the following compilers:
 *  - GCC (versions 4.5.1, 4.6.0, 4.6.1).
 *  - MSVC 10 with some features (mostly related to variadic templates,
 *     initializer lists, and some others) disabled.
 *  - MSVC 11 with some features (mostly related to variadic templates
 *     and initializer lists) disabled.
 *  - MSVC 12 beta with some features disabled.
 *
 *  @note Please refer to the README.rst file for more up-to-date version of this page.
 */

/** @page oglplus_example_rationale Example rationale
 *
 *  @OGLplus does not officially provide means for creating of the
 *  default framebuffer nor any code related to window-system or user-input.
 *
 *  The example code is usually encapsulated in a class derived from the @c Example
 *  class and the build system selects and builds and links the piece of code doing
 *  the initial setup, cleanup and handling of events to the example code
 *  automatically.
 *  This way the examples are not too cluttered by things which are not directly
 *  related to OpenGL® and @OGLplus usage. The examples are numbered and ordered
 *  roughly by their "difficulty", from the easiest to the most complex.
 *
 *  In the example directory there are however also several standalone examples,
 *  showing complete applications which also contain the initialization and cleanup
 *  code, using various libraries like GLUT, GLEW, etc.
 */

/** @defgroup math_utils GC-related math utilities
 *
 *  This group contains classes and functions which do not directly deal with
 *  OpenGL, but are related to computer graphics, like angles, vectors, matrices,
 *  etc. The @OGLplus example make use these classes, but the library does not
 *  force you to use them if you don't want to.
 */

/** @defgroup shader_variables Shader variable access
 *
 *  Classes in this group are used for access to the format and values of
 *  shading language program variables, like uniforms and vertex attributes or
 *  vertex attribute arrays.
 */

/** @defgroup enumerations Strongly typed enumerations
 *
 *  These enumerations tie the OpenGL constants (defined as preprocessor
 *  symbols) into logical groups and are then used as parameter types
 *  in OpenGL function call wrappers instead of just plain @c GLenum
 *  or @c GLbitfield. This brings additional type safety and robustness
 *  to the application code and helps avoid subtle hard to track errors.
 */

/** @defgroup utility_classes Utility classes and functions
 */

/** @defgroup modifier_classes Modifier classes
 *
 *  Class templates in this group modify the usage or behavior of other
 *  classes, mostly @ref oglplus_object "objects". They for example
 *  allow to make temporary copies of objects which are only movable
 *  or to allocate and cleanup several instances of the same type
 *  at the same time.
 */

/// Unspecified internal type
/** This type does not actually exist and is only a placeholder for various
 *  internal implementation details not to be used directly by the end-users.
 */
struct Unspecified
{ };

/** @page oglplus_screenshot_gallery Screenshot gallery
 *
 *  @htmlinclude gallery.html
 */

/** @page dsa_objects Direct-State-Access (DSA) objects
 *
 *  DSA objects allow to use @ref oglplus_object "objects" that
 *  can to be bound to a OpenGL binding point or "target" without binding them.
 *  This includes objects like @ref oglplus::Buffer "Buffer,"
 *  @ref oglplus::Texture "Texture", @ref oglplus::Renderbuffer "Renderbuffer"
 *  or @ref oglplus::Framebuffer "Framebuffer"
 *  which have a target to which individual
 *  instances can be bound and operated on through the binding point.
 *  DSA objects implement (mostly) the same functions as their non-DSA counterparts
 *  but without the target parameter and the operations are invoked on the instances
 *  themselves.
 *
 *  For example to setup a texture object one might need to do the following:
 *  @code
 *  // create the texture
 *  Texture tex;
 *  // the binding point we'll be using to setup the texture
 *  Texture::Target tex_tgt = Texture::Target::_2D;
 *  tex.Bind(tex_tgt);
 *  {
 *    Texture::Image2D(tex_tgt, ...);
 *    Texture::GenerateMipmap(tex_tgt);
 *    Texture::MinFilter(tex_tgt, TextureMinFilter::Linear);
 *    Texture::MagFilter(tex_tgt, TextureMagFilter::Linear);
 *    Texture::WrapS(tex_tgt, TextureWrap::Repeat);
 *    Texture::WrapT(tex_tgt, TextureWrap::Repeat);
 *    Texture::SwizzleG(tex_tgt, TextureSwizzle::Red);
 *    Texture::SwizzleB(tex_tgt, TextureSwizzle::Red);
 *  }
 *  @endcode
 *
 *  The @c DSATexture class allows to do things more conveniently:
 *  @code
 *  // create the texture
 *  DSATexture dsa_tex;
 *  // specify the default target for this texture
 *  dsa_tex.target = Texture::Target::_2D;
 *  dsa_tex.Image2D(...);
 *  dsa_tex.GenerateMipmap();
 *  dsa_tex.MinFilter(TextureMinFilter::Linear);
 *  dsa_tex.MagFilter(TextureMagFilter::Linear);
 *  dsa_tex.WrapS(TextureWrap::Repeat);
 *  dsa_tex.WrapT(TextureWrap::Repeat);
 *  dsa_tex.SwizzleG(TextureSwizzle::Red);
 *  dsa_tex.SwizzleB(TextureSwizzle::Red);
 *  dsa_tex.Bind();
 *  @endcode
 *
 *  In order to be able to use the DSA objects it is necessary to include
 *  the appropriate header:
 *
 *  @code
 *  #include <oglplus/texture_dsa.hpp> // DSATexture
 *  #include <oglplus/buffer_dsa.hpp> // DSABuffer
 *  #include <oglplus/framebuffer_dsa.hpp> // DSAFramebuffer
 *  #include <oglplus/renderbuffer_dsa.hpp> // DSARenderbuffer
 *  #include <oglplus/vertex_array_dsa.hpp> // DSAVertexArray
 *  @endcode
 */

/** @page bound_objects Bound objects
 *
 *  Bound objects make the usage of @ref oglplus_object "objects" that
 *  can be bound to a OpenGL binding point or "target" easier.
 *  This includes objects like @ref oglplus::Buffer "Buffer,"
 *  @ref oglplus::Texture "Texture", @ref oglplus::Renderbuffer "Renderbuffer"
 *  or @ref oglplus::Framebuffer "Framebuffer"
 *  which have a target to which individual
 *  instances can be bound and operated on through the binding point.
 *  Generally @c Bound<Object> classes re-implement those functions of @c Object
 *  which have a @c target parameter of the @c Object::Target type. These
 *  re-implemented functions have all the other parameters of the original
 *  member functions, but lack the @c target parameter and supply it to the
 *  original function call automatically.
 *
 *  For example to setup a texture object one might need to do the following:
 *  @code
 *  // create the texture
 *  Texture tex;
 *  // the binding point we'll be using to setup the texture
 *  Texture::Target tex_tgt = Texture::Target::_2D;
 *  tex.Bind(tex_tgt);
 *  {
 *    Texture::Image2D(tex_tgt, ...);
 *    Texture::GenerateMipmap(tex_tgt);
 *    Texture::MinFilter(tex_tgt, TextureMinFilter::Linear);
 *    Texture::MagFilter(tex_tgt, TextureMagFilter::Linear);
 *    Texture::WrapS(tex_tgt, TextureWrap::Repeat);
 *    Texture::WrapT(tex_tgt, TextureWrap::Repeat);
 *    Texture::SwizzleG(tex_tgt, TextureSwizzle::Red);
 *    Texture::SwizzleB(tex_tgt, TextureSwizzle::Red);
 *  }
 *  @endcode
 *
 *  The @c Bound template class instantiated through the @c Bind function
 *  allows to do things more conveniently:
 *  @code
 *  Context gl;
 *  // create the texture
 *  Texture tex;
 *  // start a block where the texture will be set-up
 *  {
 *    // make a wrapper for the texture bound to TEXTURE_2D
 *    gl.Bind(Texture::Target::_2D, tex)
 *      .Image2D(...)
 *      .GenerateMipmap()
 *      .MinFilter(TextureMinFilter::Linear)
 *      .MagFilter(TextureMagFilter::Linear)
 *      .WrapS(TextureWrap::Repeat)
 *      .WrapT(TextureWrap::Repeat)
 *      .SwizzleG(TextureSwizzle::Red)
 *      .SwizzleB(TextureSwizzle::Red);
 *  }
 *  @endcode
 *
 *  The source code for the specializations of the @ref oglplus::Bound "Bound"
 *  template class is not included automatically by including oglplus/all.hpp.
 *  In order to be able to use @c Bound objects it is necessary to include
 *  the appropriate header:
 *
 *  @code
 *  #include <oglplus/bound/texture.hpp> // Bound<Texture>
 *  #include <oglplus/bound/buffer.hpp> // Bound<Buffer>
 *  #include <oglplus/bound/framebuffer.hpp> // Bound<Framebuffer>
 *  #include <oglplus/bound/renderbuffer.hpp> // Bound<Renderbuffer>
 *  @endcode
 */

/** @page oglplus_dependencies Dependencies
 *
 */

} // namespace oglplus
#endif // OGLPLUS_DOCUMENTATION_ONLY

#endif // include guard
