#  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
if(
	NOT(OGLPLUS_FORCE_GL_INIT_LIB) OR
	("${OGLPLUS_FORCE_GL_INIT_LIB}" STREQUAL "NONE") OR
	("${OGLPLUS_FORCE_GL_INIT_LIB}" STREQUAL "QT4GL")
)
	find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
	if(QT_FOUND)
		set(QT4GL_FOUND True)
	endif()
endif()
