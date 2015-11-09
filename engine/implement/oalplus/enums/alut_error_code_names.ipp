//  File implement/oalplus/enums/alut_error_code_names.ipp
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
namespace enums {
OALPLUS_LIB_FUNC StrCRef ValueName_(
	ALUTErrorCode*,
	ALenum value
)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVN_ALUTERRORCODE)
#define OALPLUS_IMPL_EVN_ALUTERRORCODE
{
switch(value)
{
#if defined AL_OUT_OF_MEMORY
	case AL_OUT_OF_MEMORY: return StrCRef("OUT_OF_MEMORY");
#endif
#if defined AL_INVALID_OPERATION
	case AL_INVALID_OPERATION: return StrCRef("INVALID_OPERATION");
#endif
#if defined AL_NO_CURRENT_CONTEXT
	case AL_NO_CURRENT_CONTEXT: return StrCRef("NO_CURRENT_CONTEXT");
#endif
#if defined AL_AL_ERROR_ON_ENTRY
	case AL_AL_ERROR_ON_ENTRY: return StrCRef("AL_ERROR_ON_ENTRY");
#endif
#if defined AL_ALC_ERROR_ON_ENTRY
	case AL_ALC_ERROR_ON_ENTRY: return StrCRef("ALC_ERROR_ON_ENTRY");
#endif
#if defined AL_UNSUPPORTED_FILE_TYPE
	case AL_UNSUPPORTED_FILE_TYPE: return StrCRef("UNSUPPORTED_FILE_TYPE");
#endif
#if defined AL_UNSUPPORTED_FILE_SUBTYPE
	case AL_UNSUPPORTED_FILE_SUBTYPE: return StrCRef("UNSUPPORTED_FILE_SUBTYPE");
#endif
#if defined AL_CORRUPT_OR_TRUNCATED_DATA
	case AL_CORRUPT_OR_TRUNCATED_DATA: return StrCRef("CORRUPT_OR_TRUNCATED_DATA");
#endif
	default:;
}
OALPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums

