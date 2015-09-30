/**
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR 3
#define OGLPLUS_CONFIG_QUERY_GL_VER_MINOR 2

#include "init_GL.ipp"
#include "has_GL_EXT_direct_state_access.ipp"

bool do_query_gl(void)
{
	if(!does_have_EXT_direct_state_access())
	{
		return false;
	}

	unsigned missing_gl_symbols = 0;

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1iEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2iEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3iEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4iEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1uiEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2uiEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3uiEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4uiEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1fEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2fEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3fEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4fEXT);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1ivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2ivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3ivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4ivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1uivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2uivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3uivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4uivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4fvEXT);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x3fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x4fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x2fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x4fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x2fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x3fvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4fvEXT);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureParameteriEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureParameterivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureParameterfEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureParameterfvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureImage1DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureImage2DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureSubImage1DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureSubImage2DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glCopyTextureImage1DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glCopyTextureImage2DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glCopyTextureSubImage1DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glCopyTextureSubImage2DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGetTextureImageEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGetTextureParameterfvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGetTextureParameterivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGetTextureLevelParameterfvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGetTextureLevelParameterivEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureImage3DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glTextureSubImage3DEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glCopyTextureSubImage3DEXT);

	return missing_gl_symbols == 0;
}
