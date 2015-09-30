/**
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR 3
#define OGLPLUS_CONFIG_QUERY_GL_VER_MINOR 2

#include "init_GL.ipp"
#include "has_GL_ARB_gpu_shader_fp64.ipp"

bool do_query_gl(void)
{
	if(!does_have_ARB_gpu_shader_fp64())
	{
		return false;
	}

	unsigned missing_gl_symbols = 0;

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform1d);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform2d);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform3d);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform4d);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform1dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniform4dv);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix2x3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix2x4dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix3x2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix3x4dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix4x2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix4x3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUniformMatrix4dv);

#if GL_EXT_direct_state_access
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1dEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2dEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3dEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4dEXT);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4dvEXT);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x3dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x4dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x2dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x4dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x2dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x3dvEXT);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4dvEXT);
#endif

	return missing_gl_symbols == 0;
}
