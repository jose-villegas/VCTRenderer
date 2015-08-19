//  File implement/oalplus/enums/data_format_range.ipp
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
OALPLUS_LIB_FUNC aux::CastIterRange<
	const ALenum*,
	DataFormat
> ValueRange_(DataFormat*)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVR_DATAFORMAT)
#define OALPLUS_IMPL_EVR_DATAFORMAT
{
static const ALenum _values[] = {
#if defined AL_FORMAT_MONO8
AL_FORMAT_MONO8,
#endif
#if defined AL_FORMAT_MONO16
AL_FORMAT_MONO16,
#endif
#if defined AL_FORMAT_STEREO8
AL_FORMAT_STEREO8,
#endif
#if defined AL_FORMAT_STEREO16
AL_FORMAT_STEREO16,
#endif
0
};
return aux::CastIterRange<
	const ALenum*,
	DataFormat
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

