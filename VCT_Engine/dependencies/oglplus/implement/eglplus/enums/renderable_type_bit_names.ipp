//  File implement/eglplus/enums/renderable_type_bit_names.ipp
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
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	RenderableTypeBit*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_RENDERABLETYPEBIT)
#define EGLPLUS_IMPL_EVN_RENDERABLETYPEBIT
{
switch(value)
{
#if defined EGL_OPENGL_BIT
	case EGL_OPENGL_BIT: return StrCRef("OPENGL_BIT");
#endif
#if defined EGL_OPENGL_ES_BIT
	case EGL_OPENGL_ES_BIT: return StrCRef("OPENGL_ES_BIT");
#endif
#if defined EGL_OPENGL_ES2_BIT
	case EGL_OPENGL_ES2_BIT: return StrCRef("OPENGL_ES2_BIT");
#endif
#if defined EGL_OPENGL_ES3_BIT
	case EGL_OPENGL_ES3_BIT: return StrCRef("OPENGL_ES3_BIT");
#endif
#if defined EGL_OPENVG_BIT
	case EGL_OPENVG_BIT: return StrCRef("OPENVG_BIT");
#endif
	default:;
}
EGLPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums

