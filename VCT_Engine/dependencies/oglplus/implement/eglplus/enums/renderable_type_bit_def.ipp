//  File implement/eglplus/enums/renderable_type_bit_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/renderable_type_bit.txt'
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

#if defined EGL_OPENGL_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGL
#  pragma push_macro("OpenGL")
#  undef OpenGL
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL, EGL_OPENGL_BIT)
#  pragma pop_macro("OpenGL")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL, EGL_OPENGL_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_OPENGL_ES_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGL_ES
#  pragma push_macro("OpenGL_ES")
#  undef OpenGL_ES
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES, EGL_OPENGL_ES_BIT)
#  pragma pop_macro("OpenGL_ES")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES, EGL_OPENGL_ES_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_OPENGL_ES2_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGL_ES2
#  pragma push_macro("OpenGL_ES2")
#  undef OpenGL_ES2
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES2, EGL_OPENGL_ES2_BIT)
#  pragma pop_macro("OpenGL_ES2")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES2, EGL_OPENGL_ES2_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_OPENGL_ES3_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGL_ES3
#  pragma push_macro("OpenGL_ES3")
#  undef OpenGL_ES3
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES3, EGL_OPENGL_ES3_BIT)
#  pragma pop_macro("OpenGL_ES3")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES3, EGL_OPENGL_ES3_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_OPENVG_BIT
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenVG
#  pragma push_macro("OpenVG")
#  undef OpenVG
   EGLPLUS_ENUM_CLASS_VALUE(OpenVG, EGL_OPENVG_BIT)
#  pragma pop_macro("OpenVG")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenVG, EGL_OPENVG_BIT)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

