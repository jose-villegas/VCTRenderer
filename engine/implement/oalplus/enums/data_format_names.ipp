//  File implement/oalplus/enums/data_format_names.ipp
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
namespace enums {
OALPLUS_LIB_FUNC StrCRef ValueName_(
	DataFormat*,
	ALenum value
)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVN_DATAFORMAT)
#define OALPLUS_IMPL_EVN_DATAFORMAT
{
switch(value)
{
#if defined AL_FORMAT_MONO8
	case AL_FORMAT_MONO8: return StrCRef("FORMAT_MONO8");
#endif
#if defined AL_FORMAT_MONO16
	case AL_FORMAT_MONO16: return StrCRef("FORMAT_MONO16");
#endif
#if defined AL_FORMAT_STEREO8
	case AL_FORMAT_STEREO8: return StrCRef("FORMAT_STEREO8");
#endif
#if defined AL_FORMAT_STEREO16
	case AL_FORMAT_STEREO16: return StrCRef("FORMAT_STEREO16");
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

