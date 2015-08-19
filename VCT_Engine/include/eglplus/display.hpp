/**
 *  @file eglplus/display.hpp
 *  @brief Declares and implements wrapper for EGLDisplay
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_DISPLAY_1305291005_HPP
#define EGLPLUS_DISPLAY_1305291005_HPP

#include <eglplus/eglfunc.hpp>
#include <eglplus/boolean.hpp>
#include <eglplus/error/basic.hpp>

namespace eglplus {

class EGLInitializer;

class Display;
::EGLDisplay GetEGLHandle(const Display&)
OGLPLUS_NOEXCEPT(true);

/// Wrapper around EGLDisplay
class Display
{
private:
	::EGLDisplay _handle;

	friend ::EGLDisplay GetEGLHandle(const Display&)
	OGLPLUS_NOEXCEPT(true);
public:
	/// Opens the default display
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetDisplay}
	 *  @egldefref{DEFAULT_DISPLAY}
	 */
	Display(void)
	 : _handle(EGLPLUS_EGLFUNC(GetDisplay)(EGL_DEFAULT_DISPLAY))
	{
		EGLPLUS_CHECK_SIMPLE(GetDisplay);
	}

	/// Opens the display specified by @p display_id
	/**
	 *  @eglsymbols
	 *  @eglfunref{GetDisplay}
	 */
	Display(::EGLNativeDisplayType display_id)
	 : _handle(EGLPLUS_EGLFUNC(GetDisplay)(display_id))
	{
		EGLPLUS_CHECK_SIMPLE(GetDisplay);
	}

	/// Sets the SwapInterval for the current display
	/**
	 *  @eglsymbols
	 *  @eglfunref{SwapInterval}
	 */
	Boolean SwapInterval(EGLint interval)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(SwapInterval)(
				_handle,
				interval
			), std::nothrow
		);
		EGLPLUS_CHECK_SIMPLE(SwapInterval);
		return result;
	}

	/// Releases the current context without assigning a new one
	/**
	 *  @eglsymbols
	 *  @eglfunref{MakeCurrent}
	 */
	Boolean ReleaseContext(void)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(MakeCurrent)(
				_handle,
				EGL_NO_SURFACE,
				EGL_NO_SURFACE,
				EGL_NO_CONTEXT
			), std::nothrow
		);
		EGLPLUS_CHECK_SIMPLE(MakeCurrent);
		return result;
	}
};

inline
::EGLDisplay GetEGLHandle(const Display& display)
OGLPLUS_NOEXCEPT(true)
{
	return display._handle;
}

} // namespace eglplus

#endif // include guard
