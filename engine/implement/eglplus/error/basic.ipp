/**
 *  @file eglplus/error/basic.ipp
 *  @brief Implementation of Error / exceptions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace eglplus {

EGLPLUS_LIB_FUNC
const char* Error::Message(EGLenum code)
{
	switch(code)
	{
		case EGL_NOT_INITIALIZED:
			return	"EGL is not initialized or could "
				"not be initialized";
		case EGL_BAD_ACCESS:
			return	"EGL cannot access a requested resource";
		case EGL_BAD_ALLOC:
			return	"EGL failed to allocate resources for "
				"the requested operation.";
		case EGL_BAD_ATTRIBUTE:
			return	"An unrecognized attribute or attribute "
				"value was passed in an attribute list";
		case EGL_BAD_CONTEXT:
			return	"Argument does not name a valid EGLContext";
		case EGL_BAD_CONFIG:
			return "Argument does not name a valid EGLConfig";
		case EGL_BAD_CURRENT_SURFACE:
			return	"The current surface of the calling thread is "
				"a window, pbuffer, or pixmap that is no longer "
				"valid";
		case EGL_BAD_DISPLAY:
			return	"Argument does not name a valid EGLDisplay";
		case EGL_BAD_SURFACE:
			return	"Argument does not name a valid surface (window, "
			"pbuffer, or pixmap) configured for rendering";
		case EGL_BAD_MATCH:
			return	"Arguments are inconsistent";
		case EGL_BAD_PARAMETER:
			return	"One or more argument values are invalid";
		case EGL_BAD_NATIVE_PIXMAP:
			return	"Argument does not refer to a valid native "
				"pixmap";
		case EGL_BAD_NATIVE_WINDOW:
			return	"Argument does not refer to a valid native "
				"window";
		case EGL_CONTEXT_LOST:
			return	"A power management event has occurred, the "
				"previously current EGL context is lost";
		default:;
	}
	return "Unknown EGL error";
}

EGLPLUS_LIB_FUNC
Error::Error(const char* message)
 : std::runtime_error(message)
 , _code(0)
#if !EGLPLUS_ERROR_NO_FILE
 , _file(nullptr)
#endif
#if !EGLPLUS_ERROR_NO_LINE
 , _line(0)
#endif
#if !EGLPLUS_ERROR_NO_EGL_FUNC
 , _eglfunc_name(nullptr)
#endif
#if !EGLPLUS_ERROR_NO_EGL_SYMBOL
 , _enumpar_name(nullptr)
 , _enumpar(0)
#endif
{ }

EGLPLUS_LIB_FUNC
const char* Error::SourceFile(void) const
{
#if !EGLPLUS_ERROR_NO_FILE
	return _file;
#else
	return nullptr;
#endif
}

EGLPLUS_LIB_FUNC
const char* Error::SourceFunc(void) const
{
#if !EGLPLUS_ERROR_NO_FUNC
	return _func;
#else
	return nullptr;
#endif
}

EGLPLUS_LIB_FUNC
unsigned Error::SourceLine(void) const
{
#if !EGLPLUS_ERROR_NO_LINE
	return _line;
#else
	return 0u;
#endif
}

EGLPLUS_LIB_FUNC
const char* Error::EGLFunc(void) const
{
#if !EGLPLUS_ERROR_NO_EGL_FUNC
	return _eglfunc_name;
#else
	return nullptr;
#endif
}

EGLPLUS_LIB_FUNC
EGLenum Error::EnumParam(void) const
{
#if !EGLPLUS_ERROR_NO_EGL_SYMBOL
	return _enumpar;
#else
	return 0;
#endif
}

EGLPLUS_LIB_FUNC
const char* Error::EnumParamName(void) const
{
#if !EGLPLUS_ERROR_NO_EGL_SYMBOL
	return _enumpar_name;
#else
	return nullptr;
#endif
}

} // namespace eglplus

