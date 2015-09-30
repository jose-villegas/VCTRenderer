/**
 *  .file EGL/ext/NV_native_query.hpp
 *  .brief EGL/GLX EGL_NV_native_query declarations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_EGL_GLX_EXT_NV_NATIVE_QUERY_1401082234_HPP
#define OGLPLUS_EGL_GLX_EXT_NV_NATIVE_QUERY_1401082234_HPP

#include <EGL/egl.h>

extern "C" {

EGLAPI EGLBoolean EGLAPIENTRY
eglQueryNativeDisplayNV(EGLDisplay, EGLNativeDisplayType*);

EGLAPI EGLBoolean EGLAPIENTRY
eglQueryNativeWindowNV(EGLDisplay, EGLSurface, EGLNativeWindowType*);

EGLAPI EGLBoolean EGLAPIENTRY
eglQueryNativePixmapNV(EGLDisplay, EGLSurface, EGLNativePixmapType*);

} // extern "C"

#endif // include guard
