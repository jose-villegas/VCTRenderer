==============
OGLplus README
==============

:Author: Matúš Chochlík <chochlik@gmail.com>

.. contents::

.. _OpenGL: http://opengl.org/
.. _OpenAL: http://openal.org/
.. _EGL: http://www.khronos.org/egl
.. _OGLplus: http://oglplus.org/
.. _CMake: http://www.cmake.org/
.. _Doxygen: http://www.doxygen.org/
.. _Inkscape: http://inkscape.org/
.. _libPNG: http://www.libpng.org/
.. _GLEW: http://glew.sourceforge.net/
.. _GL3W: http://github.com/shakesoda/gl3w
.. _GLFW: http://www.glfw.org/
.. _FreeGLUT: http://freeglut.sourceforge.net/
.. _SDL: http://www.libsdl.org/
.. _wxGL: http://www.wxwidgets.org/
.. _Qt: http://qt.digia.com/

Introduction to OGLplus
=======================

`OGLplus`_ is collection of portable open-source libraries which implement thin
object-oriented facades over the `OpenGL`_  (version 3 and higher),
`OpenAL`_ (version 1.1) and `EGL`_ (version 1.4) C-language APIs.
It provides wrappers which automate resource and object management and make
the use of OpenGL, OpenAL and EGL in C++ safer and easier.



Building and Installation
=========================

For the impatient:
------------------

::

 # linux and similar *nix systems
 $> ./configure.sh
 # or
 $> ./configure.sh --prefix=/path/to/install

 $> cd _build
 $> make
 $> make install

 # windows
 $> .\configure.bat
 # open the generated MSVC solution in the _build directory
 # or use some variant of make if available

 # On platforms with python
 $> python configure.py --build

For the busy:
-------------

::

 # linux and similar *nix systems
 $> ./configure.sh [--prefix=/path/to/install] [--no-docs] --no-examples --build --install

Overview
--------

OGLplus uses a CMake-based build/configuration system. The library itself
is header-only, which means that applications using it do not need to link
to a compiled library, but need just to include the header files [#oglplus_link_library]_.

The build system handles several important tasks:

 * Detects if the necessary things are installed and makes a site-configuration
   header file

 * Detects the support for several C++11 features and builds a config header

 * Builds several additional, automatically generated headers

 * Installs all header files to a directory specified by the install prefix

 * Builds the example executables and assets (textures, models, etc.)
   used by the examples (optional)

 * Builds and installs the documentation (optional)


Requirements
------------

 - Compiler supporting required C++11 features [#req_cxx11_feats]_.
   Currently supported compilers:

   * ``g++`` (at least version 4.5, 4.6 and higher is recommended)

   * ``clang++`` (at least version 3.0) - possibly with some limitations
     due to the lack of support for some C++11 features

   * ``MSVC 2012`` - with some limitations due to the lack of support for
     some C++11 features


 - `CMake`_ (required)

 - `Doxygen`_ (optional) is required to build the documentation. This can be disabled
   with the ``--no-docs`` command line option of the ``configure`` script (see below).

 - `Inkscape`_ (optional) is used to convert textures for the examples from SVG to PNG.
   This is required only if the textures are not pre-built (typically
   when checked out from the repository, packaged releases are shipped with
   pre-built textures). Building of the textures is optional, they are not
   necessary when the building of examples is disabled.

 - A library defining the OpenGL API (required) -- the ``GL/glcorearb.h`` or ``GL3/gl3.h``
   headers or `GLEW`_, `GL3W`_, etc. and the corresponding binary library (``libGL.so``,
   ``OpenGL32.lib``, ``libGLEW.so``, etc.).  OGLplus does not define the OpenGL symbols
   (types, constants, functions, etc.), therefore applications using
   it need to define them themselves (before including OGLplus). The examples
   currently need GLEW (at least version 1.9) or the ``GL/glcorearb.h`` header
   (available for download from www.opengl.org/registry/api/glcorearb.h) and
   a GL binary library exporting the OpenGL (3 or higher) functions.
   The build system detects the presence of GLEW or ``GL/glcorearb.h`` and configures
   compilation and linking of the examples accordingly.
   Note, however, that if several options (like both GLEW and ``GL/glcorearb.h`` plus
   the binary GL lib) are available it may be necessary to specify which option to use.
   On Linux and similar systems the precedence is following: ``GL/glcorearb.h + libGL.so``,
   ``GL3/gl3.h + libGL.so``, `GLEW`_ and `GL3W`_ (the first one found is used, unless
   specified otherwise).  On Windows systems the precedence is:  `GLEW`_, `GL3W`_,
   ``GL/glcorearb.h + OpenGL.lib`` and ``GL3/gl3.h + OpenGL.lib``.
   Also note, that on systems with multiple versions of ``libGL.so`` (for example one
   provided by Mesa3D and another provided by your GPU vendor) it may be necessary
   to specify with the ``--library-dir`` option to the ``configure`` script (described
   below) in which directories to search for the library.
   The library to be used can be explicitly specified with the ``--use-gl-header-lib``
   option or with one of the ``--use-*`` options of the ``configure`` script.

 - A library initializing the default rendering context (required) -- Currently
   the examples can be built if at least one of the following libraries is
   installed on the system: X11+GLX, `FreeGLUT`_, `GLFW`_, `SDL`_, `wxGL`_ or `Qt`_.
   The build system detects the presence of these libraries and configures
   compilation and linking of the examples accordingly.
   The library to be used can be explicitly specified with the ``--use-gl-header-lib``
   option or with one of the ``--use-*`` options of the ``configure`` script (see below).


 - `libPNG`_ (optional) -- Some examples and some classes provided by OGLplus use libPNG to load
   PNG files. These are however not required for the general use of OGLplus,
   applications may use other means to load binary image files. The build system
   tries to detect the availability of libPNG and if not found the examples
   using it are not built.


On Linux distributions with the ``apt`` package manager, the following should
be enough to install most of the dependencies for the FreeGLUT+GLEW configuration:

::

 sudo apt-get install doxygen cmake g++ libglew-dev freeglut3-dev libpng12-dev

For the configuration using GLFW+GLEW you would need the following:

::

 sudo apt-get install doxygen cmake g++ libglew-dev libglfw-dev libpng12-dev

These two configs mentioned above are usually the ones that work on most systems.
Of course other combinations of the 'GL-API' and 'GL-Context' libraries
are supported and may be used as explained above.
For other configurations using SDL, Qt4, wxWidgets, etc. you need to install
the appropriate packages (the names vary wildly between distrubutions or even
between versions of the same distribution so they are not listed here).


CMake-based build configuration
-------------------------------

The CMake script defines and uses several variables to modify the build
configuration, which can be specified on the command-line when invoking
cmake (with the -D option. see cmake manual for details):

 * ``HEADER_SEARCH_PATHS`` *<empty>*: (semicolon-separated) list of paths
   to additional directories to search when looking for 3rd-party headers
   like GL/glew.h, GL3/gl3.h, GL/glcorearb.h, etc.

 * ``LIBRARY_SEARCH_PATHS`` *<empty>*: (semicolon-separated) list of paths
   to additional directories to search when looking for 3rd-party binary
   libraries like GL, GLEW, GL3W, GLFW, SDL, glut, png, etc.

 * ``OGLPLUS_NO_EXAMPLES`` *Off*: Do not build the examples nor the assets.

 * ``OGLPLUS_NO_DOCS`` *Off*: Do not build and install the documentation.


User-friendly configuration script
----------------------------------

The configuration script comes in three flawors:

1. ``configure.sh`` -- For platforms with bash.
2. ``configure.bat`` -- For windows.
3. ``configure.py`` -- For platforms with python (recommended).
                       This version of the script is the most portable
                       and supports most features.

The ``configure`` script is a more user-friendly way to invoke cmake and to specify
additional parameters for the configuration process.

Some of the more important command-line options are described below:

--help  Display the help screen.

--prefix PATH       Specifies the installation prefix path for cmake (sets
                    the value of the CMAKE_INSTALL_PREFIX variable).
                    If this option is not specified, cmake's default prefix is used.

--include-dir PATH    Specify additional directiories
                      to search when looking for header files. It may be used multiple
                      times to specify multiple directories. Headers are searched
                      in the directories specified with this option in the same order
                      in which they appear on the command-line and the default system
                      header locations are searched only afterwards. The first header
                      found is used, in case there are multiple versions of the searched
                      header file.


--library-dir PATH    Specify additional directiories
                      to search when looking for compiled libraries. It may be used
                      multiple times to specify multiple directories. Libraries are
                      searched in the directories specified with this option
                      in the same order in which they appear on the command-line
                      and the default system library locations are searched afterwards.
                      The first library found is used, in case there are multiple
                      versions of the searched library.

--no-docs  Do not build the documentation.

See the ``--help`` option for the full description and detailed info on the usage
of this script. Bash users can also do ``source config/configure.bash_complete``
to install a bash completion extension that allows to TAB-complete the command
line arguments for the ``configure-oglplus`` configuration script that invokes
``configure.py``.


Other build environments
========================

Microsoft Visual Studio 2012
----------------------------

OGLplus contains a solution file and a set of project files for building
some of the examples in MSVC 2012.
The solution is located at the following path:

- ``$(OGLplusRoot)/etc/msvc11/OGLplus/OGLplus.sln``

To build properly, it requires GLEW (1.9 or higher) and FreeGLUT libraries
to be installed and added to the list of header/library search paths
in the solution. This can be achieved by setting the following variables
visible to MSVC:

* ``OGLPLUS_DEP_INCLUDE_DIRS`` - list of (semicolon-separated) paths pointing
  to the directories where GLUT's and GLEW's header files are located.

* ``OGLPLUS_DEP_LIBRARY_DIRS`` - list of (semicolon-separated) paths pointing
  to the directories where GLUT's and GLEW's .lib files are located.

* ``OGLPLUS_DEP_LIBS`` - list of (semicolon-separated) .lib files to be linked
  to the examples. Typically ``glew32.lib;freeglut.lib`` for dynamic linking
  or ``glew32s.lib;freeglut_static.lib`` for static linking.

* ``OGLPLUS_PP_DEFS`` - list of (semicolon-separated) preprocessor symbols
  that should be defined. Typically ``GLEW_STATIC;FREEGLUT_STATIC`` are
  required for static linking of GLEW and FreeGLUT.

These variables can be set either as system or user environment variables
or the ``OGLplus-site.props`` property sheet file (located in the solution's
directory) can be used. Either edit the file in a text editor or use MSVC's
*Property Manager* and set the variables listed above as appropriate
on your system. Note, that the ``OGLplus-site.props`` file already
defines variables like the above, but with appended underscore
(for example ``OGLPLUS_DEP_INCLUDE_DIRS_``, etc.). To set the variables
you can open the file in a text editor, remove the underscores and set
the values to match your system configuration.

Also note that when building the examples via the MSVC solutions
the ``$(OGLplusRoot)/_prebuilt/`` directory is required. This directory
contains automatically generated textures and other files that are
not in the source repository. This directory is included in the release
packages. If you are building from sources from the Git repository
also download a recent package and copy the ``_prebuilt/`` directory
into to the directory where you have cloned the repo or use the cmake
build system and build them that way.


Assets used in examples
=======================

Many of the examples use binary media files which are not suitable
for inclusion to the source repository mainly because of their size.
OGLplus examples use texture bitmaps, fonts and models, OALplus examples use sounds,
etc. Some of these media files can be generated from textual representations
(for example SVG files converted or POVray sources rendered to bitmaps).
Assets for which there is a textual source and its size does not exceed
certain limits are included in the repository and are rendered into their
binary form by the build system. This may however require some third-party
applications and the rendering process can be very time-consuming.

Because of this, some of the medium-sized assets are pre-built and shipped
with the release packages (in the ``_prebuilt/`` directory) and the larger
assets can be downloaded individually from
http://sourceforge.net/projects/oglplus/files/assets/


Acknowledgements
================

- *Tolga Dalman* for helping to get started with CMake.
- *Timo Keller* for contributing one of the examples.
- *Denis Ovod* for constructive critique and suggestions.
- *Per Nordlöw* for useful suggestions.
- *Dmitry Yakimenko (detunized)* for several patches and porting to MacOS X.
- *Csala Tamás* for useful feedback and help with the fixing of several bugs.
- *David McFarland* for several patches.
- *Jesús Zazueta* for useful feedback and help with the fixing of several bugs.
- *Konstantin Gainullin* for porting to Marmalade and fixing several warnings.
- *Peter Laufenberg* for several patches.
- *Jan P Springer (regnirpsj)* for contributing SSBO wrappers and example.
- *Mikołaj Siedlarek* for libpng-related fixes.
- *Jason Francis* for several bugfixes.


License
=======

Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
Software License, Version 1.0. (See accompanying file
LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


.. [#oglplus_link_library] Although OGLplus is generally a header-only library,
   several parts of it (mostly some complex functions or functions with static data)
   can optionally be built separately and linked to applications, which can lead
   to improved build times especially for larger projects.
   See the documentation for the ``OGLPLUS_LINK_LIBRARY`` preprocessor configuration
   option for more details. Generally if ``OGLPLUS_LINK_LIBRARY`` is set to zero
   then everything is inlined, otherwise some functions are only declared, but not
   defined and the ``oglplus/lib.hpp`` header that contains the definition
   of all such functions must be included in one of the translation
   units that are linked into the final application.

.. [#req_cxx11_feats] OGLplus requires the following C++11 features:
   The ``type_traits`` and ``tuple`` librares, variadic preprocessor macros, r-value
   references and move-constructors.
   OGLplus also uses (but has workarounds for or disables certain components
   and/or functions if the features are not available):
   Strongly typed enumerations, variadic templates, initializer lists, lambdas,
   defaulted and deleted functions, function template default arguments,
   constexpr, noexcept, nullptr, explicit conversion operators, unicode literals
   and user-defined literals.
