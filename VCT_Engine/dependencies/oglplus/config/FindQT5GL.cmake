#  Copyright 2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
if(
	NOT(OGLPLUS_FORCE_GL_INIT_LIB) OR
	("${OGLPLUS_FORCE_GL_INIT_LIB}" STREQUAL "NONE") OR
	("${OGLPLUS_FORCE_GL_INIT_LIB}" STREQUAL "QT5GL")
)

find_package(Qt5Core QUIET)
find_package(Qt5Gui QUIET)
find_package(Qt5OpenGL QUIET)

if(Qt5Core_FOUND AND Qt5Gui_FOUND AND Qt5OpenGL_FOUND)
	set(QT5GL_FOUND True)
	message(STATUS "Found Qt5")

	foreach(MODULE Qt5Core Qt5Gui Qt5OpenGL)
		foreach(FLAGS DEFINITIONS EXECUTABLE_COMPILE_FLAGS)
			if(${MODULE}_${FLAGS})
				foreach(DEF ${${MODULE}_${FLAGS}})
					set(${MODULE}_CXX_FLAGS "${${MODULE}_CXX_FLAGS} ${DEF}")
				endforeach()
			endif()
		endforeach()
		unset(${MODULE}_DEFINITIONS)
	endforeach()
endif()

endif()
