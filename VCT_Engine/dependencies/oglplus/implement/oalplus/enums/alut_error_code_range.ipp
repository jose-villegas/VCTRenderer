//  File implement/oalplus/enums/alut_error_code_range.ipp
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
OALPLUS_LIB_FUNC aux::CastIterRange<
	const ALenum*,
	ALUTErrorCode
> ValueRange_(ALUTErrorCode*)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVR_ALUTERRORCODE)
#define OALPLUS_IMPL_EVR_ALUTERRORCODE
{
static const ALenum _values[] = {
#if defined AL_OUT_OF_MEMORY
AL_OUT_OF_MEMORY,
#endif
#if defined AL_INVALID_OPERATION
AL_INVALID_OPERATION,
#endif
#if defined AL_NO_CURRENT_CONTEXT
AL_NO_CURRENT_CONTEXT,
#endif
#if defined AL_AL_ERROR_ON_ENTRY
AL_AL_ERROR_ON_ENTRY,
#endif
#if defined AL_ALC_ERROR_ON_ENTRY
AL_ALC_ERROR_ON_ENTRY,
#endif
#if defined AL_UNSUPPORTED_FILE_TYPE
AL_UNSUPPORTED_FILE_TYPE,
#endif
#if defined AL_UNSUPPORTED_FILE_SUBTYPE
AL_UNSUPPORTED_FILE_SUBTYPE,
#endif
#if defined AL_CORRUPT_OR_TRUNCATED_DATA
AL_CORRUPT_OR_TRUNCATED_DATA,
#endif
0
};
return aux::CastIterRange<
	const ALenum*,
	ALUTErrorCode
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

