#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if(OPENAL_FOUND)
	include_directories(${OPENAL_INCLUDE_DIRS})
	add_library(oalplus EXCLUDE_FROM_ALL
		string.cpp
		enums.cpp
		error.cpp
		context.cpp
	)
	set_property(
		TARGET oalplus
		PROPERTY FOLDER "Library/OALplus"
	)
endif()
