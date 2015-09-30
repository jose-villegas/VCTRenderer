//  File implement/eglplus/enums/multisample_resolve_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/multisample_resolve.txt'
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
	MultisampleResolve
> ValueRange_(MultisampleResolve*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_MULTISAMPLERESOLVE)
#define EGLPLUS_IMPL_EVR_MULTISAMPLERESOLVE
{
static const EGLenum _values[] = {
#if defined EGL_MULTISAMPLE_RESOLVE_DEFAULT
EGL_MULTISAMPLE_RESOLVE_DEFAULT,
#endif
#if defined EGL_MULTISAMPLE_RESOLVE_BOX
EGL_MULTISAMPLE_RESOLVE_BOX,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	MultisampleResolve
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

