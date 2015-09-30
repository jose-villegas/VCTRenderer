/**
 *  .file EGL/glx/threads.cpp
 *  .brief EGL Threads-related functionality implementation.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "error.hpp"
#include "threads.hpp"

#include <pthread.h>

#include <cassert>

//------------------------------------------------------------------------------
// eglplus_egl_TLData
//------------------------------------------------------------------------------
struct eglplus_egl_TLData
{
	EGLint error_code;
	eglplus_egl_glx_Current current;

	eglplus_egl_TLData(void)
	 : error_code(EGL_SUCCESS)
	{ }
};
//------------------------------------------------------------------------------
static ::pthread_once_t eglplus_egl_once_TLData = PTHREAD_ONCE_INIT;
static ::pthread_key_t eglplus_egl_key_TLData;
//------------------------------------------------------------------------------
static void eglplus_egl_destroy_TLData(void* ptr)
{
	delete static_cast<eglplus_egl_TLData*>(ptr);
}
//------------------------------------------------------------------------------
static void eglplus_egl_create_key_TLData(void)
{
	::pthread_key_create(
		&eglplus_egl_key_TLData,
		&eglplus_egl_destroy_TLData
	);
}
//------------------------------------------------------------------------------
static eglplus_egl_TLData& eglplus_egl_TLDataRef(void)
{
	::pthread_once(
		&eglplus_egl_once_TLData,
		&eglplus_egl_create_key_TLData
	);
	void* tlptr = ::pthread_getspecific(eglplus_egl_key_TLData);
	if(tlptr == nullptr)
	{
		tlptr = new eglplus_egl_TLData();
		::pthread_setspecific(
			eglplus_egl_key_TLData,
			tlptr
		);
	} 
	assert(tlptr);
	return *static_cast<eglplus_egl_TLData*>(tlptr);
}
//------------------------------------------------------------------------------
static void eglplus_egl_release_TLData(void)
{
	::pthread_once(
		&eglplus_egl_once_TLData,
		&eglplus_egl_create_key_TLData
	);
	void* tlptr = ::pthread_getspecific(eglplus_egl_key_TLData);
	if(tlptr != nullptr)
	{
		eglplus_egl_destroy_TLData(tlptr);
		tlptr = nullptr;
	} 
	assert(tlptr == nullptr);
}
//------------------------------------------------------------------------------
// ErrorCode
//------------------------------------------------------------------------------
EGLint& eglplus_egl_ErrorCodeRef(void)
{
	return eglplus_egl_TLDataRef().error_code;
}
//------------------------------------------------------------------------------
// Current
//------------------------------------------------------------------------------
eglplus_egl_glx_Current& eglplus_egl_CurrentRef(void)
{
	return eglplus_egl_TLDataRef().current;
}
//------------------------------------------------------------------------------
// EGL API
//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------
// eglReleaseThread
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseThread(void)
{
	try
	{
		eglplus_egl_release_TLData();
		return EGL_TRUE;
	}
	catch(...)
	{
		return EGL_FALSE;
	}
}
//------------------------------------------------------------------------------
} // extern "C"
