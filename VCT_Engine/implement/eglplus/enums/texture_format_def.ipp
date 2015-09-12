//  File implement/eglplus/enums/texture_format_def.ipp
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
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

#if defined EGL_TEXTURE_RGB
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined RGB
#  pragma push_macro("RGB")
#  undef RGB
   EGLPLUS_ENUM_CLASS_VALUE(RGB, EGL_TEXTURE_RGB)
#  pragma pop_macro("RGB")
# else
   EGLPLUS_ENUM_CLASS_VALUE(RGB, EGL_TEXTURE_RGB)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_TEXTURE_RGBA
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined RGBA
#  pragma push_macro("RGBA")
#  undef RGBA
   EGLPLUS_ENUM_CLASS_VALUE(RGBA, EGL_TEXTURE_RGBA)
#  pragma pop_macro("RGBA")
# else
   EGLPLUS_ENUM_CLASS_VALUE(RGBA, EGL_TEXTURE_RGBA)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_NO_TEXTURE
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined NoTexture
#  pragma push_macro("NoTexture")
#  undef NoTexture
   EGLPLUS_ENUM_CLASS_VALUE(NoTexture, EGL_NO_TEXTURE)
#  pragma pop_macro("NoTexture")
# else
   EGLPLUS_ENUM_CLASS_VALUE(NoTexture, EGL_NO_TEXTURE)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

