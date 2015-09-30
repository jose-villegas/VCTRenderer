/**
 *  .file EGL/api.cpp
 *  .brief EGL current rendering API implementation
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <EGL/egl.h>

#include "api.hpp"

EGLenum eglplus_egl_CurrentAPI = EGL_NONE;

extern "C" {

//------------------------------------------------------------------------------
// eglBindAPI
//------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY
eglBindAPI(EGLenum api)
{
	switch(api)
	{
		case EGL_NONE:
		case EGL_OPENGL_API:
			eglplus_egl_CurrentAPI = api;
			return EGL_TRUE;
		default:;
	}
	return EGL_FALSE;
}
//------------------------------------------------------------------------------
// eglQueryAPI
//------------------------------------------------------------------------------
EGLAPI EGLenum EGLAPIENTRY
eglQueryAPI(void)
{
	return eglplus_egl_CurrentAPI;
}
//------------------------------------------------------------------------------

} // extern "C"
