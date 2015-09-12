//  File implement/eglplus/enums/swap_behavior_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/swap_behavior.txt'
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

#if defined EGL_BUFFER_PRESERVED
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined BufferPreserved
#  pragma push_macro("BufferPreserved")
#  undef BufferPreserved
   EGLPLUS_ENUM_CLASS_VALUE(BufferPreserved, EGL_BUFFER_PRESERVED)
#  pragma pop_macro("BufferPreserved")
# else
   EGLPLUS_ENUM_CLASS_VALUE(BufferPreserved, EGL_BUFFER_PRESERVED)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_BUFFER_DESTROYED
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined BufferDestroyed
#  pragma push_macro("BufferDestroyed")
#  undef BufferDestroyed
   EGLPLUS_ENUM_CLASS_VALUE(BufferDestroyed, EGL_BUFFER_DESTROYED)
#  pragma pop_macro("BufferDestroyed")
# else
   EGLPLUS_ENUM_CLASS_VALUE(BufferDestroyed, EGL_BUFFER_DESTROYED)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

