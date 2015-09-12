//  File implement/eglplus/enums/color_buffer_type_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/color_buffer_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	ColorBufferType*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_COLORBUFFERTYPE)
#define EGLPLUS_IMPL_EVN_COLORBUFFERTYPE
{
switch(value)
{
#if defined EGL_RGB_BUFFER
	case EGL_RGB_BUFFER: return StrCRef("RGB_BUFFER");
#endif
#if defined EGL_LUMINANCE_BUFFER
	case EGL_LUMINANCE_BUFFER: return StrCRef("LUMINANCE_BUFFER");
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

