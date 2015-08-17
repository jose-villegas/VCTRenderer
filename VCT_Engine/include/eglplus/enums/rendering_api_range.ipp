//  File implement/eglplus/enums/rendering_api_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/rendering_api.txt'
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
	RenderingAPI
> ValueRange_(RenderingAPI*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_RENDERINGAPI)
#define EGLPLUS_IMPL_EVR_RENDERINGAPI
{
static const EGLenum _values[] = {
#if defined EGL_OPENGL_API
EGL_OPENGL_API,
#endif
#if defined EGL_OPENGL_ES_API
EGL_OPENGL_ES_API,
#endif
#if defined EGL_OPENVG_API
EGL_OPENVG_API,
#endif
#if defined EGL_NONE
EGL_NONE,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	RenderingAPI
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

