//  File doc/quickbook/oalplus/quickref/enums/alc_error_code.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/alc_error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_alc_error_code

enum class ALCErrorCode : ALenum
{
	NoError        = AL_NO_ERROR,
	InvalidDevice  = AL_INVALID_DEVICE,
	InvalidContext = AL_INVALID_CONTEXT,
	InvalidEnum    = AL_INVALID_ENUM,
	InvalidValue   = AL_INVALID_VALUE,
	OutOfMemory    = AL_OUT_OF_MEMORY
};

template <>
__Range<ALCErrorCode> __EnumValueRange<ALCErrorCode>(void);

__StrCRef __EnumValueName(ALCErrorCode);

//]
