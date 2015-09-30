/*
 *  .file doc/tutorial/oglplus/000_build_lib.cpp
 *
 *  .author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#if OGLPLUS_DOCUMENTATION_ONLY

/** @page oglplus_tut_000_build_lib Improving build times with the library
 *
 *  This tutorial explains how to improve build times of large applications
 *  consisting of multiple translation units or large sets of individual
 *  applications using @OGLplus. The rules described here also apply to using
 *  the @OALplus and @EGLplus libraries.
 *
 *  Generally @OGLplus, @OALplus and @EGLplus are header-only libraries.
 *  Most functions just wrap the GL, AL and EGL calls, add some error checking
 *  enforce type safety, etc. Letting the compiler see the bodies of the functions
 *  allows it to perform certain optimizations and in case of smaller functions
 *  their bodies can be inlined if the compiler determines that it can improve
 *  the final binaries using the libraries.
 *
 *  Applications using them do not need to link to any binary library if their
 *  developers do not want it. Several parts of @OGLplus, like the mesh/shape
 *  generators and loaders, image loaders, text rendering utilities, etc. however
 *  do include larger functions parsing and compiling of which can take some time,
 *  especially if all those functions are compiled again and again for every
 *  translation unit (.cpp file) that is part of a larger application. Similarly
 *  if there is a larger set of applications all using @OGLplus then all the
 *  functions are compiled all over for every one of them.
 *  In both cases it would be advantageous to have the larger functions compiled
 *  only once and link their code to the final binary or binaries to improve
 *  build times.
 *
 *  Even more advantageous would be to have the ability to simply enable/disable
 *  the inlining. Disable it during the development to improve (re-)build times
 *  and enable it to let the compiler do optimizations in release builds.
 *  Fortunatelly @OGLplus allows to do this by setting the value of the
 *  @c OGLPLUS_LINK_LIBRARY, @c OALPLUS_LINK_LIBRARY and @c EALPLUS_LINK_LIBRARY
 *  preprocessor symbols and following several simple rules:
 *
 *  In case of a large application consisting of many translation units using
 *  @OGLplus one of the translation unit (preferrably the one that gets changed
 *  the least or even a dedicated one) needs to include the lib.hpp header:
 *  @code
 *  // OGLplus
 *  #if OGLPLUS_LINK_LIBRARY // != 0
 *  # include <oglplus/lib.hpp>
 *  #endif
 *  // OALplus
 *  #if OALPLUS_LINK_LIBRARY // != 0
 *  # include <oalplus/lib.hpp>
 *  #endif
 *  // EGLplus:
 *  #if EGLPLUS_LINK_LIBRARY // != 0
 *  # include <eglplus/lib.hpp>
 *  #endif
 *  @endcode
 *
 *  To disable the inlining for a particular build ensure that for all translation
 *  units the @c OGLPLUS_LINK_LIBRARY, @c OALPLUS_LINK_LIBRARY and
 *  @c EGLPLUS_LINK_LIBRARY symbols are defined and set to @c 1.
 *  To build the library in header-only mode and to enable the inlining set then
 *  all to @c 0 or just don't define them at all and they will default to zero.
 *  Actually if only @c OGLPLUS_LINK_LIBRARY is defined and has some value set,
 *  then the other preprocessor symbols default to the same value.
 *
 *  In case there are several standalone executable just create a dedicated cpp
 *  file including the @c lib.hpp header(s), build it and all the other cpp files
 *  with @c OGLPLUS_LINK_LIBRARY defined and link the compiled object file that
 *  included @c lib.hpp to all the executables. To switch the inlining on/off
 *  use the same method as described above.
 */

#endif // OGLPLUS_DOCUMENTATION_ONLY
