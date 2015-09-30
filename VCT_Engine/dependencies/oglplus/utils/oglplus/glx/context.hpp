/**
 *  @file oglplus/glx/context.hpp
 *  @brief Wrapper for GLX contexts
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_GLX_CONTEXT_1107121519_HPP
#define UTILS_OGLPLUS_GLX_CONTEXT_1107121519_HPP

#include <oglplus/x11/display.hpp>
#include <oglplus/x11/window.hpp>
#include <oglplus/glx/drawable.hpp>
#include <oglplus/glx/fb_config.hpp>

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <stdexcept>


namespace oglplus {
namespace glx {

class Context
 : public x11::DisplayObject< ::GLXContext, void(::Display*, ::GLXContext)>
{
private:
	static ::GLXContext make_context(
		const x11::Display& display,
		const FBConfig& fbc,
		int version_major,
		int version_minor,
		bool debugging,
		bool compatibility,
		::GLXContext share_context = ::GLXContext(0)
	)
	{
		typedef GLXContext (*glXCreateContextAttribsARBProc)(
			::Display*,
			::GLXFBConfig,
			::GLXContext,
			Bool,
			const int*
		);

		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
		glXCreateContextAttribsARB =
			reinterpret_cast<glXCreateContextAttribsARBProc>(
				glXGetProcAddressARB(
					reinterpret_cast<const GLubyte *>(
						"glXCreateContextAttribsARB"
					)
				)
			);

		const int CONTEXT_MAJOR_VERSION_ARB = 0x2091;
		const int CONTEXT_MINOR_VERSION_ARB = 0x2092;
		const int CONTEXT_FLAGS_ARB = 0x2094;
		const int CONTEXT_PROFILE_MASK_ARB = 0x9126;
		const int CONTEXT_DEBUG_BIT_ARB = 0x0001;
		const int CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;
		const int CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 0x00000002;

		int context_attribs[] =
		{
			CONTEXT_MAJOR_VERSION_ARB, version_major,
			CONTEXT_MINOR_VERSION_ARB, version_minor,
			CONTEXT_FLAGS_ARB, (debugging?CONTEXT_DEBUG_BIT_ARB:0),
			CONTEXT_PROFILE_MASK_ARB, (
				compatibility
				?CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
				:CONTEXT_CORE_PROFILE_BIT_ARB
			),
			None
		};
		::GLXContext res = glXCreateContextAttribsARB(
			display,
			fbc.Handle(),
			share_context,
			True,
			context_attribs
		);
		::XSync(display, False);
		return res;
	}
public:
	Context(
		const x11::Display& display,
		const FBConfig& fbc,
		int version_major,
		int version_minor,
		bool debugging = true,
		bool compatibility = false
	): x11::DisplayObject< ::GLXContext, void(::Display*, ::GLXContext)>(
		display,
		make_context(
			display,
			fbc,
			version_major,
			version_minor,
			debugging,
			compatibility
		),
		::glXDestroyContext,
		"Error creating glX context"
	){ }

	Context(
		const x11::Display& display,
		const FBConfig& fbc,
		const Context& share_context,
		int version_major,
		int version_minor,
		bool debugging = true,
		bool compatibility = false
	): x11::DisplayObject< ::GLXContext, void(::Display*, ::GLXContext)>(
		display,
		make_context(
			display,
			fbc,
			version_major,
			version_minor,
			debugging,
			compatibility,
			share_context.Handle()
		),
		::glXDestroyContext,
		"Error creating sharing glX context"
	){ }

	void MakeCurrent(const Drawable& drawable) const
	{
		::glXMakeCurrent(this->DisplayRef(), drawable, this->Handle());
	}

	static void Release(const x11::Display& display)
	{
		::glXMakeCurrent(display, 0, 0);
	}

	void SwapBuffers(const Drawable& drawable) const
	{
		::glXSwapBuffers(this->DisplayRef(), drawable);
	}
};


} // namespace glx
} // namespace oglplus

#endif // include guard
