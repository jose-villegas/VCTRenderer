/**
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR 3
#define OGLPLUS_CONFIG_QUERY_GL_VER_MINOR 0

#include "init_GL.ipp"
#include "has_GL_ARB_transform_feedback2.ipp"

#include <cstring>
#include <cstddef>

bool do_query_gl(void)
{
	if(!does_have_ARB_transform_feedback2())
	{
		return false;
	}

	GLuint x = 0;
	glGenTransformFeedbacks(1, &x);
	if(glGetError() != GL_NO_ERROR)
	{
		return false;
	}
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, x);
	if(glGetError() != GL_NO_ERROR)
	{
		return false;
	}
	if(glIsTransformFeedback(x) != GL_TRUE)
	{
		return false;
	}
	if(glGetError() != GL_NO_ERROR)
	{
		return false;
	}
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	if(glGetError() != GL_NO_ERROR)
	{
		return false;
	}
	glDeleteTransformFeedbacks(1, &x);
	if(glGetError() != GL_NO_ERROR)
	{
		return false;
	}
	return true;
}

