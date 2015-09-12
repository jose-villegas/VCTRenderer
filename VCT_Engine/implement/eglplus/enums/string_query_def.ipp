//  File implement/eglplus/enums/string_query_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/string_query.txt'
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

#if defined EGL_CLIENT_APIS
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined ClientAPIs
#  pragma push_macro("ClientAPIs")
#  undef ClientAPIs
   EGLPLUS_ENUM_CLASS_VALUE(ClientAPIs, EGL_CLIENT_APIS)
#  pragma pop_macro("ClientAPIs")
# else
   EGLPLUS_ENUM_CLASS_VALUE(ClientAPIs, EGL_CLIENT_APIS)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_EXTENSIONS
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined Extensions
#  pragma push_macro("Extensions")
#  undef Extensions
   EGLPLUS_ENUM_CLASS_VALUE(Extensions, EGL_EXTENSIONS)
#  pragma pop_macro("Extensions")
# else
   EGLPLUS_ENUM_CLASS_VALUE(Extensions, EGL_EXTENSIONS)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_VENDOR
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined Vendor
#  pragma push_macro("Vendor")
#  undef Vendor
   EGLPLUS_ENUM_CLASS_VALUE(Vendor, EGL_VENDOR)
#  pragma pop_macro("Vendor")
# else
   EGLPLUS_ENUM_CLASS_VALUE(Vendor, EGL_VENDOR)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined EGL_VERSION
# ifdef EGLPLUS_LIST_NEEDS_COMMA
   EGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined Version
#  pragma push_macro("Version")
#  undef Version
   EGLPLUS_ENUM_CLASS_VALUE(Version, EGL_VERSION)
#  pragma pop_macro("Version")
# else
   EGLPLUS_ENUM_CLASS_VALUE(Version, EGL_VERSION)
# endif
# ifndef EGLPLUS_LIST_NEEDS_COMMA
#  define EGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef EGLPLUS_LIST_NEEDS_COMMA
# undef EGLPLUS_LIST_NEEDS_COMMA
#endif

