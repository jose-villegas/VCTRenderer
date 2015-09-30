/**
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR 3
#define OGLPLUS_CONFIG_QUERY_GL_VER_MINOR 2

#include "init_GL.ipp"
#include "has_GL_KHR_debug.ipp"

bool do_query_gl(void)
{
	if(!does_have_KHR_debug())
	{
		return false;
	}

	unsigned missing_gl_symbols = 0;
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glPushDebugGroup);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glPopDebugGroup);
	return missing_gl_symbols == 0;
}
