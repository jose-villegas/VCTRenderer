/**
 *  @file advanced/cloud_trace/programs.hpp
 *  @brief Declaration of the shading program classes
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_PROGRAMS_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_PROGRAMS_1119071146_HPP

#include "app_data.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>
#include <oglplus/program.hpp>
#include <oglplus/dsa/uniform.hpp>
#include <oglplus/uniform_block.hpp>
#include <oglplus/prog_var/optional.hpp>

#include <oglplus/math/vector.hpp>
#include <oglplus/math/matrix.hpp>

namespace oglplus {
namespace cloud_trace {

class RaytraceProg : public Program
{
private:
	static Program make(void);

	Program& self(void);

	ProgramUniform<Mat3f> ray_matrix;
public:
	ProgramUniformSampler cloud_tex;
	ProgramUniform<GLint> cloud_count;
	ProgramUniform<Vec4f> clip_plane0;
	ProgramUniform<Vec4f> clip_plane1;
	ProgramUniform<Vec4f> clip_plane2;
	ProgramUniform<Vec4f> clip_plane3;

	UniformBlock cloud_block;

	RaytraceProg(const AppData&);

	void SetRayMatrix(const AppData&, unsigned face);
};

class RenderProg : public Program
{
private:
	static Program make(const AppData&);

	Program& self(void);

	Optional<ProgramUniform<GLint>> cube_face;
	Optional<ProgramUniform<Mat3f>> ray_matrix;
public:
	ProgramUniform<Vec2f> raytrace_size;
	ProgramUniformSampler raytrace_output;

	RenderProg(const AppData&);

	void SetRayMatrix(const AppData&, unsigned face);
};

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
