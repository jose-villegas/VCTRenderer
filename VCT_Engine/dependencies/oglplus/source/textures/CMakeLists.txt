#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)

add_custom_target(oglplus-textures)
set_property(TARGET oglplus-textures PROPERTY FOLDER "Textures")

set(NOT_AVAILABLE_PNG ${CMAKE_CURRENT_SOURCE_DIR}/not_available.png)

# SVG -> PNG
find_program(INKSCAPE_EXECUTABLE inkscape)
file(GLOB OGLPLUS_SVG_TEXTURES "./*.svg")
foreach(SVG_FILE ${OGLPLUS_SVG_TEXTURES})
	get_filename_component(TEXTURE_NAME ${SVG_FILE} NAME_WE)
	set(PNG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEXTURE_NAME}.png")

	set(PREBUILT_PNG ${PROJECT_SOURCE_DIR}/_prebuilt/textures/${TEXTURE_NAME}.png)

	if(EXISTS ${PREBUILT_PNG})
		add_custom_command(
			OUTPUT ${PNG_FILE}
			COMMAND ${CMAKE_COMMAND} -E copy
				"${PREBUILT_PNG}"
				"${PNG_FILE}"
			COMMENT "Copying pre-built texture ${TEXTURE_NAME}"
		)
	elseif(INKSCAPE_EXECUTABLE)
		add_custom_command(
			OUTPUT ${PNG_FILE}
			COMMAND ${INKSCAPE_EXECUTABLE}
				--without-gui
				--file="${SVG_FILE}"
				--export-png="${PNG_FILE}"
			MAIN_DEPENDENCY ${SVG_FILE}
			COMMENT "Converting texture ${TEXTURE_NAME}"
		)
	else()
		add_custom_command(
			OUTPUT ${PNG_FILE}
			COMMAND ${CMAKE_COMMAND} -E copy
				"${NOT_AVAILABLE_PNG}"
				"${PNG_FILE}"
			COMMENT "Copying default fallback texture ${TEXTURE_NAME}"
		)
	endif()

	add_custom_target(texture-${TEXTURE_NAME} DEPENDS ${PNG_FILE})
	add_dependencies(oglplus-textures texture-${TEXTURE_NAME})
	set_property(TARGET texture-${TEXTURE_NAME} PROPERTY FOLDER "Textures")
endforeach(SVG_FILE)

# POV -> PNG
find_program(POVRAY_EXECUTABLE povray)
file(GLOB OGLPLUS_POV_TEXTURES "./*.pov")
foreach(POV_FILE ${OGLPLUS_POV_TEXTURES})
	get_filename_component(TEXTURE_NAME ${POV_FILE} NAME_WE)
	set(PNG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEXTURE_NAME}.png")

	set(PREBUILT_PNG ${PROJECT_SOURCE_DIR}/_prebuilt/textures/${TEXTURE_NAME}.png)

	if(EXISTS ${PREBUILT_PNG})
		add_custom_command(
			OUTPUT ${PNG_FILE}
			COMMAND ${CMAKE_COMMAND} -E copy
				"${PREBUILT_PNG}"
				"${PNG_FILE}"
			COMMENT "Copying pre-built texture ${TEXTURE_NAME}"
		)
	elseif(POVRAY_EXECUTABLE)
		set(INI_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_NAME}.ini")
		if(EXISTS ${INI_FILE})
			add_custom_command(
				OUTPUT ${PNG_FILE}
				COMMAND ${POVRAY_EXECUTABLE}
					"${INI_FILE}"
					"-V"
					"-D"
					"+I${POV_FILE}"
					"+O${PNG_FILE}"
					"+L${CMAKE_CURRENT_SOURCE_DIR}"
				MAIN_DEPENDENCY ${POV_FILE}
				COMMENT "Converting texture ${TEXTURE_NAME}"
			)
		else()
			add_custom_command(
				OUTPUT ${PNG_FILE}
				COMMAND ${POVRAY_EXECUTABLE}
					"+W512"
					"+H512"
					"+Q9"
					"+A0.3"
					"+UA"
					"-V"
					"-D"
					"+I${POV_FILE}"
					"+O${PNG_FILE}"
					"+L${CMAKE_CURRENT_SOURCE_DIR}"
				MAIN_DEPENDENCY ${POV_FILE}
				COMMENT "Converting texture ${TEXTURE_NAME}"
			)
		endif()
	else()
		add_custom_command(
			OUTPUT ${PNG_FILE}
			COMMAND ${CMAKE_COMMAND} -E copy
				"${NOT_AVAILABLE_PNG}"
				"${PNG_FILE}"
			COMMENT "Copying default fallback texture ${TEXTURE_NAME}"
		)
	endif()

	add_custom_target(texture-${TEXTURE_NAME} DEPENDS ${PNG_FILE})
	add_dependencies(oglplus-textures texture-${TEXTURE_NAME})
	set_property(TARGET texture-${TEXTURE_NAME} PROPERTY FOLDER "Textures")
endforeach(POV_FILE)

