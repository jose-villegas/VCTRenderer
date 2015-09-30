#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

macro(synchronize_directories SRC_DIR DST_DIR DIRNAME PARENT_TARGET)
	make_directory(${DST_DIR})
	file(GLOB DIR_ENTRIES RELATIVE ${SRC_DIR} ${SRC_DIR}/[^.]*)
	foreach(DIR_ENTRY ${DIR_ENTRIES})
		if(IS_DIRECTORY ${SRC_DIR}/${DIR_ENTRY})
			synchronize_directories(
				${SRC_DIR}/${DIR_ENTRY}
				${DST_DIR}/${DIR_ENTRY}
				${DIRNAME}-${DIRENTRY}
				${PARENT_TARGET}
			)
		else()
			add_custom_command(
				OUTPUT ${DST_DIR}/${DIR_ENTRY}
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
					${SRC_DIR}/${DIR_ENTRY}
					${DST_DIR}/${DIR_ENTRY}
				DEPENDS ${SRC_DIR}/${DIR_ENTRY}
				COMMENT "Updating file ${DIR_ENTRY}."
			)
			add_custom_target(
				copy-${PARENT_TARGET}-${DIRNAME}-${DIR_ENTRY}
				DEPENDS ${DST_DIR}/${DIR_ENTRY}
			)
			add_dependencies(
				${PARENT_TARGET}
				copy-${PARENT_TARGET}-${DIRNAME}-${DIR_ENTRY}
			)
			set_property(
				TARGET copy-${PARENT_TARGET}-${DIRNAME}-${DIR_ENTRY}
				PROPERTY FOLDER "Example/Advanced/${DIRNAME}"
			)
		endif()
	endforeach(DIR_ENTRY)
endmacro(synchronize_directories)

macro(add_resource_directory DIRNAME PARENT_TARGET)
	synchronize_directories(
		${CMAKE_CURRENT_SOURCE_DIR}/${DIRNAME}
		${CMAKE_CURRENT_BINARY_DIR}/${DIRNAME}
		${DIRNAME}
		${PARENT_TARGET}
	)
endmacro(add_resource_directory)

add_custom_target(oglplus-advanced-examples ALL)
set_property(TARGET oglplus-advanced-examples PROPERTY FOLDER "Example/Advanced")

add_subdirectory("cloud_trace")
add_subdirectory("spectra")
