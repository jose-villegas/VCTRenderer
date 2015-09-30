#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_custom_target(oglplus-advanced-example-cloud_trace-tools)
set_property(
	TARGET oglplus-advanced-example-cloud_trace-tools
	PROPERTY FOLDER "Example/Advanced/CloudTrace/Tools"
)

add_executable(
	blender2csv
	EXCLUDE_FROM_ALL
	blender2csv.cpp
)
set_property(
	TARGET blender2csv
	PROPERTY FOLDER "Example/Advanced/CloudTrace/Tools"
)

add_dependencies(oglplus-advanced-example-cloud_trace-tools blender2csv)
add_dependencies(oglplus-advanced-examples oglplus-advanced-example-cloud_trace-tools)

