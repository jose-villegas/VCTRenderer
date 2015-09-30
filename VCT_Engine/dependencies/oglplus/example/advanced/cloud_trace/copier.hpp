/**
 *  @file advanced/cloud_trace/copier_default.hpp
 *  @brief Declares the classes that handle copying of raytrace output to target
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_COPIER_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_COPIER_1119071146_HPP

#include "app_data.hpp"
#include "raytracer.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <oglplus/context.hpp>
#include <oglplus/framebuffer.hpp>

#ifdef CLOUD_TRACE_USE_NV_copy_image
#include <oglplus/x11/display.hpp>
#include <oglplus/glx/context.hpp>
#endif

namespace oglplus {
namespace cloud_trace {

class RaytraceCopierDefault
{
private:
	Context gl;
	Framebuffer fbo;
public:
	struct Params
	{
		template <typename ... T>
		Params(T&...) { }
	};

	RaytraceCopierDefault(AppData&, RaytracerTarget&);

	void Copy(AppData&, Params&, Raytracer&, unsigned tile);
};

#ifdef CLOUD_TRACE_USE_NV_copy_image
class RaytraceCopierNV_copy_image
{
private:
	RaytracerTarget& rt_target;
	void (*copy_func)(void);
public:
	struct Params
	{
		::Display* display;
		GLXContext source_context;
		GLXContext target_context;

		Params(
			const x11::Display& disp,
			const glx::Context& src_context,
			const glx::Context& tgt_context
		): display(disp)
		 , source_context(src_context)
		 , target_context(tgt_context)
		{ }

	};

	RaytraceCopierNV_copy_image(AppData&, RaytracerTarget&);

	void Copy(AppData&, Params&, Raytracer&, unsigned tile);
};

typedef RaytraceCopierNV_copy_image RaytraceCopier;
#else
typedef RaytraceCopierDefault RaytraceCopier;
#endif

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
