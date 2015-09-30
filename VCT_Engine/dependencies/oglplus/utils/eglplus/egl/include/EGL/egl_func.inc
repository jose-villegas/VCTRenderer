/**
 *  .file EGL/egl_func.inc
 *  .brief EGL function declarations
 *
 *  Copyright (c) 2007-2009 The Khronos Group Inc.
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and/or associated documentation files (the
 *  "Materials"), to deal in the Materials without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Materials, and to
 *  permit persons to whom the Materials are furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Materials.
 *
 *  THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

EGLAPI EGLint EGLAPIENTRY eglGetError(void);

EGLAPI EGLDisplay EGLAPIENTRY
eglGetDisplay(EGLNativeDisplayType display_id);

EGLAPI EGLBoolean EGLAPIENTRY
eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor);

EGLAPI EGLBoolean EGLAPIENTRY
eglTerminate(EGLDisplay dpy);

EGLAPI const char * EGLAPIENTRY
eglQueryString(EGLDisplay dpy, EGLint name);

EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigs(
	EGLDisplay dpy,
	EGLConfig *configs,
	EGLint config_size,
	EGLint *num_config
);

EGLAPI EGLBoolean EGLAPIENTRY
eglChooseConfig(
	EGLDisplay dpy,
	const EGLint *attrib_list,
	EGLConfig *configs,
	EGLint config_size,
	EGLint *num_config
);

EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigAttrib(
	EGLDisplay dpy,
	EGLConfig config,
	EGLint attribute,
	EGLint *value
);

EGLAPI EGLSurface EGLAPIENTRY
eglCreateWindowSurface(
	EGLDisplay dpy,
	EGLConfig config,
	EGLNativeWindowType win,
	const EGLint *attrib_list
);

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePbufferSurface(
	EGLDisplay dpy,
	EGLConfig config,
	const EGLint *attrib_list
);

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePixmapSurface(
	EGLDisplay dpy,
	EGLConfig config,
	EGLNativePixmapType pixmap,
	const EGLint *attrib_list
);

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroySurface(
	EGLDisplay dpy,
	EGLSurface surface
);

EGLAPI EGLBoolean EGLAPIENTRY
eglQuerySurface(
	EGLDisplay dpy,
	EGLSurface surface,
	EGLint attribute,
	EGLint *value
);

EGLAPI EGLBoolean EGLAPIENTRY
eglBindAPI(EGLenum api);

EGLAPI EGLenum EGLAPIENTRY
eglQueryAPI(void);


EGLAPI EGLBoolean EGLAPIENTRY
eglWaitClient(void);


EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseThread(void);


EGLAPI EGLSurface EGLAPIENTRY
eglCreatePbufferFromClientBuffer(
	EGLDisplay dpy,
	EGLenum buftype,
	EGLClientBuffer buffer,
	EGLConfig config,
	const EGLint *attrib_list
);

EGLAPI EGLBoolean EGLAPIENTRY
eglSurfaceAttrib(
	EGLDisplay dpy,
	EGLSurface surface,
	EGLint attribute,
	EGLint value
);

EGLAPI EGLBoolean EGLAPIENTRY
eglBindTexImage(
	EGLDisplay dpy,
	EGLSurface surface,
	EGLint buffer
);

EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseTexImage(
	EGLDisplay dpy,
	EGLSurface surface,
	EGLint buffer
);


EGLAPI EGLBoolean EGLAPIENTRY
eglSwapInterval(
	EGLDisplay dpy,
	EGLint interval
);


EGLAPI EGLContext EGLAPIENTRY
eglCreateContext(
	EGLDisplay dpy,
	EGLConfig config,
	EGLContext share_context,
	const EGLint *attrib_list
);

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyContext(EGLDisplay dpy, EGLContext ctx);

EGLAPI EGLBoolean EGLAPIENTRY
eglMakeCurrent(
	EGLDisplay dpy,
	EGLSurface draw,
	EGLSurface read,
	EGLContext ctx
);

EGLAPI EGLContext EGLAPIENTRY
eglGetCurrentContext(void);

EGLAPI EGLSurface EGLAPIENTRY
eglGetCurrentSurface(EGLint readdraw);

EGLAPI EGLDisplay EGLAPIENTRY
eglGetCurrentDisplay(void);


EGLAPI EGLBoolean EGLAPIENTRY
eglQueryContext(
	EGLDisplay dpy,
	EGLContext ctx,
	EGLint attribute,
	EGLint *value
);

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitGL(void);

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitNative(EGLint engine);

EGLAPI EGLBoolean EGLAPIENTRY
eglSwapBuffers(EGLDisplay dpy, EGLSurface surface);

EGLAPI EGLBoolean EGLAPIENTRY
eglCopyBuffers(
	EGLDisplay dpy,
	EGLSurface surface,
	EGLNativePixmapType target
);

typedef void (*__eglMustCastToProperFunctionPointerType)(void);

EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY
eglGetProcAddress(const char *procname);

