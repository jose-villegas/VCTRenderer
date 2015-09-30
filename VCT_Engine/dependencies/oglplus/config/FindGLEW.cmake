#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
if(${WIN32})
	oglplus_common_find_module(GLEW glew GL/glew.h glew32)
else()
	oglplus_common_find_module(GLEW glew GL/glew.h GLEW)
endif()
