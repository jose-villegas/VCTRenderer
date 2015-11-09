//  File implement/eglplus/enums/rendering_api_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/rendering_api.txt'
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

#if defined EGL_OPENGL_API
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGL
#  pragma push_macro("OpenGL")
#  undef OpenGL
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL, EGL_OPENGL_API)
#  pragma pop_macro("OpenGL")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL, EGL_OPENGL_API)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_OPENGL_ES_API
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenGL_ES
#  pragma push_macro("OpenGL_ES")
#  undef OpenGL_ES
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES, EGL_OPENGL_ES_API)
#  pragma pop_macro("OpenGL_ES")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenGL_ES, EGL_OPENGL_ES_API)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_OPENVG_API
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined OpenVG
#  pragma push_macro("OpenVG")
#  undef OpenVG
   EGLPLUS_ENUM_CLASS_VALUE(OpenVG, EGL_OPENVG_API)
#  pragma pop_macro("OpenVG")
# else
   EGLPLUS_ENUM_CLASS_VALUE(OpenVG, EGL_OPENVG_API)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_NONE
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined None
#  pragma push_macro("None")
#  undef None
   EGLPLUS_ENUM_CLASS_VALUE(None, EGL_NONE)
#  pragma pop_macro("None")
# else
   EGLPLUS_ENUM_CLASS_VALUE(None, EGL_NONE)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

