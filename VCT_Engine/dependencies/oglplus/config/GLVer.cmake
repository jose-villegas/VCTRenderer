#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

macro(gl_version_detection GL_LIB VERSION)
	set(OGLPLUS_CONFIG_QUERY_GL_VER ${GL_LIB}_VERSION_${VERSION})
	string(REPLACE "_" ";" GL_VER_LIST ${VERSION})
	list(GET GL_VER_LIST 0 OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR)
	list(GET GL_VER_LIST 1 OGLPLUS_CONFIG_QUERY_GL_VER_MINOR)

	if(NOT DEFINED OGLPLUS_CONFIG_STRICT_VERSION_CHECK)
		set(OGLPLUS_CONFIG_STRICT_VERSION_CHECK 1)
	endif()

	gl_feature_detection(${GL_LIB} ${VERSION} VER)

	unset(OGLPLUS_CONFIG_QUERY_GL_VER)
	unset(OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR)
	unset(OGLPLUS_CONFIG_QUERY_GL_VER_MINOR)
endmacro()

if(NOT OGLPLUS_MAX_GL_VERSION)
	set(OGLPLUS_MAX_GL_VERSION 9_9)
endif()

set(
	OGLPLUS_FIX_GL_VERSION_HPP
	${PROJECT_BINARY_DIR}/include/oglplus/config/fix_gl_version.hpp
)

configure_file(
	${PROJECT_SOURCE_DIR}/config/oglplus/config/fix_gl_version.hpp.in
	${OGLPLUS_FIX_GL_VERSION_HPP}
)
configure_file(
	${PROJECT_SOURCE_DIR}/config/gl/assert_GL_VER_symbols.ipp.in
	${PROJECT_BINARY_DIR}/gl/assert_GL_VER_symbols.ipp
)

message(STATUS "Detecting OpenGL version")
foreach(GL_VERSION 4_5 4_4 4_3 4_2 4_1 4_0 3_3 3_2 3_1)
	if(NOT(OGLPLUS_GL_VERSION_MAJOR))
		if(NOT("${GL_VERSION}" STRGREATER "${OGLPLUS_MAX_GL_VERSION}"))
			gl_version_detection(GL ${GL_VERSION})
			if(${HAS_GL_${GL_VERSION}})
				set(HAS_GL_VERSION_${GL_VERSION} TRUE)
				string(REPLACE "_" ";" GL_VER_LIST ${GL_VERSION})
				list(GET GL_VER_LIST 0 OGLPLUS_GL_VERSION_MAJOR)
				list(GET GL_VER_LIST 1 OGLPLUS_GL_VERSION_MINOR)
			endif()
		endif()
	endif()
	if(OGLPLUS_GL_VERSION_MAJOR)
		set(HAS_GL_VERSION_${GL_VERSION} TRUE)
	else()
		set(OGLPLUS_NO_GL_VERSION_${GL_VERSION} TRUE)
		file(APPEND ${OGLPLUS_FIX_GL_VERSION_HPP} "#ifdef GL_VERSION_${GL_VERSION}\n")
		file(APPEND ${OGLPLUS_FIX_GL_VERSION_HPP} "#undef GL_VERSION_${GL_VERSION}\n")
		file(APPEND ${OGLPLUS_FIX_GL_VERSION_HPP} "#endif //GL_VERSION_${GL_VERSION}\n")
	endif()
endforeach()

file(APPEND ${OGLPLUS_FIX_GL_VERSION_HPP} "#endif //include guard\n")
install(FILES ${OGLPLUS_FIX_GL_VERSION_HPP} DESTINATION include/oglplus)

if(OGLPLUS_GL_VERSION_MAJOR)
	message(STATUS "Found GL version ${OGLPLUS_GL_VERSION_MAJOR}.${OGLPLUS_GL_VERSION_MINOR}")
else()
	if(OGLPLUS_DEBUG_GL_VER_ERROR)
		message(FATAL_ERROR "Error detecting GL version")
	else()
		message("Could not detect GL version, assuming 3.0")
	endif()
	set(HAS_GL_VERSION_3_0 TRUE)
	set(OGLPLUS_GL_VERSION_MAJOR 3)
	set(OGLPLUS_GL_VERSION_MINOR 0)
endif()
