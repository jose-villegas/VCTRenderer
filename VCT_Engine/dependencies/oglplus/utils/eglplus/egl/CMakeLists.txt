#  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(EGLplus_EGL)

find_package(X11 REQUIRED)
find_library(GL_LIBRARIES NAMES GL)
find_package(Threads REQUIRED)

#add_definitions(-std=c++0x)
add_definitions(-std=c++0x -g3)

include_directories(${X11_INCLUDE_DIRS})
include_directories(${Threads_INCLUDE_DIRS})
include_directories(include)

add_library(EGL SHARED
	src/EGL/api.cpp
	src/EGL/glx/error.cpp
	src/EGL/glx/display.cpp
	src/EGL/glx/configs.cpp
	src/EGL/glx/surface.cpp
	src/EGL/glx/context.cpp
	src/EGL/glx/threads.cpp
	src/EGL/glx/extensions.cpp
	src/EGL/glx/ext/NV_native_query.cpp
)
target_link_libraries(EGL ${X11_LIBRARIES})
target_link_libraries(EGL ${GL_LIBRARIES})
target_link_libraries(EGL ${CMAKE_THREAD_LIBS_INIT})

set(EGL_EXT_LIBRARIES "")
foreach(EXT_LIB ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
	set(EGL_EXT_LIBRARIES "${EGL_EXT_LIBRARIES} ${EXT_LIB}")
endforeach()

configure_file(
	${CMAKE_SOURCE_DIR}/pkgconfig/egl.pc.in
	${CMAKE_BINARY_DIR}/pkgconfig/egl.pc
	@ONLY
)

install(TARGETS EGL LIBRARY DESTINATION lib)
install(DIRECTORY include/EGL DESTINATION include)
install(DIRECTORY ${CMAKE_BINARY_DIR}/pkgconfig DESTINATION lib)
