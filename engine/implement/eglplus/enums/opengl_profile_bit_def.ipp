//  File implement/eglplus/enums/opengl_profile_bit_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/opengl_profile_bit.txt'
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

#if defined EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined Core
#  pragma push_macro("Core")
#  undef Core
   EGLPLUS_ENUM_CLASS_VALUE(Core, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT)
#  pragma pop_macro("Core")
# else
   EGLPLUS_ENUM_CLASS_VALUE(Core, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined Compatibility
#  pragma push_macro("Compatibility")
#  undef Compatibility
   EGLPLUS_ENUM_CLASS_VALUE(Compatibility, EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT)
#  pragma pop_macro("Compatibility")
# else
   EGLPLUS_ENUM_CLASS_VALUE(Compatibility, EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

