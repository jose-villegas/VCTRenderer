//  File doc/quickbook/oalplus/quickref/enums/data_format.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/data_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_data_format

enum class DataFormat : ALenum
{
	Mono8    = AL_FORMAT_MONO8,
	Mono16   = AL_FORMAT_MONO16,
	Stereo8  = AL_FORMAT_STEREO8,
	Stereo16 = AL_FORMAT_STEREO16
};

template <>
__Range<DataFormat> __EnumValueRange<DataFormat>(void);

__StrCRef __EnumValueName(DataFormat);

//]
