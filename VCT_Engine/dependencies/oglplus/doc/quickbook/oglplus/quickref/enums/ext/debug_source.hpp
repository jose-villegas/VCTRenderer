//  File doc/quickbook/oglplus/quickref/enums/ext/debug_source.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_source.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_source

enum class DebugSource : GLenum
{
	API            = GL_DEBUG_SOURCE_API,
	WindowSystem   = GL_DEBUG_SOURCE_WINDOW_SYSTEM,
	ShaderCompiler = GL_DEBUG_SOURCE_SHADER_COMPILER,
	ThirdParty     = GL_DEBUG_SOURCE_THIRD_PARTY,
	Application    = GL_DEBUG_SOURCE_APPLICATION,
	Other          = GL_DEBUG_SOURCE_OTHER,
	DontCare       = GL_DONT_CARE
};

template <>
__Range<DebugSource> __EnumValueRange<DebugSource>(void);

__StrCRef __EnumValueName(DebugSource);

//]
