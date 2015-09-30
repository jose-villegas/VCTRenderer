#  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

function(gl_feature_detection GL_LIB QUERY QUERY_KIND)

	# if there is a specific template for the detection
	if(EXISTS ${PROJECT_SOURCE_DIR}/config/gl/has_${GL_LIB}_${QUERY}.cpp.in)
		# use it
		configure_file(
			${PROJECT_SOURCE_DIR}/config/gl/has_${GL_LIB}_${QUERY}.cpp.in
			${PROJECT_BINARY_DIR}/gl/has_${GL_LIB}_${QUERY}.cpp
		)
	# if there is a specific template for the detection
	elseif(EXISTS ${PROJECT_SOURCE_DIR}/config/gl/has_${GL_LIB}_${QUERY}.cpp)
		# use it
		configure_file(
			${PROJECT_SOURCE_DIR}/config/gl/has_${GL_LIB}_${QUERY}.cpp
			${PROJECT_BINARY_DIR}/gl/has_${GL_LIB}_${QUERY}.cpp
		)
	else()
		# otherwise use the generic template
		configure_file(
			${PROJECT_SOURCE_DIR}/config/gl/has_${GL_LIB}_${QUERY_KIND}.cpp.in
			${PROJECT_BINARY_DIR}/gl/has_${GL_LIB}_${QUERY}.cpp
		)
	endif()

	if(EXISTS ${PROJECT_SOURCE_DIR}/config/gl/has_${GL_LIB}_${QUERY_KIND}.ipp.in)
		configure_file(
			${PROJECT_SOURCE_DIR}/config/gl/has_${GL_LIB}_${QUERY_KIND}.ipp.in
			${PROJECT_BINARY_DIR}/gl/has_${GL_LIB}_${QUERY}.ipp
		)
	endif()

	set(OGLPLUS_CONFIG_REQUIRED_INCLUDE_DIRS
		"${OGLPLUS_GL_INCLUDE_DIRS};"
		"${PROJECT_SOURCE_DIR}/include;"
		"${PROJECT_SOURCE_DIR}/implement;"
		"${PROJECT_SOURCE_DIR}/utils;"
		"${PROJECT_BINARY_DIR}/include"
	)
	set(OGLPLUS_CONFIG_REQUIRED_LIBRARIES "${OGLPLUS_GL_LIBRARIES}")

	foreach(INIT_LIB GLX GLFW3 GLFW GLUT SDL)
		if(${INIT_LIB}_FOUND)
			if(${INIT_LIB}_INCLUDE_DIRS)
				set(OGLPLUS_CONFIG_REQUIRED_INCLUDE_DIRS
					"${OGLPLUS_CONFIG_REQUIRED_INCLUDE_DIRS};"
					"${${INIT_LIB}_INCLUDE_DIRS}"
				)
			endif()

			if(${INIT_LIB}_LIBRARY_DIRS)
				set(OGLPLUS_CONFIG_REQUIRED_LIBRARY_DIRS
					"${OGLPLUS_CONFIG_REQUIRED_INCLUDE_DIRS};"
					"${${INIT_LIB}_LIBRARY_DIRS}"
				)
			endif()

			if(${INIT_LIB}_LIBRARIES)
				set(OGLPLUS_CONFIG_REQUIRED_LIBRARIES
					"${OGLPLUS_CONFIG_REQUIRED_LIBRARIES};"
					"${${INIT_LIB}_LIBRARIES}"
				)
			endif()
			break()
		endif()
	endforeach()

	try_run(
		RUNS_WITH_${GL_LIB}_${QUERY} COMPILED_WITH_${GL_LIB}_${QUERY}
		"${PROJECT_BINARY_DIR}/gl"
		"${PROJECT_BINARY_DIR}/gl/has_${GL_LIB}_${QUERY}.cpp"
		COMPILE_DEFINITIONS ${OGLPLUS_CPP_STD_COMPILER_SWITCH}
		CMAKE_FLAGS
			"-DINCLUDE_DIRECTORIES:STRING=${OGLPLUS_CONFIG_REQUIRED_INCLUDE_DIRS} "
			"-DLIBRARY_DIRECTORIES:STRING=${OGLPLUS_CONFIG_REQUIRED_LIBRARY_DIRS} "
			"-DLINK_DIRECTORIES:STRING=${OGLPLUS_CONFIG_REQUIRED_LIBRARY_DIRS} "
			"-DLINK_LIBRARIES:STRING=${OGLPLUS_CONFIG_REQUIRED_LIBRARIES} "
	)
	if(COMPILED_WITH_${GL_LIB}_${QUERY})
		if(RUNS_WITH_${GL_LIB}_${QUERY} EQUAL 0)
			set(HAS_${GL_LIB}_${QUERY} TRUE PARENT_SCOPE)
		else()
			set(OGLPLUS_NO_${GL_LIB}_${QUERY} TRUE PARENT_SCOPE)
		endif()
	else()
		set(OGLPLUS_NO_${GL_LIB}_${QUERY} TRUE PARENT_SCOPE)
	endif()
	unset(RUNS_WITH_${GL_LIB}_${QUERY})
	unset(COMPILED_WITH_${GL_LIB}_${QUERY})
endfunction()

configure_file(
	${PROJECT_SOURCE_DIR}/config/gl/init_GL.ipp.in
	${PROJECT_BINARY_DIR}/gl/init_GL.ipp
)

