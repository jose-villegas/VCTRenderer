#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if(HAS_GL_VERSION_3_2)
if((HAS_GL_VERSION_4_1) OR (HAS_GL_EXT_direct_state_access) OR (HAS_GL_ARB_separate_shader_objects))
if(
	((GLX_FOUND) AND (HAS_CPP_THREADS) AND (HAS_CPP_CHRONO)) OR
	(GLFW_FOUND)
)
	add_custom_target(oglplus-advanced-example-cloud_trace)
	set_property(
		TARGET oglplus-advanced-example-cloud_trace
		PROPERTY FOLDER "Example/Advanced/CloudTrace"
	)
	add_resource_directory(glsl oglplus-advanced-example-cloud_trace)

	include_directories(${PROJECT_SOURCE_DIR}/utils)

	if(("${OGLPLUS_GL_INIT_LIB}" STREQUAL "GLX") AND (HAS_CPP_THREADS) AND (HAS_CPP_CHRONO))
		set(CLOUD_TRACE_GL_INIT GLX)
	elseif("${OGLPLUS_GL_INIT_LIB}" STREQUAL "GLFW")
		set(CLOUD_TRACE_GL_INIT GLFW)
	else()
		if((GLX_FOUND) AND (HAS_CPP_THREADS) AND (HAS_CPP_CHRONO))
			set(CLOUD_TRACE_GL_INIT GLX)
		elseif(GLFW_FOUND)
			set(CLOUD_TRACE_GL_INIT GLFW)
		endif()
	endif()

	if("${CLOUD_TRACE_GL_INIT}" STREQUAL "GLX")
		if(HAS_GL_NV_copy_image)
			add_definitions("-DCLOUD_TRACE_USE_NV_copy_image=1")
		endif()
	endif()

	do_use_single_dependency(${CLOUD_TRACE_GL_INIT})
	if(HAS_CPP_THREADS)
		do_use_single_dependency(THREADS)
		set(CLOUD_TRACE_ADDITIONAL_SOURCES threads.cpp)
	endif()

	add_executable(
		cloud_trace	
		EXCLUDE_FROM_ALL
			${CLOUD_TRACE_ADDITIONAL_SOURCES}
			main_${CLOUD_TRACE_GL_INIT}.cpp
			main_common.cpp
			app_data.cpp
			arg_parser.cpp
			cloud_data.cpp
			ray_matrix.cpp
			resources.cpp
			textures.cpp
			programs.cpp
			raytracer.cpp
			renderer.cpp
			copier.cpp
			saver.cpp
	)
	target_link_libraries(cloud_trace ${${CLOUD_TRACE_GL_INIT}_LIBRARIES})
	target_link_libraries(cloud_trace ${OGLPLUS_GL_LIBRARIES})
	target_link_libraries(cloud_trace ${THREADS_LIBRARIES})

	if(${WIN32})
		set_property(TARGET cloud_trace PROPERTY WIN32_EXECUTABLE true)
	endif()
	set_property(
		TARGET cloud_trace
		PROPERTY FOLDER "Example/Advanced/CloudTrace"
	)

	add_dependencies(oglplus-advanced-example-cloud_trace cloud_trace)
	add_dependencies(oglplus-advanced-examples oglplus-advanced-example-cloud_trace)
endif()
endif()
add_subdirectory(tools)
endif()
