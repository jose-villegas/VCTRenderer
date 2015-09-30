#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
unset(OPENGL_INCLUDE_DIRS)
set(OPENGL_FOUND 0)
#
# try to find GL3/gl3.h
find_path(
	OPENGL_GL3_H_DIR GL3/gl3.h
	PATHS ${HEADER_SEARCH_PATHS}
	NO_DEFAULT_PATH
)
# if that didn't work try the system directories
if((NOT OPENGL_GL3_H_DIR) OR (NOT EXISTS ${OPENGL_GL3_H_DIR}))
	find_path(
		OPENGL_GL3_H_DIR
		NAMES GL3/gl3.h OpenGL/gl3.h
	)
endif()
# if found append it to the include directories
if((OPENGL_GL3_H_DIR) AND (EXISTS ${OPENGL_GL3_H_DIR}))
	set(OPENGL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIRS} ${OPENGL_GL3_H_DIR})
	set(OPENGL_FOUND 1)
	set(GL3_H_FOUND 1)
endif()
#
#
# try to find GL/glcorearb.h
find_path(
	OPENGL_GLCOREARB_H_DIR GL/glcorearb.h
	PATHS ${HEADER_SEARCH_PATHS}
	NO_DEFAULT_PATH
)
# if that didn't work try the system directories
if((NOT OPENGL_GLCOREARB_H_DIR) OR (NOT EXISTS ${OPENGL_GLCOREARB_H_DIR}))
	find_path(OPENGL_GLCOREARB_H_DIR GL/glcorearb.h)
endif()
#
if((OPENGL_GLCOREARB_H_DIR) AND (EXISTS ${OPENGL_GLCOREARB_H_DIR}))
	set(OPENGL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIRS} ${OPENGL_GLCOREARB_H_DIR})
	set(OPENGL_FOUND 1)
	set(GLCOREARB_H_FOUND 1)
endif()

# try to find the GL library
find_library(
	OPENGL_LIBRARIES NAMES GL OpenGL OpenGL32
	PATHS ${LIBRARY_SEARCH_PATHS}
	NO_DEFAULT_PATH
)
if(NOT OPENGL_LIBRARIES)
	find_library(OPENGL_LIBRARIES NAMES GL OpenGL OpenGL32)
else()
	get_filename_component(OPENGL_LIBRARY_DIRS ${OPENGL_LIBRARIES} PATH)
endif()

#if we have not found the library
if(NOT OPENGL_LIBRARIES)
	set(OPENGL_LIBRARIES "")
endif()
