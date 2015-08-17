//  File implement/eglplus/enums/vg_colorspace_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/vg_colorspace.txt'
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

#if defined EGL_VG_COLORSPACE_sRGB
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined sRGB
#  pragma push_macro("sRGB")
#  undef sRGB
   EGLPLUS_ENUM_CLASS_VALUE(sRGB, EGL_VG_COLORSPACE_sRGB)
#  pragma pop_macro("sRGB")
# else
   EGLPLUS_ENUM_CLASS_VALUE(sRGB, EGL_VG_COLORSPACE_sRGB)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_VG_COLORSPACE_LINEAR
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined Linear
#  pragma push_macro("Linear")
#  undef Linear
   EGLPLUS_ENUM_CLASS_VALUE(Linear, EGL_VG_COLORSPACE_LINEAR)
#  pragma pop_macro("Linear")
# else
   EGLPLUS_ENUM_CLASS_VALUE(Linear, EGL_VG_COLORSPACE_LINEAR)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

