//  File implement/eglplus/enums/color_buffer_type_def.ipp
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
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

#if defined EGL_RGB_BUFFER
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined RGBBuffer
#  pragma push_macro("RGBBuffer")
#  undef RGBBuffer
   EGLPLUS_ENUM_CLASS_VALUE(RGBBuffer, EGL_RGB_BUFFER)
#  pragma pop_macro("RGBBuffer")
# else
   EGLPLUS_ENUM_CLASS_VALUE(RGBBuffer, EGL_RGB_BUFFER)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_LUMINANCE_BUFFER
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined LuminanceBuffer
#  pragma push_macro("LuminanceBuffer")
#  undef LuminanceBuffer
   EGLPLUS_ENUM_CLASS_VALUE(LuminanceBuffer, EGL_LUMINANCE_BUFFER)
#  pragma pop_macro("LuminanceBuffer")
# else
   EGLPLUS_ENUM_CLASS_VALUE(LuminanceBuffer, EGL_LUMINANCE_BUFFER)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

