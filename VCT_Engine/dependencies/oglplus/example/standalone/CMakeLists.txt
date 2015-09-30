#  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)

#TODO
#if(NOT(${CMAKE_COMPILER_IS_GNUCXX}))
#	return()
#endif()

add_custom_target(standalone-examples ALL)
set_property(TARGET standalone-examples PROPERTY FOLDER "Example/Standalone")

function(standalone_example_common EXAMPLE_NAME)
	foreach(DEP OPENGL ${ARGN})
		do_use_single_dependency(${DEP})
	endforeach()
	add_executable(
		${EXAMPLE_NAME}
		EXCLUDE_FROM_ALL
		${EXAMPLE_NAME}.cpp
	)
	add_dependencies(standalone-examples ${EXAMPLE_NAME})
	foreach(DEP OPENGL ${ARGN})
		target_link_libraries(${EXAMPLE_NAME} ${${DEP}_LIBRARIES})
	endforeach()
	set_property(TARGET ${EXAMPLE_NAME} PROPERTY FOLDER "Example/Standalone")
endfunction()

if(EGL_FOUND AND OPENGL_FOUND)
	standalone_example_common(001_triangle_screenshot EGL OGLPLUS_GL)
endif()

standalone_example_common(001_text2d)

if(GLUT_FOUND AND GLES3_FOUND)
	standalone_example_common(001_triangle_glut_gles3 GLUT GLES3)
endif()

if(GLUT_FOUND AND GLEW_FOUND)
	standalone_example_common(001_gl_info GLUT GLEW) 
	standalone_example_common(001_gl_limits GLUT GLEW)

	standalone_example_common(001_triangle_glut_glew GLUT GLEW)
	standalone_example_common(001_hello_glut_glew GLUT GLEW)

	standalone_example_common(001_shape2dot GLUT GLEW) 
	if(HAS_GL_VERSION_4_3)
		standalone_example_common(001_program_resources GLUT GLEW)
	endif()

	if(HAS_GLEW_ATI_meminfo)
		standalone_example_common(001_ati_meminfo GLUT GLEW)
	endif()

	if(HAS_GLEW_AMD_performance_monitor)
		if(NOT OGLPLUS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
			standalone_example_common(002_amd_perf_monitor GLUT GLEW)
		endif()
	endif()

	if(HAS_GLEW_ARB_compatibility AND HAS_GLEW_ARB_debug_output)
		standalone_example_common(002_triangle_compatibility GLUT GLEW)
	endif()

	if(HAS_GLEW_ARB_compatibility AND HAS_GLEW_NV_path_rendering)

		if(NOT OGLPLUS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
			standalone_example_common(002_simple_path GLUT GLEW)
		endif()

		if(HAS_GLEW_EXT_direct_state_access)
			standalone_example_common(003_simple_text GLUT GLEW)

			standalone_example_common(004_text_rendering GLUT GLEW)
		endif()
	endif()

	if(HAS_GLEW_EXT_direct_state_access)
		standalone_example_common(003_dsa_texture GLUT GLEW)
	endif()

	if(PNG_FOUND)
		standalone_example_common(004_bitmap_text_hello GLUT GLEW PNG)
		standalone_example_common(004_bitmap_text_rendering GLUT GLEW PNG)
		standalone_example_common(004_any_text_rendering GLUT GLEW PNG)
		standalone_example_common(004_stbttf_text_rendering GLUT GLEW PNG)
	endif()

	if(PANGO_CAIRO_FOUND)
		standalone_example_common(004_pango_cairo_hello GLUT GLEW PANGO_CAIRO)
	endif()

	if(GLM_FOUND)
		standalone_example_common(007_glm_boxes GLUT GLEW GLM)
	endif()

	standalone_example_common(010_blender2html)

	if(OPENAL_FOUND)
		standalone_example_common(020_oglplus_oalplus GLUT GLEW OPENAL)
	endif()

	if(PNG_FOUND)
		standalone_example_common(025_bitmap_font_text GLUT GLEW PNG)
	endif()

	standalone_example_common(026_blender_mesh_loader GLUT GLEW)
endif()

if(GLFW3_FOUND)
	standalone_example_common(002_triangles_glfw3_glew GLFW3 GLEW)
endif()

if(GLFW3_FOUND)
	standalone_example_common(029_mandelbrot_glfw3 GLFW3 OGLPLUS_GL)
endif()

