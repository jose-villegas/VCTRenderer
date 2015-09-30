//  File doc/quickbook/oalplus/quickref/enums/source_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/source_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_source_type

enum class SourceType : ALenum
{
	Undetermined = AL_UNDETERMINED,
	Static       = AL_STATIC,
	Streaming    = AL_STREAMING
};

template <>
__Range<SourceType> __EnumValueRange<SourceType>(void);

__StrCRef __EnumValueName(SourceType);

//]
