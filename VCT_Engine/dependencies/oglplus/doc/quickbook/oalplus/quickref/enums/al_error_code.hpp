//  File doc/quickbook/oalplus/quickref/enums/al_error_code.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/al_error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_al_error_code

enum class ALErrorCode : ALenum
{
	NoError          = AL_NO_ERROR,
	InvalidName      = AL_INVALID_NAME,
	InvalidEnum      = AL_INVALID_ENUM,
	InvalidValue     = AL_INVALID_VALUE,
	InvalidOperation = AL_INVALID_OPERATION,
	OutOfMemory      = AL_OUT_OF_MEMORY
};

template <>
__Range<ALErrorCode> __EnumValueRange<ALErrorCode>(void);

__StrCRef __EnumValueName(ALErrorCode);

//]
