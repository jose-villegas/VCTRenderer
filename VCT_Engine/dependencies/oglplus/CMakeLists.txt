#  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX d)

project(OGLplus)

include(config/CommonDefs.cmake)
file(READ VERSION OGLPLUS_VERSION)

# The GL context initialization library
set(OGLPLUS_FORCE_GL_INIT_LIB "NONE" CACHE STRING "The GL context initialization library to be used.")
set_property(CACHE OGLPLUS_FORCE_GL_INIT_LIB PROPERTY STRINGS ${OGLPLUS_GL_INIT_LIBS})

# The GL API declaration library
set(OGLPLUS_FORCE_GL_API_LIB "NONE" CACHE STRING "The GL API definition library to be used.")
set_property(CACHE OGLPLUS_FORCE_GL_API_LIB PROPERTY STRINGS ${OGLPLUS_GL_API_LIBS})

option(OGLPLUS_NO_EXAMPLES "Don't build examples and textures." Off)
option(OGLPLUS_NO_SCREENSHOTS "Don't make example screenshots for the docs." Off)
option(OGLPLUS_NO_DOCS "Don't build and install the documentation." Off)

option(OGLPLUS_NO_BOOST_CONFIG "Don't use the Boost.Config library for the detection of support for C++ features." Off)

option(OGLPLUS_WITH_TESTS "Configure the testsuite." Off)

# The low-profile setting
option(OGLPLUS_CONFIG_SET_LOW_PROFILE "Set the OGLPLUS_LOW_PROFILE switch in site_config.hpp." Off)

if(OGLPLUS_CONFIG_SET_LOW_PROFILE)
	set(OGLPLUS_CONFIG_SET_LOW_PROFILE 1)
	if(${OGLPLUS_LOW_PROFILE})
		set(OGLPLUS_LOW_PROFILE 1)
	else()
		set(OGLPLUS_LOW_PROFILE 0)
	endif()
else()
	set(OGLPLUS_CONFIG_SET_LOW_PROFILE 0)
	set(OGLPLUS_LOW_PROFILE 0)
endif()

# the GL header and GL init lib
set(OGLPLUS_USE_GLCOREARB_H 0)
set(OGLPLUS_USE_GL3_H 0)
set(OGLPLUS_USE_GLEW 0)
set(OGLPLUS_USE_GL3W 0)

set(HEADER_SEARCH_PATHS ${HEADER_SEARCH_PATHS};${PROJECT_SOURCE_DIR}/third_party/include)

include(FindPkgConfig)
include(config/CommonFindMod.cmake)

include(config/FindThreads.cmake)

include(config/FindOpenGL.cmake)
include(config/FindGLEW.cmake)
include(config/FindGL3W.cmake)
include(config/FindGLES3.cmake)
include(config/FindGLFW.cmake)
include(config/FindGLFW3.cmake)
include(config/FindGLUT.cmake)
include(config/FindWXGL.cmake)
include(config/FindQT4GL.cmake)
include(config/FindQT5GL.cmake)
include(config/FindSDL.cmake)
include(config/FindGLX.cmake)

include(config/FindOpenAL.cmake)

include(config/FindEGL.cmake)

include(config/FindGLM.cmake)
include(config/FindPNG.cmake)
include(config/FindPangoCairo.cmake)

# compiler options
include(config/Compiler.cmake)
# c++ compiler options / supported feature detection
include(config/CPPFeature.cmake)
# check if Boost.Config is available
include(config/FindBoost.cmake)

# macro for checking executable requirements
include(config/ExeRequirements.cmake)

# We'll need OpenGL API and context initialization/window-system library
foreach(KIND API INIT)
	if((NOT OGLPLUS_FORCE_GL_${KIND}_LIB) OR (${OGLPLUS_FORCE_GL_${KIND}_LIB} STREQUAL "NONE"))
		foreach(GL_${KIND}_LIB ${OGLPLUS_GL_${KIND}_LIBS})
			if(${GL_${KIND}_LIB}_FOUND)
				set(GL_LIB_HAS_ALL_DEPS true)
				string(TOLOWER ${GL_${KIND}_LIB} LIB_LC)
				do_require_all_dependencies(
					"${CMAKE_CURRENT_SOURCE_DIR}/example/oglplus"
					"${LIB_LC}_main"
					GL_LIB_HAS_ALL_DEPS
				)
				unset(LIB_LC)
				if(${GL_LIB_HAS_ALL_DEPS})
					set(OGLPLUS_GL_${KIND}_LIB ${GL_${KIND}_LIB})
					set(OGLPLUS_USE_${GL_${KIND}_LIB} 1)
					break()
				endif()
			endif()
		endforeach()
	else()
		foreach(GL_${KIND}_LIB ${OGLPLUS_GL_${KIND}_LIBS})
			if(${OGLPLUS_FORCE_GL_${KIND}_LIB} STREQUAL ${GL_${KIND}_LIB})
				if(${GL_${KIND}_LIB}_FOUND)
					set(OGLPLUS_GL_${KIND}_LIB ${GL_${KIND}_LIB})
					set(OGLPLUS_USE_${GL_${KIND}_LIB} 1)
					break()
				else()
					message(
						FATAL_ERROR
						"The '${GL_${KIND}_LIB}' library requested but not found"
					)
					return()
				endif()
			endif()
		endforeach()
	endif()
endforeach()

if(NOT OGLPLUS_GL_INIT_LIB)
	if(NOT OGLPLUS_NO_EXAMPLES)
		message(FATAL_ERROR "No OpenGL context initialization library found!")
	endif()
else()
	message(STATUS "Using ${OGLPLUS_GL_INIT_LIB} for GL context initialization.")
endif()

unset(OGLPLUS_GL_INCLUDE_DIRS)

if((${OGLPLUS_USE_GLCOREARB_H}) OR (${OGLPLUS_USE_GL3_H}))

	set(OGLPLUS_GL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${OPENGL_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${OPENGL_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using native OpenGL API library")
elseif(${OGLPLUS_USE_GLEW})

	set(OGLPLUS_GL_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${GLEW_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using GLEW as GL API library")
elseif(${OGLPLUS_USE_GL3W})

  set(OGLPLUS_GL_INCLUDE_DIRS ${GL3W_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${GL3W_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${GL3W_LIBRARIES} ${OPENGL_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using GL3W as GL API library")
else()
	message(FATAL_ERROR "A library including OpenGL symbols is required for OGLplus")
	return()
endif()

if(${BoostConfig_FOUND})
	include_directories(BEFORE SYSTEM ${BoostConfig_INCLUDE_DIRS})
endif()

# now create the site-configuration header
configure_file(
	${PROJECT_SOURCE_DIR}/config/oglplus/config/site.hpp.in
	${PROJECT_BINARY_DIR}/include/oglplus/config/site.hpp
)

# include the oglplus headers
include_directories(${PROJECT_SOURCE_DIR}/include)
# include the generated headers
include_directories(${PROJECT_BINARY_DIR}/include)
# include the third party headers
include_directories(${PROJECT_SOURCE_DIR}/third_party/include)
# include the oglplus implementation
include_directories(${PROJECT_SOURCE_DIR}/implement)

# common functions for GL version and extension detection
include(config/GLVerExt.cmake)
# GL version detection
include(config/GLVer.cmake)
# GL extension detection
include(config/GLExt.cmake)

# re-create the site-configuration header
configure_file(
	${PROJECT_SOURCE_DIR}/config/oglplus/config/site.hpp.in
	${PROJECT_BINARY_DIR}/include/oglplus/config/site.hpp
)

install(
	FILES ${PROJECT_BINARY_DIR}/include/oglplus/config/site.hpp
	DESTINATION include/oglplus/config
)

include(config/OGLplusLib.cmake)

# the configuration info
add_subdirectory(config)

if(OGLPLUS_NO_EXAMPLES)
	set(OGLPLUS_NO_SCREENSHOTS On)
endif()
if(NOT OGLPLUS_NO_SCREENSHOTS)
	include(FindImageMagick)
endif()

if(NOT OGLPLUS_NO_DOCS)
	add_subdirectory("doc/doxygen")
endif()

if(NOT OGLPLUS_NO_EXAMPLES)
	add_subdirectory("source/textures" "textures")
	add_subdirectory("source/models" "models")
	add_subdirectory("example")
endif()

add_subdirectory("source/lib")

if(OGLPLUS_WITH_TESTS)
	add_subdirectory("test")
endif()

# install the "regular" headers
install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION include
)
# install the implementation inline files
install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/implement/
	DESTINATION include
)

# install the docs if they are enabled
if(NOT OGLPLUS_NO_DOCS)
	foreach(LIB oglplus oalplus eglplus)
		install(
			DIRECTORY ${PROJECT_BINARY_DIR}/doc/doxygen/${LIB}/html
			DESTINATION share/${LIB}/doc
		)
	endforeach()
endif()
