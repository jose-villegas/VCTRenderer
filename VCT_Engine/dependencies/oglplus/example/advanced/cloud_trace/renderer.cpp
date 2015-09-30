/**
 *  @file advanced/cloud_trace/renderer.cpp
 *  @brief Implements the renderer class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "renderer.hpp"

#include <oglplus/framebuffer.hpp>
#include <oglplus/opt/list_init.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/images/image_spec.hpp>

#include <cassert>

namespace oglplus {
namespace cloud_trace {

Renderer::Renderer(const AppData& app_data, GLuint raytrace_tex_unit)
 : gl()
 , render_prog(app_data)
 , screen(List("Position")("TexCoord").Get(), shapes::Screen(), render_prog)
{
	render_prog.raytrace_output.Set(raytrace_tex_unit);
}

void Renderer::Use(const AppData&)
{
	dfb.Bind(FramebufferTarget::Draw);
	render_prog.Use();
	screen.Use();
}

void Renderer::InitFrame(const AppData& app_data, unsigned face)
{
	render_prog.SetRayMatrix(app_data, face);
}

void Renderer::Render(const AppData& app_data)
{
	gl.Viewport(0, 0, app_data.render_width, app_data.render_height);
	render_prog.raytrace_size.Set(app_data.raytrace_width, app_data.raytrace_height);

	screen.Draw();
}

} // namespace cloud_trace
} // namespace oglplus

