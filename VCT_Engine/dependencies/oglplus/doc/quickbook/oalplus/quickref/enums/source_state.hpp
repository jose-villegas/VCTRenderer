//  File doc/quickbook/oalplus/quickref/enums/source_state.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/source_state.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_source_state

enum class SourceState : ALenum
{
	Initial = AL_INITIAL,
	Playing = AL_PLAYING,
	Paused  = AL_PAUSED,
	Stopped = AL_STOPPED
};

template <>
__Range<SourceState> __EnumValueRange<SourceState>(void);

__StrCRef __EnumValueName(SourceState);

//]
