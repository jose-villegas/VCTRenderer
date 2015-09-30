/**
 *  .file EGL/glx/egl_type.h
 *  .brief EGL/GLX header
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
struct eglplus_egl_glx_ConfigImpl
{
	void* _glx_fb_config;
};

struct eglplus_egl_glx_ContextImpl;
struct eglplus_egl_glx_DisplayImpl;
struct eglplus_egl_glx_SurfaceImpl;
struct eglplus_egl_glx_ClientBufferImpl;

typedef void* EGLNativeDisplayType;
typedef khronos_uint32_t EGLNativePixmapType;
typedef khronos_uint32_t EGLNativeWindowType;

typedef intptr_t EGLAttrib;
typedef void *EGLSync;
typedef khronos_utime_nanoseconds_t EGLTime;

typedef khronos_int32_t EGLint;
typedef unsigned int EGLBoolean;
typedef unsigned int EGLenum;

typedef struct eglplus_egl_glx_ConfigImpl EGLConfig;
typedef struct eglplus_egl_glx_ContextImpl *EGLContext;
typedef struct eglplus_egl_glx_DisplayImpl *EGLDisplay;
typedef struct eglplus_egl_glx_SurfaceImpl *EGLSurface;
typedef struct eglplus_egl_glx_ClientBufferImpl *EGLClientBuffer;
