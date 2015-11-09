//  File implement/eglplus/enums/renderable_type_bit_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/renderable_type_bit.txt'
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
	RenderableTypeBit
> ValueRange_(RenderableTypeBit*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_RENDERABLETYPEBIT)
#define EGLPLUS_IMPL_EVR_RENDERABLETYPEBIT
{
static const EGLenum _values[] = {
#if defined EGL_OPENGL_BIT
EGL_OPENGL_BIT,
#endif
#if defined EGL_OPENGL_ES_BIT
EGL_OPENGL_ES_BIT,
#endif
#if defined EGL_OPENGL_ES2_BIT
EGL_OPENGL_ES2_BIT,
#endif
#if defined EGL_OPENGL_ES3_BIT
EGL_OPENGL_ES3_BIT,
#endif
#if defined EGL_OPENVG_BIT
EGL_OPENVG_BIT,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	RenderableTypeBit
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

