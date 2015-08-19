//  File implement/oalplus/enums/context_attrib_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/context_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
OALPLUS_LIB_FUNC StrCRef ValueName_(
	ContextAttrib*,
	ALenum value
)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVN_CONTEXTATTRIB)
#define OALPLUS_IMPL_EVN_CONTEXTATTRIB
{
switch(value)
{
#if defined ALC_FREQUENCY
	case ALC_FREQUENCY: return StrCRef("FREQUENCY");
#endif
#if defined ALC_REFRESH
	case ALC_REFRESH: return StrCRef("REFRESH");
#endif
#if defined ALC_SYNC
	case ALC_SYNC: return StrCRef("SYNC");
#endif
#if defined ALC_MONO_SOURCES
	case ALC_MONO_SOURCES: return StrCRef("MONO_SOURCES");
#endif
#if defined ALC_STEREO_SOURCES
	case ALC_STEREO_SOURCES: return StrCRef("STEREO_SOURCES");
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

