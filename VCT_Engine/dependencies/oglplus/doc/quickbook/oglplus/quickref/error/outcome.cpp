/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_Outcome_example_1
__VertexShader shader;

const char* source_glsl_430 = /* ... */;
const char* source_glsl_330 = /* ... */;
const char* source_glsl_150 = /* ... */;
const char* fallback_source = /* ... */;

shader.Source(source_glsl_430).Compile(std::nothrow).DoneWithoutError() ||
shader.Source(source_glsl_330).Compile(std::nothrow).DoneWithoutError() ||
shader.Source(source_glsl_150).Compile(std::nothrow).DoneWithoutError() ||
/* ... */
shader.Source(fallback_source).Compile(std::nothrow).Done();
//]

//[oglplus_error_PositiveOutcome_example_1
__VertexShader shader;

if(auto compiled_shader = __Succeeded(shader.Source(source).Compile(std::nothrow)))
{
	/* Shader compiled successfully */
}
else
{
	/* Shader compilation failed */
}
//]

//[oglplus_error_NegativeOutcome_example_1
__VertexShader shader;

if(auto compilation_failed = __Failed(shader.Source(source).Compile(std::nothrow)))
{
	/* Shader compilation failed */
}
else
{
	/* Shader compiled successfully */
}
//]
