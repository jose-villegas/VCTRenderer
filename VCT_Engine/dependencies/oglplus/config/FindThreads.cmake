#  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
if(
	(${CMAKE_COMPILER_IS_GNUCXX}) OR
	("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
)
	set(THREADS_FOUND 1)
	set(THREADS_CXXFLAGS -pthread)
	set(THREADS_LIBRARIES pthread)
endif()
