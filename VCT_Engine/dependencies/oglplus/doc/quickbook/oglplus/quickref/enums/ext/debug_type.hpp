//  File doc/quickbook/oglplus/quickref/enums/ext/debug_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_type

enum class DebugType : GLenum
{
	Error              = GL_DEBUG_TYPE_ERROR,
	DeprecatedBehavior = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
	UndefinedBehavior  = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
	Portability        = GL_DEBUG_TYPE_PORTABILITY,
	Performance        = GL_DEBUG_TYPE_PERFORMANCE,
	Other              = GL_DEBUG_TYPE_OTHER,
	Marker             = GL_DEBUG_TYPE_MARKER,
	PushGroup          = GL_DEBUG_TYPE_PUSH_GROUP,
	PopGroup           = GL_DEBUG_TYPE_POP_GROUP,
	DontCare           = GL_DONT_CARE
};

template <>
__Range<DebugType> __EnumValueRange<DebugType>(void);

__StrCRef __EnumValueName(DebugType);

//]
