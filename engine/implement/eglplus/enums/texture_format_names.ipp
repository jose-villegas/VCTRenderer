//  File implement/eglplus/enums/texture_format_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/texture_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	TextureFormat*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_TEXTUREFORMAT)
#define EGLPLUS_IMPL_EVN_TEXTUREFORMAT
{
switch(value)
{
#if defined EGL_TEXTURE_RGB
	case EGL_TEXTURE_RGB: return StrCRef("TEXTURE_RGB");
#endif
#if defined EGL_TEXTURE_RGBA
	case EGL_TEXTURE_RGBA: return StrCRef("TEXTURE_RGBA");
#endif
#if defined EGL_NO_TEXTURE
	case EGL_NO_TEXTURE: return StrCRef("NO_TEXTURE");
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

