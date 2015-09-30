/**
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR 3
#define OGLPLUS_CONFIG_QUERY_GL_VER_MINOR 2

#include "init_GL.ipp"
#include "has_GL_ARB_separate_shader_objects.ipp"

bool do_query_gl(void)
{
	if(!does_have_ARB_separate_shader_objects())
	{
		return false;
	}

	unsigned missing_gl_symbols = 0;

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1i);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2i);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3i);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4i);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1ui);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2ui);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3ui);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4ui);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1f);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2f);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3f);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4f);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1d);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2d);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3d);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4d);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1iv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2iv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3iv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4iv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1uiv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2uiv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3uiv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4uiv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform1dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniform4dv);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x3fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x4fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x2fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x4fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x2fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x3fv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4fv);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix2x4dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix3x4dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x2dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4x3dv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramUniformMatrix4dv);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glUseProgramStages);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glActiveShaderProgram);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glCreateShaderProgramv);

	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGenProgramPipelines);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glDeleteProgramPipelines);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glIsProgramPipeline);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glBindProgramPipeline);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glProgramParameteri);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGetProgramPipelineiv);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glGetProgramPipelineInfoLog);
	OGLPLUS_CONFIG_ASSERT_GL_FUNC(glValidateProgramPipeline);

	return missing_gl_symbols == 0;
}
