/**
 *  @file eglplus/surface.hpp
 *  @brief Declares and implements wrapper for EGLContext
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_SURFACE_1305291005_HPP
#define EGLPLUS_SURFACE_1305291005_HPP

#include <eglplus/eglfunc.hpp>
#include <eglplus/error/basic.hpp>
#include <eglplus/display.hpp>
#include <eglplus/configs.hpp>
#include <eglplus/attrib_list.hpp>
#include <eglplus/surface_attrib.hpp>

#include <eglplus/render_buffer.hpp>
#include <eglplus/multisample_resolve.hpp>
#include <eglplus/swap_behavior.hpp>
#include <eglplus/texture_format.hpp>
#include <eglplus/texture_target.hpp>
#include <eglplus/gl_colorspace.hpp>
#include <eglplus/vg_colorspace.hpp>
#include <eglplus/vg_alpha_format.hpp>

namespace eglplus {

struct SurfaceValueTypeToSurfaceAttrib
{
	static RenderBuffer
	ValueType(std::integral_constant<int, 0>);
	SurfaceAttrib operator()(RenderBuffer) const
	{
		return SurfaceAttrib::RenderBuffer;
	}

	static MultisampleResolve
	ValueType(std::integral_constant<int, 1>);
	SurfaceAttrib operator()(MultisampleResolve) const
	{
		return SurfaceAttrib::MultisampleResolve;
	}

	static SwapBehavior
	ValueType(std::integral_constant<int, 2>);
	SurfaceAttrib operator()(SwapBehavior) const
	{
		return SurfaceAttrib::SwapBehavior;
	}

	static TextureTarget
	ValueType(std::integral_constant<int, 3>);
	SurfaceAttrib operator()(TextureTarget) const
	{
		return SurfaceAttrib::TextureTarget;
	}

	static TextureFormat
	ValueType(std::integral_constant<int, 4>);
	SurfaceAttrib operator()(TextureFormat) const
	{
		return SurfaceAttrib::TextureFormat;
	}

	static VGColorspace
	ValueType(std::integral_constant<int, 5>);
	SurfaceAttrib operator()(VGColorspace) const
	{
		return SurfaceAttrib::VGColorspace;
	}

	static VGAlphaFormat
	ValueType(std::integral_constant<int, 6>);
	SurfaceAttrib operator()(VGAlphaFormat) const
	{
		return SurfaceAttrib::VGAlphaFormat;
	}

#ifndef EGL_GL_COLORSPACE
	static std::integral_constant<int, 6> MaxValueType(void);
#else
	static GLColorspace
	ValueType(std::integral_constant<int, 7>);
	SurfaceAttrib operator()(GLColorspace) const
	{
		return SurfaceAttrib::GLColorspace;
	}

	static std::integral_constant<int, 7> MaxValueType(void);
#endif
};

/// Attribute list for surface attributes
typedef AttributeList<
	SurfaceAttrib,
	SurfaceValueTypeToSurfaceAttrib,
	AttributeListTraits
> SurfaceAttribs;

/// Finished list of surface attribute values
typedef FinishedAttributeList<
	SurfaceAttrib,
	AttributeListTraits
> FinishedSurfaceAttribs;

class Surface;
::EGLSurface GetEGLHandle(const Surface&)
OGLPLUS_NOEXCEPT(true);

/// Wrapper for EGLSurfaces
class Surface
{
private:
	Display _display;
	::EGLSurface _handle;

	friend ::EGLSurface GetEGLHandle(const Surface&)
	OGLPLUS_NOEXCEPT(true);

	Surface(const Surface&);

	struct Pbuffer_ { };

	static ::EGLSurface _init(
		Pbuffer_,
		const Display& display,
		const Config& config,
		const FinishedSurfaceAttribs& attribs
	)
	{
		::EGLSurface result = EGLPLUS_EGLFUNC(CreatePbufferSurface)(
			GetEGLHandle(display),
			GetEGLHandle(config),
			attribs.Get()
		);
		EGLPLUS_CHECK_SIMPLE(CreatePbufferSurface);
		return result;
	}

	Surface(
		Pbuffer_ sel,
		const Display& display,
		const Config& config,
		const FinishedSurfaceAttribs& attribs
	): _display(display)
	 , _handle(_init(sel, _display, config, attribs))
	{ }

	struct Pixmap_{ };

	static ::EGLSurface _init(
		Pixmap_,
		const Display& display,
		const Config& config,
		EGLNativePixmapType pixmap,
		const FinishedSurfaceAttribs& attribs
	)
	{
		::EGLSurface result = EGLPLUS_EGLFUNC(CreatePixmapSurface)(
			GetEGLHandle(display),
			GetEGLHandle(config),
			pixmap,
			attribs.Get()
		);
		EGLPLUS_CHECK_SIMPLE(CreatePixmapSurface);
		return result;
	}

	Surface(
		Pixmap_ sel,
		const Display& display,
		const Config& config,
		EGLNativePixmapType pixmap,
		const FinishedSurfaceAttribs& attribs
	): _display(display)
	 , _handle(_init(sel, _display, config, pixmap, attribs))
	{ }

	struct Window_ { };

	static ::EGLSurface _init(
		Window_,
		const Display& display,
		const Config& config,
		EGLNativeWindowType window,
		const FinishedSurfaceAttribs& attribs
	)
	{
		::EGLSurface result = EGLPLUS_EGLFUNC(CreateWindowSurface)(
			GetEGLHandle(display),
			GetEGLHandle(config),
			window,
			attribs.Get()
		);
		EGLPLUS_CHECK_SIMPLE(CreateWindowSurface);
		return result;
	}

	Surface(
		Window_ sel,
		const Display& display,
		const Config& config,
		EGLNativeWindowType window,
		const FinishedSurfaceAttribs& attribs
	): _display(display)
	 , _handle(_init(sel, _display, config, window, attribs))
	{ }
public:
	/// Surfaces are move-constructible
	Surface(Surface&& tmp)
	 : _display(tmp._display)
	 , _handle(tmp._handle)
	{
		tmp._handle = EGL_NO_SURFACE;
	}

	/// Destroys the wrapped surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{DestroySurface}
	 */
	~Surface(void)
	{
		if(_handle != EGL_NO_SURFACE)
		{
			EGLPLUS_EGLFUNC(DestroySurface)(
				GetEGLHandle(_display),
				_handle
			);
			EGLPLUS_VERIFY_SIMPLE(DestroySurface);
		}
	}

	/// Creates a Pbuffer surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{CreatePbufferSurface}
	 */
	static Surface Pbuffer(
		const Display& display,
		const Config& config,
		const FinishedSurfaceAttribs& attribs
	)
	{
		return Surface(
			Pbuffer_(),
			display,
			config,
			attribs
		);
	}

	/// Creates a Pixmap surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{CreatePixmapSurface}
	 */
	static Surface Pixmap(
		const Display& display,
		const Config& config,
		EGLNativePixmapType pixmap,
		const FinishedSurfaceAttribs& attribs
	)
	{
		return Surface(
			Pixmap_(),
			display,
			config,
			pixmap,
			attribs
		);
	}

	/// Creates a Window surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{CreateWindowSurface}
	 */
	static Surface Window(
		const Display& display,
		const Config& config,
		EGLNativeWindowType window,
		const FinishedSurfaceAttribs& attribs
	)
	{
		return Surface(
			Window_(),
			display,
			config,
			window,
			attribs
		);
	}

	/// Swap buffers
	/**
	 *  @eglsymbols
	 *  @eglfunref{SwapBuffers}
	 */
	Boolean SwapBuffers(void)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(SwapBuffers)(
				GetEGLHandle(_display),
				_handle
			), std::nothrow
		);
		EGLPLUS_VERIFY_SIMPLE(SwapBuffers);
		return result;
	}

	/// Copy buffer to native pixmap
	/**
	 *  @eglsymbols
	 *  @eglfunref{CopyBuffers}
	 */
	Boolean CopyBuffers(EGLNativePixmapType target)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(CopyBuffers)(
				GetEGLHandle(_display),
				_handle,
				target
			), std::nothrow
		);
		EGLPLUS_VERIFY_SIMPLE(CopyBuffers);
		return result;
	}

	/// Sets the value of the specified attribute
	/**
	 *  @eglsymbols
	 *  @eglfunref{SurfaceAttrib}
	 */
	Boolean Attrib(SurfaceAttrib attrib, EGLint value)
	{
		Boolean result(
			EGLPLUS_EGLFUNC(SurfaceAttrib)(
				GetEGLHandle(_display),
				_handle,
				EGLint(EGLenum(attrib)),
				value
			), std::nothrow
		);
		EGLPLUS_VERIFY_SIMPLE(SurfaceAttrib);
		return result;
	}

	/// Queries the specified surface attribute value
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 */
	Boolean QueryAttrib(SurfaceAttrib attrib, EGLint& value) const
	{
		Boolean result(
			EGLPLUS_EGLFUNC(QuerySurface)(
				GetEGLHandle(_display),
				_handle,
				EGLint(EGLenum(attrib)),
				&value
			), std::nothrow
		);
		EGLPLUS_VERIFY_SIMPLE(QuerySurface);
		return result;
	}

	/// Gets the specified surface attribute value
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 */
	EGLint GetAttrib(SurfaceAttrib attrib) const
	{
		EGLint result = EGL_UNKNOWN;
		QueryAttrib(attrib, result);
		return result;
	}

	/// Returns the Width of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{WIDTH}
	 */
	EGLint Width(void) const
	{
		return GetAttrib(SurfaceAttrib::Width);
	}

	/// Returns the height of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{HEIGHT}
	 */
	EGLint Height(void) const
	{
		return GetAttrib(SurfaceAttrib::Height);
	}

	/// Returns the render buffer setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{RENDER_BUFFER}
	 */
	eglplus::RenderBuffer RenderBuffer(void) const
	{
		return eglplus::RenderBuffer(
			GetAttrib(SurfaceAttrib::RenderBuffer)
		);
	}

	/// Sets the render buffer setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{SurfaceAttrib}
	 *  @egldefref{RENDER_BUFFER}
	 */
	void RenderBuffer(eglplus::RenderBuffer render_buffer)
	{
		Attrib(
			SurfaceAttrib::RenderBuffer,
			EGLint(render_buffer)
		);
	}

	/// Returns the multisample resolve setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{MULTISAMPLE_RESOLVE}
	 */
	eglplus::MultisampleResolve MultisampleResolve(void) const
	{
		return eglplus::MultisampleResolve(
			EGLenum(GetAttrib(SurfaceAttrib::MultisampleResolve))
		);
	}

	/// Sets the multisample resolve setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{SurfaceAttrib}
	 *  @egldefref{MULTISAMPLE_RESOLVE}
	 */
	void MultisampleResolve(eglplus::MultisampleResolve value)
	{
		Attrib(
			SurfaceAttrib::MultisampleResolve,
			EGLint(value)
		);
	}

	/// Returns the buffer swap behavior setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{SWAP_BEHAVIOR}
	 */
	eglplus::SwapBehavior SwapBehavior(void) const
	{
		return eglplus::SwapBehavior(
			EGLenum(GetAttrib(SurfaceAttrib::SwapBehavior))
		);
	}

	/// Sets the buffer swap behavior setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{SurfaceAttrib}
	 *  @egldefref{SWAP_BEHAVIOR}
	 */
	void SwapBehavior(eglplus::SwapBehavior value)
	{
		Attrib(
			SurfaceAttrib::SwapBehavior,
			EGLint(value)
		);
	}

	/// Returns the texture target setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{TEXTURE_TARGET}
	 */
	eglplus::TextureTarget TextureTarget(void) const
	{
		return eglplus::TextureTarget(
			EGLenum(GetAttrib(SurfaceAttrib::TextureTarget))
		);
	}

	/// Returns the texture format setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{TEXTURE_FORMAT}
	 */
	eglplus::TextureFormat TextureFormat(void) const
	{
		return eglplus::TextureFormat(
			EGLenum(GetAttrib(SurfaceAttrib::TextureFormat))
		);
	}

#if EGLPLUS_DOCUMENTATION_ONLY || defined(EGL_GL_COLORSPACE)
	/// Returns the OpenGL colorspace setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{GL_COLORSPACE}
	 */
	eglplus::GLColorspace GLColorspace(void) const
	{
		return eglplus::GLColorspace(
			EGLenum(GetAttrib(SurfaceAttrib::GLColorspace))
		);
	}
#endif

	/// Returns the OpenVG colorspace setting of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{VG_COLORSPACE}
	 */
	eglplus::VGColorspace VGColorspace(void) const
	{
		return eglplus::VGColorspace(
			EGLenum(GetAttrib(SurfaceAttrib::VGColorspace))
		);
	}

	/// Returns the OpenVG alpha format of the surface
	/**
	 *  @eglsymbols
	 *  @eglfunref{QuerySurface}
	 *  @egldefref{VG_ALPHA_FORMAT}
	 */
	eglplus::VGAlphaFormat VGAlphaFormat(void) const
	{
		return eglplus::VGAlphaFormat(
			EGLenum(GetAttrib(SurfaceAttrib::VGAlphaFormat))
		);
	}
};

inline
::EGLSurface GetEGLHandle(const Surface& surface)
OGLPLUS_NOEXCEPT(true)
{
	return surface._handle;
}

} // namespace eglplus

#endif // include guard
