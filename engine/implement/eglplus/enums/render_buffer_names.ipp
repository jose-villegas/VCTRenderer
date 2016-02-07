//  File implement/eglplus/enums/render_buffer_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/render_buffer.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	RenderBuffer*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_RENDERBUFFER)
#define EGLPLUS_IMPL_EVN_RENDERBUFFER
{
switch(value)
{
#if defined EGL_SINGLE_BUFFER
	case EGL_SINGLE_BUFFER: return StrCRef("SINGLE_BUFFER");
#endif
#if defined EGL_BACK_BUFFER
	case EGL_BACK_BUFFER: return StrCRef("BACK_BUFFER");
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

