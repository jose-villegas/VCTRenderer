//  File implement/eglplus/enums/context_flag_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/context_flag.txt'
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

#if defined EGL_CONTEXT_OPENGL_DEBUG_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGLDebug
#  pragma push_macro("OpenGLDebug")
#  undef OpenGLDebug
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLDebug, EGL_CONTEXT_OPENGL_DEBUG_BIT)
#  pragma pop_macro("OpenGLDebug")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLDebug, EGL_CONTEXT_OPENGL_DEBUG_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGLForwardCompatible
#  pragma push_macro("OpenGLForwardCompatible")
#  undef OpenGLForwardCompatible
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLForwardCompatible, EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT)
#  pragma pop_macro("OpenGLForwardCompatible")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLForwardCompatible, EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGLRobustAccess
#  pragma push_macro("OpenGLRobustAccess")
#  undef OpenGLRobustAccess
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLRobustAccess, EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT)
#  pragma pop_macro("OpenGLRobustAccess")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLRobustAccess, EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

