/**
 *  @file advanced/cloud_trace/copier.cpp
 *  @brief Implements the classes that handle copying of raytrace output to target
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "copier.hpp"

#include <oglplus/object/auto_rebind.hpp>

#include <stdexcept>
#include <cassert>

namespace oglplus {
namespace cloud_trace {

RaytraceCopierDefault::RaytraceCopierDefault(AppData&, RaytracerTarget& target)
{
	AutoRebind<Framebuffer> rebind_fb(FramebufferTarget::Draw);

	fbo	<< FramebufferTarget::Draw
		<< FramebufferAttachment::Color << target.tex;
}

void RaytraceCopierDefault::Copy(
	AppData& app_data,
	RaytraceCopierDefault::Params&,
	Raytracer& raytracer,
	unsigned tile
)
{
	const unsigned w = app_data.cols();
	const unsigned h = app_data.rows();
	unsigned i = tile % w;
	unsigned j = tile / w;
	assert(j*h+i < w*h);

	j = h-j-1;

	raytracer.fbo << FramebufferTarget::Read;
	fbo << FramebufferTarget::Draw;

	gl.Flush();
	gl.BlitFramebuffer(
		app_data.tile*(i+0),
		app_data.tile*(j+0),
		app_data.tile*(i+1),
		app_data.tile*(j+1),
		app_data.tile*(i+0),
		app_data.tile*(j+0),
		app_data.tile*(i+1),
		app_data.tile*(j+1),
		BufferSelectBit::ColorBuffer,
		BlitFilter::Nearest
	);

	raytracer.fbo << FramebufferTarget::Draw;
}

#ifdef CLOUD_TRACE_USE_NV_copy_image
typedef void (*PFNglXCopyImageSubDataNV)(
	::Display *dpy,
	GLXContext srcCtx,
	GLuint srcName,
	GLenum srcTarget,
	GLint srcLevel,
	GLint srcX, GLint srcY, GLint srcZ,
	GLXContext dstCtx,
	GLuint dstName,
	GLenum dstTarget,
	GLint dstLevel,
	GLint dstX,
	GLint dstY,
	GLint dstZ,
	GLsizei width,
	GLsizei height,
	GLsizei depth
);

RaytraceCopierNV_copy_image::RaytraceCopierNV_copy_image(
	AppData&,
	RaytracerTarget& target
): rt_target(target)
 , copy_func(::glXGetProcAddress(((const GLubyte*)"glXCopyImageSubDataNV")))
{
	if(!copy_func)
	{
		throw std::runtime_error(
			"Failed to get pointer to glXCopyImageSubDataNV"
		);
	}
}

void RaytraceCopierNV_copy_image::Copy(
	AppData& app_data,
	RaytraceCopierNV_copy_image::Params& params,
	Raytracer& raytracer,
	unsigned tile
)
{
	const unsigned w = app_data.cols();
	const unsigned h = app_data.rows();
	unsigned i = tile % w;
	unsigned j = tile / w;

	assert(j*h+i < w*h);

	j = h-j-1;

	unsigned x = app_data.tile*i;
	unsigned y = app_data.tile*j;

	assert(copy_func);
	((PFNglXCopyImageSubDataNV)copy_func)(
		params.display,
		params.source_context,
		GetGLName(raytracer.rbo),
		GL_RENDERBUFFER,
		0, x, y, 0,
		params.target_context,
		GetGLName(rt_target.tex),
		GL_TEXTURE_RECTANGLE,
		0, x, y, 0,
		app_data.tile,
		app_data.tile,
		1
	);
	OGLPLUS_VERIFY_SIMPLE(GetError);
}
#endif

} // namespace cloud_trace
} // namespace oglplus
