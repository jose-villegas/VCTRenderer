//  File implement/eglplus/enums/render_buffer_def.ipp
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
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

#if defined EGL_SINGLE_BUFFER
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined SingleBuffer
#  pragma push_macro("SingleBuffer")
#  undef SingleBuffer
   EGLPLUS_ENUM_CLASS_VALUE(SingleBuffer, EGL_SINGLE_BUFFER)
#  pragma pop_macro("SingleBuffer")
# else
   EGLPLUS_ENUM_CLASS_VALUE(SingleBuffer, EGL_SINGLE_BUFFER)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_BACK_BUFFER
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined BackBuffer
#  pragma push_macro("BackBuffer")
#  undef BackBuffer
   EGLPLUS_ENUM_CLASS_VALUE(BackBuffer, EGL_BACK_BUFFER)
#  pragma pop_macro("BackBuffer")
# else
   EGLPLUS_ENUM_CLASS_VALUE(BackBuffer, EGL_BACK_BUFFER)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

