/**
 *  @file advanced/cloud_trace/renderer.hpp
 *  @brief Declares the renderer class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_RENDERER_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_RENDERER_1119071146_HPP

#include "app_data.hpp"
#include "resources.hpp"
#include "programs.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <oglplus/context.hpp>
#include <oglplus/texture.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/shapes/wrapper.hpp>

namespace oglplus {
namespace cloud_trace {

class Renderer
{
private:
	Context gl;

	DefaultFramebuffer dfb;

	RenderProg render_prog;

	shapes::ShapeWrapper screen;
public:
	Renderer(const AppData&, GLuint raytrace_tex_unit);

	void Use(const AppData&);
	void InitFrame(const AppData&, unsigned face);
	void Render(const AppData&);
};

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
