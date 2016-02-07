//  File implement/oalplus/enums/string_query_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/string_query.txt'
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
	StringQuery
> ValueRange_(StringQuery*)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVR_STRINGQUERY)
#define OALPLUS_IMPL_EVR_STRINGQUERY
{
static const ALenum _values[] = {
#if defined AL_VERSION
AL_VERSION,
#endif
#if defined AL_RENDERER
AL_RENDERER,
#endif
#if defined AL_VENDOR
AL_VENDOR,
#endif
#if defined AL_EXTENSIONS
AL_EXTENSIONS,
#endif
0
};
return aux::CastIterRange<
	const ALenum*,
	StringQuery
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

