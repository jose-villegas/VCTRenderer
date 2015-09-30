#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

enable_testing()
include(CTest)

string(TOLOWER ${OGLPLUS_GL_INIT_LIB} OGLPLUS_TEST_FIXTURE)

# check the harness dependencies and requirements
set(FIXTURE_CAN_BE_BUILT true)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fixture_${OGLPLUS_TEST_FIXTURE}.cpp)
	require_all_dependencies(${OGLPLUS_TEST_FIXTURE}_main FIXTURE_CAN_BE_BUILT)
else()
	message(
		FATAL_ERROR
		"A test fixture using '${OGLPLUS_TEST_FIXTURE}' is not implemented. "
		"Either disable the building of tests or choose another implementation."
	)
	set(FIXTURE_CAN_BE_BUILT false)
endif()
if(NOT ${FIXTURE_CAN_BE_BUILT})
	message(
		FATAL_ERROR
		"Some of the requirements for the '${OGLPLUS_TEST_FIXTURE}' "
		"test fixture were not met. Please choose a different build "
		"configuration or install the required libraries or use "
		"a different compiler."
	)
	return()
endif()

include_directories(${PROJECT_SOURCE_DIR}/utils)

add_library(
	oglplus_test_fixture
	STATIC
	EXCLUDE_FROM_ALL
	${CMAKE_CURRENT_SOURCE_DIR}/fixture_${OGLPLUS_TEST_FIXTURE}.cpp
)
# add the dependencies for the fixture
add_all_dependencies(fixture_${OGLPLUS_TEST_FIXTURE} oglplus_test_fixture)
set_property(TARGET oglplus_test_fixture PROPERTY FOLDER "Test/OGLplus")

# make a list of libraries that we're going to link to
set(OGLPLUS_TEST_LIBS
	oglplus_test_fixture
	${OGLPLUS_GL_LIBRARIES}
)


function(add_oglplus_test TEST_NAME TEST_LIBRARIES BUILD_ONLY)

	add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${TEST_NAME}.cpp)
	target_link_libraries(${TEST_NAME} ${TEST_LIBRARIES})
	set_property(TARGET ${TEST_NAME} PROPERTY FOLDER "Test/OGLplus")

	if(NOT BUILD_ONLY)
		target_link_libraries(
			${TEST_NAME}
			${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
			oglplus_test_fixture
		)
	endif()

	add_test(
		build-test-${TEST_NAME}
		"${CMAKE_COMMAND}"
		--build ${CMAKE_BINARY_DIR}
		--target ${TEST_NAME}
	)
	set_tests_properties(
		build-test-${TEST_NAME}
		PROPERTIES FOLDER
		"Test/OGLplus"
	)

	if(NOT BUILD_ONLY)
		add_test(exec-test-${TEST_NAME} ${TEST_NAME})
		set_tests_properties(
			exec-test-${TEST_NAME}
			PROPERTIES DEPENDS
			build-test-${TEST_NAME}
		)
		set_tests_properties(
			exec-test-${TEST_NAME}
			PROPERTIES FOLDER
			"Test/OGLplus"
		)
	endif()
endfunction()

function(oglplus_build_test TEST_NAME TEST_LIBRARIES)
	add_oglplus_test(${TEST_NAME} "${TEST_LIBRARIES}" TRUE)
endfunction()

function(oglplus_build_test_no_fixture TEST_NAME)
	add_oglplus_test(${TEST_NAME} "" TRUE)
endfunction()

function(oglplus_exec_test TEST_NAME TEST_LIBRARIES)
	add_oglplus_test(${TEST_NAME} "${TEST_LIBRARIES}" FALSE)
endfunction()

function(oglplus_exec_test_no_fixture TEST_NAME)
	add_oglplus_test(${TEST_NAME} "" FALSE)
endfunction()
