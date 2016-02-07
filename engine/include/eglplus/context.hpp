/**
 *  @file eglplus/context.hpp
 *  @brief Declares and implements wrapper for EGLContext
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_CONTEXT_1305291005_HPP
#define EGLPLUS_CONTEXT_1305291005_HPP

#include <eglplus/eglfunc.hpp>
#include <eglplus/error/basic.hpp>
#include <eglplus/boolean.hpp>
#include <eglplus/display.hpp>
#include <eglplus/configs.hpp>
#include <eglplus/surface.hpp>
#include <eglplus/attrib_list.hpp>
#include <eglplus/context_attrib.hpp>
#include <eglplus/context_flag.hpp>
#include <eglplus/opengl_profile_bit.hpp>
#include <eglplus/opengl_rns.hpp>

namespace eglplus {


struct ContextValueTypeToContextAttrib
{
#ifdef EGL_CONTEXT_FLAGS
	static ContextFlag
	ValueType(std::integral_constant<int, 0>);
	ContextAttrib operator()(ContextFlag) const
	{
		return ContextAttrib::Flags;
	}
#endif

#ifdef EGL_CONTEXT_OPENGL_PROFILE_MASK
	static OpenGLProfileBit
	ValueType(std::integral_constant<int, 1>);
	ContextAttrib operator()(OpenGLProfileBit) const
	{
		return ContextAttrib::OpenGLProfileMask;
	}
#endif

#ifdef EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY
	static OpenGLResetNotificationStrategy
	ValueType(std::integral_constant<int, 2>);
	ContextAttrib operator()(OpenGLResetNotificationStrategy) const
	{
		return ContextAttrib::OpenGLResetNotificationStrategy;
	}
#endif

	static std::integral_constant<int, 2> MaxValueType(void);
};


/// Attribute list for context attributes
typedef AttributeList<
	ContextAttrib,
	ContextValueTypeToContextAttrib,
	AttributeListTraits
> ContextAttribs;

/// Finished list of context attribute values
typedef FinishedAttributeList<
	ContextAttrib,
	AttributeListTraits
> FinishedContextAttribs;

class Context;
::EGLContext GetEGLHandle(const Context&)
OGLPLUS_NOEXCEPT(true);

/// Wrapper around EGLContext
class Context
{
private:
	Display _display;
	::EGLContext _handle;

	friend ::EGLContext GetEGLHandle(const Context&)
	OGLPLUS_NOEXCEPT(true);

	Context(const Context&);

	Context(
		Display display,
		::EGLContext handle
	): _display(display)
	 , _handle(handle)
	{ }

	static ::EGLContext _init(
		const Display& display,
		const Config& config,
		::EGLContext share_context,
		const EGLint* attribs
	)
	{
		::EGLContext result = EGLPLUS_EGLFUNC(CreateContext)(
			GetEGLHandle(display),
			GetEGLHandle(config),
			share_context,
			attribs
		);
		EGLPLUS_VERIFY_SIMPLE(CreateContext);
		return result;
	}
public:
	/// Contexts are move constructible
	Context(Context&& tmp)
	 : _display(tmp._display)
	 , _handle(tmp._handle)
	{
		tmp._handle = EGL_NO_CONTEXT;
	}

	/// Construct a non-sharing context without any attributes
	/**
	 *  @eglsymbols
	 *  @eglfunref{CreateContext}
	 */
	Context(
		const Display& display,
		const Config& config
	): _display(display)
	 , _handle(_init(display, config, EGL_NO_CONTEXT, nullptr))
	{ }

	/// Construct a sharing context without any attributes
	/**
	 *  @eglsymbols
	 *  @eglfunref{CreateContext}
	 */
	Context(
		const Display& display,
		const Config& config,
		const Context& shared_context
	): _display(display)
	 , _handle(_init(display, config, shared_context._handle, nullptr))
	{ }

	/// Construct a non-sharing context with attributes
	/**
	 *  @eglsymbols
	 *  @eglfunref{CreateContext}
	 */
	Context(
		const Display& display,
		const Config& config,
		const FinishedContextAttribs& attribs
	): _display(display)
	 , _handle(_init(display, config, EGL_NO_CONTEXT, attribs.Get()))
	{ }

	/// Construct a sharing context without any attributes
	/**
	 *  @eglsymbols
	 *  @eglfunref{CreateContext}
	 */
	Context(
		const Display& display,
		const Config& config,
		const Context& shared_context,
		const FinishedContextAttribs& attribs
	): _display(display)
	 , _handle(_init(display, config, shared_context._handle, attribs.Get()))
	{ }

	/// Destroys the wrapped context
	/**
	 *  @eglsymbols
	 *  @eglfunref{DestroyContext}
	 */
	~Context(void)
	{
		if(_handle != EGL_NO_CONTEXT)
		{
			EGLPLUS_EGLFUNC(DestroyContext)(
				GetEGLHandle(_display),
				_handle
			);
			EGLPLUS_VERIFY_SIMPLE(DestroyContext);
		}
	}

	/// Makes the context current
	/**
	 *  @eglsymbols
	 *  @eglfunref{MakeCurrent}
	 */
	Boolean MakeCurrent(
		const Surface& draw_surface,
		const Surface& read_surface
	)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(MakeCurrent)(
				GetEGLHandle(_display),
				GetEGLHandle(draw_surface),
				GetEGLHandle(read_surface),
				_handle
			), std::nothrow
		);
		EGLPLUS_CHECK_SIMPLE(MakeCurrent);
		return result;
	}

	/// Makes the context current
	/**
	 *  @eglsymbols
	 *  @eglfunref{MakeCurrent}
	 */
	Boolean MakeCurrent(const Surface& surface)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(MakeCurrent)(
				GetEGLHandle(_display),
				GetEGLHandle(surface),
				GetEGLHandle(surface),
				_handle
			), std::nothrow
		);
		EGLPLUS_CHECK_SIMPLE(MakeCurrent);
		return result;
	}

	/// Makes the context current without surfaces
	/**
	 *  @note This function works only if EGL implements
	 *  the EGL_KHR_surfaceless_context extension.
	 *
	 *  @eglsymbols
	 *  @eglfunref{MakeCurrent}
	 */
	Boolean MakeCurrent(void)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(MakeCurrent)(
				GetEGLHandle(_display),
				EGL_NO_SURFACE,
				EGL_NO_SURFACE,
				_handle
			), std::nothrow
		);
		EGLPLUS_CHECK_SIMPLE(MakeCurrent);
		return result;
	}

	/// Releases the current context without assigning a new one
	/**
	 *  @eglsymbols
	 *  @eglfunref{MakeCurrent}
	 */
	Boolean Release(void)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(MakeCurrent)(
				GetEGLHandle(_display),
				EGL_NO_SURFACE,
				EGL_NO_SURFACE,
				EGL_NO_CONTEXT
			), std::nothrow
		);
		EGLPLUS_CHECK_SIMPLE(MakeCurrent);
		return result;
	}

	/// Queries a context attribute
	/**
	 *  @eglsymbols
	 *  @eglfunref{QueryContext}
	 */
	Boolean Query(ContextAttrib attrib, EGLint& value) const
	{
		Boolean result(
			EGLPLUS_EGLFUNC(QueryContext)(
				GetEGLHandle(_display),
				_handle,
				EGLint(EGLenum(attrib)),
				&value
			), std::nothrow
		);
		EGLPLUS_CHECK_SIMPLE(QueryContext);
		return result;
	}

	/// Returns the context framebuffer config id
	/**
	 *  @eglsymbols
	 *  @eglfunref{QueryContext}
	 */
	EGLint ConfigId(void) const
	{
		EGLint result = 0;
		EGLPLUS_EGLFUNC(QueryContext)(
			GetEGLHandle(_display),
			_handle,
			EGLint(EGL_CONFIG_ID),
			&result
		);
		EGLPLUS_CHECK_SIMPLE(QueryContext);
		return result;
	}

	/// Wait for client API commands to complete
	/**
	 *  @eglsymbols
	 *  @eglfunref{WaitClient}
	 */
	Boolean WaitClient(void) const
	{
		Boolean result(
			EGLPLUS_EGLFUNC(WaitClient)(),
			std::nothrow
		);
		EGLPLUS_VERIFY_SIMPLE(WaitClient);
		return result;
	}

	/// Wait for GL API commands to complete
	/**
	 *  @eglsymbols
	 *  @eglfunref{WaitGL}
	 */
	Boolean WaitGL(void) const
	{
		Boolean result(
			EGLPLUS_EGLFUNC(WaitGL)(),
			std::nothrow
		);
		EGLPLUS_VERIFY_SIMPLE(WaitGL);
		return result;
	}

	/// Wait for native API commands to complete
	/**
	 *  @eglsymbols
	 *  @eglfunref{WaitNative}
	 */
	Boolean WaitNative(EGLint engine) const
	{
		Boolean result(
			EGLPLUS_EGLFUNC(WaitNative)(engine),
			std::nothrow
		);
		EGLPLUS_VERIFY_SIMPLE(WaitNative);
		return result;
	}
};

inline
::EGLContext GetEGLHandle(const Context& context)
OGLPLUS_NOEXCEPT(true)
{
	return context._handle;
}

} // namespace eglplus

#endif // include guard
