/**
 *  @file advanced/cloud_trace/raytracer.hpp
 *  @brief Declares the raytracer class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_RAYTRACER_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_RAYTRACER_1119071146_HPP

#include "app_data.hpp"
#include "cloud_data.hpp"
#include "programs.hpp"
#include "textures.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <oglplus/context.hpp>
#include <oglplus/framebuffer.hpp>
#include <oglplus/renderbuffer.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <array>

namespace oglplus {
namespace cloud_trace {

struct RaytracerData
{
	CloudData cloud_data;
	CloudVolume cloud_vol;

	RaytracerData(const AppData&);
};

struct RaytracerResources
{
	CloudBuffer cloud_buf;
	CloudTexture cloud_tex;

	RaytraceProg raytrace_prog;

	RaytracerResources(
		const AppData&,
		const RaytracerData&,
		ResourceAllocator&
	);
	void Use(void);
};

struct RaytracerTarget
{
	const GLuint tex_unit;

	Context gl;
	Texture tex;
	Framebuffer fbo;

	RaytracerTarget(AppData&, ResourceAllocator&);
	void Clear(AppData&);
};

class Raytracer
{
private:
	Context gl;

	const unsigned w, h;

	RaytracerResources& resources;

	shapes::ShapeWrapper screen;

public:
	Renderbuffer rbo;
	Framebuffer fbo;

	Raytracer(AppData&, RaytracerResources&);

	void Use(AppData&);
	void InitFrame(AppData&, unsigned face);
	void BeginWork(const AppData&);
	void EndWork(const AppData&);
	void Raytrace(const AppData&, unsigned tile);
};

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
