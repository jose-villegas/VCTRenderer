//  File doc/quickbook/oglplus/quickref/enums/ext/debug_output_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_output_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_output_type

enum class DebugOutputARBType : GLenum
{
	Error              = GL_DEBUG_TYPE_ERROR_ARB,
	DeprecatedBehavior = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB,
	UndefinedBehavior  = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB,
	Portability        = GL_DEBUG_TYPE_PORTABILITY_ARB,
	Performance        = GL_DEBUG_TYPE_PERFORMANCE_ARB,
	Other              = GL_DEBUG_TYPE_OTHER_ARB,
	DontCare           = GL_DONT_CARE
};

template <>
__Range<DebugOutputARBType> __EnumValueRange<DebugOutputARBType>(void);

__StrCRef __EnumValueName(DebugOutputARBType);

//]
