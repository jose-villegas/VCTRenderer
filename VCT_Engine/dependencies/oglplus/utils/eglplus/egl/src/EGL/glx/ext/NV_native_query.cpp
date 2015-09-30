/**
 *  .file EGL/ext/NV_native_query.cpp
 *  .brief EGL/GLX EGL_NV_native_query implementation
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "NV_native_query.hpp"
#include "../error.hpp"
#include "../display.hpp"
#include "../surface.hpp"

extern "C" {
//------------------------------------------------------------------------------
// eglQueryNativeDisplayNV
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglQueryNativeDisplayNV(
	EGLDisplay display,
	EGLNativeDisplayType* glx_display
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_FALSE;
	}
	if(!display->_x_open_display)
	{
		eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	if(glx_display == nullptr)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}

	*glx_display = EGLNativeDisplayType(display->_x_open_display);
	return EGL_TRUE;
}
//------------------------------------------------------------------------------
// eglQueryNativeWindowNV
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglQueryNativeWindowNV(
	EGLDisplay display,
	EGLSurface surface,
	EGLNativeWindowType* glx_window
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_FALSE;
	}
	if(!display->_x_open_display)
	{
		eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	if((!surface) || (!surface->_glx_drawable))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}

	if(surface->_glx_drawable_type != GLX_WINDOW_BIT)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}

	if(glx_window == nullptr)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}

	*glx_window = EGLNativeWindowType(surface->_glx_drawable);
	return EGL_TRUE;
}
//------------------------------------------------------------------------------
// eglQueryNativePixmapNV
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglQueryNativePixmapNV(
	EGLDisplay display,
	EGLSurface surface,
	EGLNativePixmapType* glx_pixmap
)
{
	if(!eglplus_egl_valid_display(display))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_DISPLAY);
		return EGL_FALSE;
	}
	if(!display->_x_open_display)
	{
		eglplus_egl_SetErrorCode(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	if((!surface) || (!surface->_glx_drawable))
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}

	if(surface->_glx_drawable_type != GLX_PIXMAP_BIT)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_SURFACE);
		return EGL_FALSE;
	}

	if(glx_pixmap == nullptr)
	{
		eglplus_egl_SetErrorCode(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}

	*glx_pixmap = EGLNativePixmapType(surface->_glx_drawable);
	return EGL_TRUE;
}
//------------------------------------------------------------------------------

} // extern "C"
