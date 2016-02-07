//  File implement/oalplus/enums/alc_error_code_range.ipp
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
namespace enums {
OALPLUS_LIB_FUNC aux::CastIterRange<
	const ALenum*,
	ALCErrorCode
> ValueRange_(ALCErrorCode*)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVR_ALCERRORCODE)
#define OALPLUS_IMPL_EVR_ALCERRORCODE
{
static const ALenum _values[] = {
#if defined AL_NO_ERROR
AL_NO_ERROR,
#endif
#if defined AL_INVALID_DEVICE
AL_INVALID_DEVICE,
#endif
#if defined AL_INVALID_CONTEXT
AL_INVALID_CONTEXT,
#endif
#if defined AL_INVALID_ENUM
AL_INVALID_ENUM,
#endif
#if defined AL_INVALID_VALUE
AL_INVALID_VALUE,
#endif
#if defined AL_OUT_OF_MEMORY
AL_OUT_OF_MEMORY,
#endif
0
};
return aux::CastIterRange<
	const ALenum*,
	ALCErrorCode
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

