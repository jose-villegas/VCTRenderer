//  File implement/eglplus/enums/context_attrib_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/context_attrib.txt'
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

#if defined EGL_CONTEXT_MAJOR_VERSION
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined MajorVersion
#  pragma push_macro("MajorVersion")
#  undef MajorVersion
   EGLPLUS_ENUM_CLASS_VALUE(MajorVersion, EGL_CONTEXT_MAJOR_VERSION)
#  pragma pop_macro("MajorVersion")
# else
   EGLPLUS_ENUM_CLASS_VALUE(MajorVersion, EGL_CONTEXT_MAJOR_VERSION)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_CONTEXT_MINOR_VERSION
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined MinorVersion
#  pragma push_macro("MinorVersion")
#  undef MinorVersion
   EGLPLUS_ENUM_CLASS_VALUE(MinorVersion, EGL_CONTEXT_MINOR_VERSION)
#  pragma pop_macro("MinorVersion")
# else
   EGLPLUS_ENUM_CLASS_VALUE(MinorVersion, EGL_CONTEXT_MINOR_VERSION)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_CONTEXT_FLAGS
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined Flags
#  pragma push_macro("Flags")
#  undef Flags
   EGLPLUS_ENUM_CLASS_VALUE(Flags, EGL_CONTEXT_FLAGS)
#  pragma pop_macro("Flags")
# else
   EGLPLUS_ENUM_CLASS_VALUE(Flags, EGL_CONTEXT_FLAGS)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_PROFILE_MASK
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGLProfileMask
#  pragma push_macro("OpenGLProfileMask")
#  undef OpenGLProfileMask
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLProfileMask, EGL_CONTEXT_OPENGL_PROFILE_MASK)
#  pragma pop_macro("OpenGLProfileMask")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLProfileMask, EGL_CONTEXT_OPENGL_PROFILE_MASK)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGLResetNotificationStrategy
#  pragma push_macro("OpenGLResetNotificationStrategy")
#  undef OpenGLResetNotificationStrategy
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLResetNotificationStrategy, EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY)
#  pragma pop_macro("OpenGLResetNotificationStrategy")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGLResetNotificationStrategy, EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

