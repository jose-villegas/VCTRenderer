/**
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <GL/glew.h>

bool do_query_gl(void)
{
#if !defined GL_NV_path_rendering
	// Missing the GL_NV_path_rendering extension
	return false;
#elif !GL_NV_path_rendering
	// Missing the GL_NV_path_rendering extension
	return false;
#elif	!defined(GL_FONT_X_MIN_BOUNDS_BIT_NV) ||\
	!defined(GL_FONT_Y_MIN_BOUNDS_BIT_NV) ||\
	!defined(GL_FONT_X_MAX_BOUNDS_BIT_NV) ||\
	!defined(GL_FONT_Y_MAX_BOUNDS_BIT_NV) ||\
	!defined(GL_FONT_UNITS_PER_EM_BIT_NV) ||\
	!defined(GL_FONT_ASCENDER_BIT_NV) ||\
	!defined(GL_FONT_DESCENDER_BIT_NV) ||\
	!defined(GL_FONT_HEIGHT_BIT_NV) ||\
	!defined(GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV) ||\
	!defined(GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV) ||\
	!defined(GL_FONT_UNDERLINE_POSITION_BIT_NV) ||\
	!defined(GL_FONT_UNDERLINE_THICKNESS_BIT_NV) ||\
	!defined(GL_FONT_HAS_KERNING_BIT_NV)

	// GL_NV_path_rendering extension broken
	return false;
#else
	return true;
#endif
}

int main(void)
{
	return do_query_gl()?0:1;
}
