//  File implement/eglplus/enums/swap_behavior_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/swap_behavior.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC aux::CastIterRange<
	const EGLenum*,
	SwapBehavior
> ValueRange_(SwapBehavior*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_SWAPBEHAVIOR)
#define EGLPLUS_IMPL_EVR_SWAPBEHAVIOR
{
static const EGLenum _values[] = {
#if defined EGL_BUFFER_PRESERVED
EGL_BUFFER_PRESERVED,
#endif
#if defined EGL_BUFFER_DESTROYED
EGL_BUFFER_DESTROYED,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	SwapBehavior
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

