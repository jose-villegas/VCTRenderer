#  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

macro(gl_lib_ext_detection GL_LIB EXTENSION_VENDOR EXTENSION_NAME)

	set(OGLPLUS_CONFIG_QUERY_GL_EXT ${GL_LIB}_${EXTENSION_VENDOR}_${EXTENSION_NAME})
	set(OGLPLUS_CONFIG_QUERY_GL_EXT_VENDOR ${EXTENSION_VENDOR})
	set(OGLPLUS_CONFIG_QUERY_GL_EXT_NAME ${EXTENSION_NAME})

	gl_feature_detection(${GL_LIB} ${EXTENSION_VENDOR}_${EXTENSION_NAME} EXT)

	if(HAS_${GL_LIB}_${EXTENSION_VENDOR}_${EXTENSION_NAME})
		message(STATUS "Found ${GL_LIB} extension: ${EXTENSION_VENDOR}_${EXTENSION_NAME}")
	else()
		file(APPEND ${OGLPLUS_FIX_GL_EXTENSION_HPP} "#ifdef GL_${EXTENSION_VENDOR}_${EXTENSION_NAME}\n")
		file(APPEND ${OGLPLUS_FIX_GL_EXTENSION_HPP} "#undef GL_${EXTENSION_VENDOR}_${EXTENSION_NAME}\n")
		file(APPEND ${OGLPLUS_FIX_GL_EXTENSION_HPP} "#endif //GL_${EXTENSION_VENDOR}_${EXTENSION_NAME}\n")
		if(OGLPLUS_DEBUG_GL_EXT_ERROR)
			if("${OGLPLUS_DEBUG_GL_EXT_ERROR}" STREQUAL "${EXTENSION_VENDOR}_${EXTENSION_NAME}")
				message(FATAL_ERROR "Error detecting GL extension ${EXTENSION_VENDOR}_${EXTENSION_NAME}")
			endif()
		endif()
	endif()

	unset(OGLPLUS_CONFIG_QUERY_GL_EXT)
	unset(OGLPLUS_CONFIG_QUERY_GL_EXT_VENDOR)
	unset(OGLPLUS_CONFIG_QUERY_GL_EXT_NAME)

endmacro()

set(
	OGLPLUS_FIX_GL_EXTENSION_HPP
	${PROJECT_BINARY_DIR}/include/oglplus/config/fix_gl_extension.hpp
)

configure_file(
	${PROJECT_SOURCE_DIR}/config/oglplus/config/fix_gl_extension.hpp.in
	${OGLPLUS_FIX_GL_EXTENSION_HPP}
)

gl_lib_ext_detection(GL ARB shader_subroutine)
gl_lib_ext_detection(GL ARB shader_atomic_counters)
gl_lib_ext_detection(GL ARB tessellation_shader)
gl_lib_ext_detection(GL ARB separate_shader_objects)
gl_lib_ext_detection(GL ARB shading_language_include)
gl_lib_ext_detection(GL ARB transform_feedback2)
gl_lib_ext_detection(GL ARB viewport_array)
gl_lib_ext_detection(GL ARB compatibility)
gl_lib_ext_detection(GL ARB gpu_shader_fp64)
gl_lib_ext_detection(GL ARB direct_state_access)
gl_lib_ext_detection(GL EXT direct_state_access)
gl_lib_ext_detection(GL KHR debug)
gl_lib_ext_detection(GL NV path_rendering)
gl_lib_ext_detection(GL NV copy_image)

file(APPEND ${OGLPLUS_FIX_GL_EXTENSION_HPP} "#endif //include guard\n")
install(FILES ${OGLPLUS_FIX_GL_EXTENSION_HPP} DESTINATION include/oglplus)

macro(glew_ext_detection EXTENSION_VENDOR EXTENSION_NAME)
	if(GLEW_FOUND)
		gl_lib_ext_detection(GLEW ${EXTENSION_VENDOR} ${EXTENSION_NAME})
	endif()
endmacro()

glew_ext_detection(ARB compatibility)
glew_ext_detection(EXT direct_state_access)
glew_ext_detection(ARB debug_output)
glew_ext_detection(ATI meminfo)
glew_ext_detection(NV path_rendering)
glew_ext_detection(AMD performance_monitor)
