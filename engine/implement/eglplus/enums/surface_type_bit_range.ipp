//  File implement/eglplus/enums/surface_type_bit_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/surface_type_bit.txt'
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
	SurfaceTypeBit
> ValueRange_(SurfaceTypeBit*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_SURFACETYPEBIT)
#define EGLPLUS_IMPL_EVR_SURFACETYPEBIT
{
static const EGLenum _values[] = {
#if defined EGL_WINDOW_BIT
EGL_WINDOW_BIT,
#endif
#if defined EGL_PIXMAP_BIT
EGL_PIXMAP_BIT,
#endif
#if defined EGL_PBUFFER_BIT
EGL_PBUFFER_BIT,
#endif
#if defined EGL_MULTISAMPLE_RESOLVE_BOX_BIT
EGL_MULTISAMPLE_RESOLVE_BOX_BIT,
#endif
#if defined EGL_SWAP_BEHAVIOR_PRESERVED_BIT
EGL_SWAP_BEHAVIOR_PRESERVED_BIT,
#endif
#if defined EGL_VG_COLORSPACE_LINEAR_BIT
EGL_VG_COLORSPACE_LINEAR_BIT,
#endif
#if defined EGL_VG_ALPHA_FORMAT_PRE_BIT
EGL_VG_ALPHA_FORMAT_PRE_BIT,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	SurfaceTypeBit
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

