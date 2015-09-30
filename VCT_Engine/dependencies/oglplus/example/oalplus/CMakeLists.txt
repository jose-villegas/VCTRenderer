#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)

function(add_examples EXAMPLE_PATTERN PARENT_TARGET COMMON_LIBS)

	file(GLOB EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_PATTERN}.cpp")

	foreach(EXAMPLE_PATH ${EXAMPLES})
		get_filename_component(EXAMPLE_NAME "${EXAMPLE_PATH}" NAME_WE)
		set(TARGET_NAME "${EXAMPLE_NAME}")

		set(EXAMPLE_CAN_BE_BUILT true)
		require_all_dependencies(${EXAMPLE_NAME} EXAMPLE_CAN_BE_BUILT)

		if(EXAMPLE_CAN_BE_BUILT)
			add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL ${EXAMPLE_PATH})
			target_link_libraries(${TARGET_NAME} ${COMMON_LIBS})
			add_all_dependencies(${TARGET_NAME})
			add_dependencies("${PARENT_TARGET}" ${TARGET_NAME})
			set_property(
				TARGET ${TARGET_NAME}
				PROPERTY FOLDER "Example/OALplus"
			)
		endif()
	endforeach()
endfunction(add_examples)

if(OPENAL_FOUND)
	include_directories(${OPENAL_INCLUDE_DIRS})

	set(EXAMPLE_LIBS ${OPENAL_LIBRARIES} oalplus)

	add_custom_target(oalplus-examples ALL)
	set_property(TARGET oalplus-examples PROPERTY FOLDER "Example/OALplus")

	add_definitions(-DOALPLUS_LINK_LIBRARY=1)
	add_examples("[0-9][0-9][0-9]*" oalplus-examples "${EXAMPLE_LIBS}")
endif(OPENAL_FOUND)
