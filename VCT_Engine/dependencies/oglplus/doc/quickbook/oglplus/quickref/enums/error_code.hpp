//  File doc/quickbook/oglplus/quickref/enums/error_code.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_error_code

enum class ErrorCode : GLenum
{
	NoError                     = GL_NO_ERROR,
	OutOfMemory                 = GL_OUT_OF_MEMORY,
	InvalidEnum                 = GL_INVALID_ENUM,
	InvalidValue                = GL_INVALID_VALUE,
	InvalidOperation            = GL_INVALID_OPERATION,
	InvalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
	StackOverflow               = GL_STACK_OVERFLOW,
	StackUnderflow              = GL_STACK_UNDERFLOW,
	TableTooLarge               = GL_TABLE_TOO_LARGE,
	ContextLost                 = GL_CONTEXT_LOST
};

template <>
__Range<ErrorCode> __EnumValueRange<ErrorCode>(void);

__StrCRef __EnumValueName(ErrorCode);

//]
