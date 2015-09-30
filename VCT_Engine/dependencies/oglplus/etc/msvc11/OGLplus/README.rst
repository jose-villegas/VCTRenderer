Microsoft Visual Studio 2012 Solution
--------------------------------------

OGLplus contains solution files and a set of project files for building
some of the examples in MSVC 2012. Note however, that the preferred way
to build OGLplus is to use the cmake-based build system. Please see
the ``README.rst`` file in the project root directory for details.

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

These variables can be set either as system or user *environment variables*
or the ``OGLplus-site.props`` property sheet file (located in the solution's
directory) can be used. Either edit the file in a text editor or use MSVC's
*Property Manager* and set the variables listed above as appropriate
on your system.

:Note: the ``OGLplus-site.props`` file already
  defines variables like the above, but with appended underscore;
  (for example ``OGLPLUS_DEP_INCLUDE_DIRS_``, etc.). To set the variables,
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
