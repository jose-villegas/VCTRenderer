#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if(PNG_FOUND)
	do_use_single_dependency(PNG)
endif()

if(PANGO_CAIRO_FOUND)
	do_use_single_dependency(PANGO_CAIRO)
endif()

set(OGLPLUS_LIB_SOURCES
	native.cpp
	string.cpp
	enums.cpp
	error.cpp
	object.cpp
	program.cpp
	prog_var.cpp
	extension.cpp
	images_base.cpp
	images_load.cpp
	shapes_base.cpp
	shapes_utils.cpp
	shapes_blender.cpp
	shapes_obj.cpp
	text.cpp
	opt.cpp
	debug_output.cpp
)

if(HAS_GL_KHR_debug)
	set(OGLPLUS_LIB_SOURCES ${OGLPLUS_LIB_SOURCES} debug.cpp)
endif()

add_library(oglplus EXCLUDE_FROM_ALL ${OGLPLUS_LIB_SOURCES})
set_property(
	TARGET oglplus
	PROPERTY FOLDER "Library/OGLplus"
)

if(PNG_FOUND)
	target_link_libraries(oglplus ${PNG_LIBRARIES})
endif()

if(PANGO_CAIRO_FOUND)
	target_link_libraries(oglplus ${PANGO_CAIRO_LIBRARIES})
endif()


