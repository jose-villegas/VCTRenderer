#  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
set(OGLPLUS_GL_INIT_LIBS GLX GLUT GLFW3 GLFW SDL WXGL QT4GL QT5GL EGL)
if(${WIN32})
	set(OGLPLUS_GL_API_LIBS GLEW GL3W GLCOREARB_H GL3_H)
else()
	set(OGLPLUS_GL_API_LIBS GLCOREARB_H GL3_H GLEW GL3W)
endif()
