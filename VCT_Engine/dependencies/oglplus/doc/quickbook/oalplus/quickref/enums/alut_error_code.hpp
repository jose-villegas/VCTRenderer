//  File doc/quickbook/oalplus/quickref/enums/alut_error_code.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/alut_error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_alut_error_code

enum class ALUTErrorCode : ALenum
{
	OutOfMemory            = AL_OUT_OF_MEMORY,
	InvalidOperation       = AL_INVALID_OPERATION,
	NoCurrentContext       = AL_NO_CURRENT_CONTEXT,
	AlErrorOnEntry         = AL_AL_ERROR_ON_ENTRY,
	AlcErrorOnEntry        = AL_ALC_ERROR_ON_ENTRY,
	UnsupportedFileType    = AL_UNSUPPORTED_FILE_TYPE,
	UnsupportedFileSubtype = AL_UNSUPPORTED_FILE_SUBTYPE,
	CorruptOrTruncatedData = AL_CORRUPT_OR_TRUNCATED_DATA
};

template <>
__Range<ALUTErrorCode> __EnumValueRange<ALUTErrorCode>(void);

__StrCRef __EnumValueName(ALUTErrorCode);

//]
