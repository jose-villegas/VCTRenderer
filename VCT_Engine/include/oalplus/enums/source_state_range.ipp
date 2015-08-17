//  File implement/oalplus/enums/source_state_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/source_state.txt'
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
	SourceState
> ValueRange_(SourceState*)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVR_SOURCESTATE)
#define OALPLUS_IMPL_EVR_SOURCESTATE
{
static const ALenum _values[] = {
#if defined AL_INITIAL
AL_INITIAL,
#endif
#if defined AL_PLAYING
AL_PLAYING,
#endif
#if defined AL_PAUSED
AL_PAUSED,
#endif
#if defined AL_STOPPED
AL_STOPPED,
#endif
0
};
return aux::CastIterRange<
	const ALenum*,
	SourceState
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

