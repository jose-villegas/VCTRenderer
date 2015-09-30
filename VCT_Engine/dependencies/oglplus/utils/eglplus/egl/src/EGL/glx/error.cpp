/**
 *  .file EGL/glx/error.cpp
 *  .brief EGL/GLX Error handling implementation
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "error.hpp"
#include "threads.hpp"


//------------------------------------------------------------------------------
void eglplus_egl_SetErrorCode(EGLint code)
{
	eglplus_egl_ErrorCodeRef() = code;
}
//------------------------------------------------------------------------------
// EGL API
//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------
// eglGetError
//------------------------------------------------------------------------------
EGLAPI EGLint EGLAPIENTRY eglGetError(void)
{
	EGLint ec_ref = eglplus_egl_ErrorCodeRef();
	EGLint old_ec = ec_ref;
	ec_ref = EGL_SUCCESS;
	return old_ec;
}
//------------------------------------------------------------------------------

} // extern "C"
