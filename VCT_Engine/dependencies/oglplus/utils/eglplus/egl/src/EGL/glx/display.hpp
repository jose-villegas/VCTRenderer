/**
 *  .file EGL/display.hpp
 *  .brief EGL/GLX Display declarations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_EGL_GLX_DISPLAY_1306251555_HPP
#define OGLPLUS_EGL_GLX_DISPLAY_1306251555_HPP

#include <X11/Xlib.h>

extern bool eglplus_egl_valid_display(EGLDisplay);

struct eglplus_egl_glx_DisplayImpl
{
	::Display * _x_display_id;
	::Display * _x_open_display;

	eglplus_egl_glx_DisplayImpl(EGLNativeDisplayType);

	bool initialized(void) const;

	bool _init(void);
	bool _cleanup(void);
};

#endif // include guard
