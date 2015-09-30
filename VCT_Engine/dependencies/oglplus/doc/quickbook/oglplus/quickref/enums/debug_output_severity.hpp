//  File doc/quickbook/oglplus/quickref/enums/debug_output_severity.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/debug_output_severity.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_debug_output_severity

enum class DebugOutputSeverity : GLenum
{
	High         = GL_DEBUG_SEVERITY_HIGH,
	Medium       = GL_DEBUG_SEVERITY_MEDIUM,
	Low          = GL_DEBUG_SEVERITY_LOW,
	Notification = GL_DEBUG_SEVERITY_NOTIFICATION,
	DontCare     = GL_DONT_CARE
};

template <>
__Range<DebugOutputSeverity> __EnumValueRange<DebugOutputSeverity>(void);

__StrCRef __EnumValueName(DebugOutputSeverity);

//]
