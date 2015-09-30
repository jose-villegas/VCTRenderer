/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_ProgramBuildError

class ProgramBuildError
 : public __ObjectError
{
public:
	ProgramBuildError(const char* message);

	const __String& Log(void) const; /*<
	Returns the output of the GLSL shader compiler, program linker
	or validator. If no output is available then an empty __String
	is returned.
	>*/
};

//]
//[oglplus_error_CompileError

class CompileError
 : public __ProgramBuildError
{
public:
	CompileError(const char* message);
};

//]
//[oglplus_error_LinkError

class LinkError
 : public __ProgramBuildError
{
public:
	LinkError(const char* message);
};

//]
//[oglplus_error_ValidationError

class ValidationError
 : public __ProgramBuildError
{
public:
	ValidationError(const char* message);
};

//]

