#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)

# doxygen working directory
set(DOXYGEN_WD ${CMAKE_CURRENT_SOURCE_DIR})

# HTML documentation
# the OpenGL logo
configure_file(
	${PROJECT_SOURCE_DIR}/doc/logo/ogl.jpg
	${CMAKE_CURRENT_BINARY_DIR}/html/logo/ogl.jpg
	COPYONLY
)

# the gallery.html file
set(GALLERY_HTML ${CMAKE_CURRENT_BINARY_DIR}/gallery.html)

file(WRITE ${GALLERY_HTML} "\n")
file(APPEND ${GALLERY_HTML} "<script src='http://ajax.googleapis.com/ajax/libs/jquery/1/jquery.js'></script>\n")
file(APPEND ${GALLERY_HTML} "<script src='../galleria/galleria-1.2.5.min.js'></script>\n")
file(APPEND ${GALLERY_HTML} "<div id='gallery'>\n")

# get a list of all oglplus examples
file(GLOB OGLPLUS_EXAMPLES "${PROJECT_SOURCE_DIR}/example/oglplus/[0-9][0-9][0-9]_*.cpp")
foreach(EXAMPLE ${OGLPLUS_EXAMPLES})
	get_filename_component(EXE_NAME "${EXAMPLE}" NAME_WE)
	file(
		APPEND ${GALLERY_HTML}
		"<img "
		"src='${EXE_NAME}.png' "
		"title='Example ${EXE_NAME}.cpp' "
		"longdesc='oglplus_2${EXE_NAME}_8cpp-example.html'"
		"/>\n"
	)
endforeach()

file(APPEND ${GALLERY_HTML} "</div>\n")
file(APPEND ${GALLERY_HTML} "<script>\n")
file(APPEND ${GALLERY_HTML} "	Galleria.loadTheme('../galleria/themes/classic/galleria.classic.min.js');\n")
file(APPEND ${GALLERY_HTML} "	$('#gallery').galleria({\n")
file(APPEND ${GALLERY_HTML} "		width: 500,\n")
file(APPEND ${GALLERY_HTML} "		height: 500,\n")
file(APPEND ${GALLERY_HTML} "		autoplay: 1000\n")
file(APPEND ${GALLERY_HTML} "	});\n")
file(APPEND ${GALLERY_HTML} "</script>\n")

unset(GALLERY_HTML)

# get the version string
file(READ ${PROJECT_SOURCE_DIR}/VERSION OGLPLUS_VERSION_STR)

# doxyfile for building html docs
set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile-html)
file(WRITE ${DOXYFILE} "@INCLUDE = ${DOXYGEN_WD}/Doxyfile.oglplus\n")
file(APPEND ${DOXYFILE} "IMAGE_PATH += ../../logo/ \n")
if(NOT OGLPLUS_NO_EXAMPLES)
	file(APPEND ${DOXYFILE} "IMAGE_PATH += ${PROJECT_BINARY_DIR}/example/oglplus \n")
endif()
file(APPEND ${DOXYFILE} "OUTPUT_DIRECTORY = ${CMAKE_CURRENT_BINARY_DIR}\n")
file(APPEND ${DOXYFILE} "GENERATE_TAGFILE = ${CMAKE_CURRENT_BINARY_DIR}/doxygen.tags\n")
file(APPEND ${DOXYFILE} "TAGFILES += ${CMAKE_CURRENT_BINARY_DIR}/../oalplus/doxygen.tags\n")
file(APPEND ${DOXYFILE} "TAGFILES += ${CMAKE_CURRENT_BINARY_DIR}/../eglplus/doxygen.tags\n")
file(APPEND ${DOXYFILE} "PROJECT_NUMBER = ${OGLPLUS_VERSION_STR}\n")
file(APPEND ${DOXYFILE} "QUIET = YES\n")

if(OGLPLUS_NO_SCREENSHOTS)
	file(APPEND ${DOXYFILE} "ALIASES += oglplus_screenshot{1}=\"\"\n")
endif()

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
	COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Generating OGLplus HTML documentation"
)

add_custom_target(
	oglplus-prebuild-doc
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
)
set_property(TARGET oglplus-prebuild-doc PROPERTY FOLDER "Documentation/OGLplus")

add_custom_target(
	oglplus-doc
	COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	DEPENDS oglplus-prebuild-doc
)
set_property(TARGET oglplus-doc PROPERTY FOLDER "Documentation/OGLplus")

unset(DOXYFILE)

unset(DOC_DEPENDS)
