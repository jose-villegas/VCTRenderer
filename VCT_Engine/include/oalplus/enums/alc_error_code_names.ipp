//  File implement/oalplus/enums/alc_error_code_names.ipp
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
OALPLUS_LIB_FUNC StrCRef ValueName_(
	ALCErrorCode*,
	ALenum value
)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVN_ALCERRORCODE)
#define OALPLUS_IMPL_EVN_ALCERRORCODE
{
switch(value)
{
#if defined AL_NO_ERROR
	case AL_NO_ERROR: return StrCRef("NO_ERROR");
#endif
#if defined AL_INVALID_DEVICE
	case AL_INVALID_DEVICE: return StrCRef("INVALID_DEVICE");
#endif
#if defined AL_INVALID_CONTEXT
	case AL_INVALID_CONTEXT: return StrCRef("INVALID_CONTEXT");
#endif
#if defined AL_INVALID_ENUM
	case AL_INVALID_ENUM: return StrCRef("INVALID_ENUM");
#endif
#if defined AL_INVALID_VALUE
	case AL_INVALID_VALUE: return StrCRef("INVALID_VALUE");
#endif
#if defined AL_OUT_OF_MEMORY
	case AL_OUT_OF_MEMORY: return StrCRef("OUT_OF_MEMORY");
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

