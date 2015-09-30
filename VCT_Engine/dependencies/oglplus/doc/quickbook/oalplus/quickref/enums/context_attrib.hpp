//  File doc/quickbook/oalplus/quickref/enums/context_attrib.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/context_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_context_attrib

enum class ContextAttrib : ALenum
{
	Frequency     = ALC_FREQUENCY,
	Refresh       = ALC_REFRESH,
	Sync          = ALC_SYNC,
	MonoSources   = ALC_MONO_SOURCES,
	StereoSources = ALC_STEREO_SOURCES
};

template <>
__Range<ContextAttrib> __EnumValueRange<ContextAttrib>(void);

__StrCRef __EnumValueName(ContextAttrib);

//]
