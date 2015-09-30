#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)

add_custom_target(oglplus-models)
set_property(TARGET oglplus-models PROPERTY FOLDER "Models")

# OBJ -> OBJ
file(GLOB OGLPLUS_OBJ_MODELS "./*.obj")
foreach(SRC_OBJ_FILE ${OGLPLUS_OBJ_MODELS})

	get_filename_component(MODEL_NAME ${SRC_OBJ_FILE} NAME_WE)
	set(DST_OBJ_FILE "${CMAKE_CURRENT_BINARY_DIR}/${MODEL_NAME}.obj")

	add_custom_command(
		OUTPUT ${DST_OBJ_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy
			"${SRC_OBJ_FILE}"
			"${DST_OBJ_FILE}"
		COMMENT "Copying model ${MODEL_NAME}"
	)

	add_custom_target(model-${MODEL_NAME} DEPENDS ${DST_OBJ_FILE})
	add_dependencies(oglplus-models model-${MODEL_NAME})
	set_property(TARGET model-${MODEL_NAME} PROPERTY FOLDER "Models")
endforeach(SRC_OBJ_FILE)

