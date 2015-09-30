//  File doc/quickbook/oglplus/quickref/enums/ext/debug_output_source.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_output_source.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_output_source

enum class DebugOutputARBSource : GLenum
{
	API            = GL_DEBUG_SOURCE_API_ARB,
	WindowSystem   = GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB,
	ShaderCompiler = GL_DEBUG_SOURCE_SHADER_COMPILER_ARB,
	ThirdParty     = GL_DEBUG_SOURCE_THIRD_PARTY_ARB,
	Application    = GL_DEBUG_SOURCE_APPLICATION_ARB,
	Other          = GL_DEBUG_SOURCE_OTHER_ARB,
	DontCare       = GL_DONT_CARE
};

template <>
__Range<DebugOutputARBSource> __EnumValueRange<DebugOutputARBSource>(void);

__StrCRef __EnumValueName(DebugOutputARBSource);

//]
