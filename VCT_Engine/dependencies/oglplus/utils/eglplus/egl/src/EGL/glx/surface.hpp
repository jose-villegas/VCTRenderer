/**
 *  .file EGL/surface.hpp
 *  .brief EGL/GLX Surface declarations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_EGL_GLX_SURFACE_1306251555_HPP
#define OGLPLUS_EGL_GLX_SURFACE_1306251555_HPP

#include <X11/Xlib.h>
#include <GL/glx.h>

struct eglplus_egl_glx_SurfaceImpl
{
	int _glx_drawable_type;
	::GLXDrawable _glx_drawable;
	void (*_do_cleanup)( ::Display*, ::GLXDrawable);

	eglplus_egl_glx_SurfaceImpl(int, ::GLXDrawable);
	eglplus_egl_glx_SurfaceImpl(
		int,
		::GLXDrawable,
		void (*)( ::Display*, ::GLXDrawable)
	);
	~eglplus_egl_glx_SurfaceImpl(void);

	void _cleanup( ::Display*);
};

extern bool eglplus_egl_valid_surface(EGLSurface);

#endif // include guard
