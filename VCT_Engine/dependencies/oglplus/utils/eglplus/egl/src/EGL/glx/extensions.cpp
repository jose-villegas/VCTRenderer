/**
 *  .file EGL/glx/extension.cpp
 *  .brief EGL Extensions functionality implementation.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <EGL/egl.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cstring>

#include "ext/NV_native_query.hpp"

//------------------------------------------------------------------------------
// EGL API
//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------
// eglGetProcAddress
//------------------------------------------------------------------------------
EGLAPI void EGLAPIENTRY (*eglGetProcAddress(const char* procname))(void)
{
	typedef void(*fnptr)(void);

	// EGL 1.4
	if(std::strcmp(procname, "eglGetError") == 0)
		return fnptr(&eglGetError);
	if(std::strcmp(procname, "eglGetDisplay") == 0)
		return fnptr(&eglGetDisplay);
	if(std::strcmp(procname, "eglInitialize") == 0)
		return fnptr(&eglInitialize);
	if(std::strcmp(procname, "eglTerminate") == 0)
		return fnptr(&eglTerminate);
	if(std::strcmp(procname, "eglQueryString") == 0)
		return fnptr(&eglQueryString);
	if(std::strcmp(procname, "eglGetConfigs") == 0)
		return fnptr(&eglGetConfigs);
	if(std::strcmp(procname, "eglChooseConfig") == 0)
		return fnptr(&eglChooseConfig);
	if(std::strcmp(procname, "eglGetConfigAttrib") == 0)
		return fnptr(&eglGetConfigAttrib);
	if(std::strcmp(procname, "eglCreateWindowSurface") == 0)
		return fnptr(&eglCreateWindowSurface);
	if(std::strcmp(procname, "eglCreatePbufferSurface") == 0)
		return fnptr(&eglCreatePbufferSurface);
	if(std::strcmp(procname, "eglCreatePixmapSurface") == 0)
		return fnptr(&eglCreatePixmapSurface);
	if(std::strcmp(procname, "eglDestroySurface") == 0)
		return fnptr(&eglDestroySurface);
	if(std::strcmp(procname, "eglQuerySurface") == 0)
		return fnptr(&eglQuerySurface);
	if(std::strcmp(procname, "eglBindAPI") == 0)
		return fnptr(&eglBindAPI);
	if(std::strcmp(procname, "eglQueryAPI") == 0)
		return fnptr(&eglQueryAPI);
	if(std::strcmp(procname, "eglWaitClient") == 0)
		return fnptr(&eglWaitClient);
	if(std::strcmp(procname, "eglReleaseThread") == 0)
		return fnptr(&eglReleaseThread);
	if(std::strcmp(procname, "eglCreatePbufferFromClientBuffer") == 0)
		return fnptr(&eglCreatePbufferFromClientBuffer);
	if(std::strcmp(procname, "eglSurfaceAttrib") == 0)
		return fnptr(&eglSurfaceAttrib);
	if(std::strcmp(procname, "eglBindTexImage") == 0)
		return fnptr(&eglBindTexImage);
	if(std::strcmp(procname, "eglReleaseTexImage") == 0)
		return fnptr(&eglReleaseTexImage);
	if(std::strcmp(procname, "eglSwapInterval") == 0)
		return fnptr(&eglSwapInterval);
	if(std::strcmp(procname, "eglCreateContext") == 0)
		return fnptr(&eglCreateContext);
	if(std::strcmp(procname, "eglDestroyContext") == 0)
		return fnptr(&eglDestroyContext);
	if(std::strcmp(procname, "eglMakeCurrent") == 0)
		return fnptr(&eglMakeCurrent);
	if(std::strcmp(procname, "eglGetCurrentContext") == 0)
		return fnptr(&eglGetCurrentContext);
	if(std::strcmp(procname, "eglGetCurrentSurface") == 0)
		return fnptr(&eglGetCurrentSurface);
	if(std::strcmp(procname, "eglGetCurrentDisplay") == 0)
		return fnptr(&eglGetCurrentDisplay);
	if(std::strcmp(procname, "eglQueryContext") == 0)
		return fnptr(&eglQueryContext);
	if(std::strcmp(procname, "eglWaitGL") == 0)
		return fnptr(&eglWaitGL);
	if(std::strcmp(procname, "eglWaitNative") == 0)
		return fnptr(&eglWaitNative);
	if(std::strcmp(procname, "eglSwapBuffers") == 0)
		return fnptr(&eglSwapBuffers);
	if(std::strcmp(procname, "eglCopyBuffers") == 0)
		return fnptr(&eglCopyBuffers);

	// EGL_NV_native_query
	if(std::strcmp(procname, "eglQueryNativeDisplayNV") == 0)
		return fnptr(&eglQueryNativeDisplayNV);
	if(std::strcmp(procname, "eglQueryNativeWindowNV") == 0)
		return fnptr(&eglQueryNativeWindowNV);
	if(std::strcmp(procname, "eglQueryNativePixmapNV") == 0)
		return fnptr(&eglQueryNativePixmapNV);

	return ::glXGetProcAddress((GLubyte*)procname);
}
//------------------------------------------------------------------------------
} // extern "C"
