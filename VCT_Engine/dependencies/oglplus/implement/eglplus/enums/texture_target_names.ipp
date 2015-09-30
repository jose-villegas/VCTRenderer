//  File implement/eglplus/enums/texture_target_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/texture_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	TextureTarget*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_TEXTURETARGET)
#define EGLPLUS_IMPL_EVN_TEXTURETARGET
{
switch(value)
{
#if defined EGL_NO_TEXTURE
	case EGL_NO_TEXTURE: return StrCRef("NO_TEXTURE");
#endif
#if defined EGL_TEXTURE_2D
	case EGL_TEXTURE_2D: return StrCRef("TEXTURE_2D");
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

