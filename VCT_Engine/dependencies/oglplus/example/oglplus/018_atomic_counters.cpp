/**
 *  @example oglplus/018_atomic_counters.cpp
 *  @brief Shows how to use atomic-counters to visualize rendering order
 *
 *  @oglplus_screenshot{018_atomic_counters}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_shader_atomic_counters}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/plane.hpp>

#include "example.hpp"

namespace oglplus {

class LandscapeExample : public Example
{
private:
	GLuint grid_side;

	// helper object building plane vertex attributes
	shapes::Plane make_plane;
	// helper object encapsulating plane drawing instructions
	shapes::DrawingInstructions plane_instr;
	// indices pointing to plane primitive elements
	shapes::Plane::IndexArray plane_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;

	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<GLfloat> vc_int, gc_int, fc_int;

	// A vertex array object for the rendered plane
	VertexArray plane;

	// VBOs for the plane's vertex attributes
	Buffer positions;

	// Buffer for the atomic counters
	Buffer counters;
public:
	LandscapeExample(void)
	 : grid_side(8)
	 , make_plane(
		Vec3f(0.0f, 0.0f, 0.0f),
		Vec3f(1.0f, 0.0f, 0.0f),
		Vec3f(0.0f, 0.0f,-1.0f),
		grid_side, grid_side
	), plane_instr(make_plane.Instructions())
	 , plane_indices(make_plane.Indices())
	{
		VertexShader vs;
		vs.Source(
			"#version 420\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"

			"layout(binding = 0, offset = 0) uniform atomic_uint vc;"
			"const float mult = 1.0/128.0;"
			"uniform float vc_int;"

			"in vec4 Position;"

			"out vec3 vertColor;"

			"void main(void)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		Position;"
			"	vertColor = vec3("
			"		fract(atomicCounterIncrement(vc)*mult),"
			"		0.0,"
			"		0.0 "
			"	)*max(vc_int, 0.0);"
			"}"
		);
		vs.Compile();
		prog.AttachShader(vs);

		GeometryShader gs;
		gs.Source(
			"#version 420\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 3) out;"

			"layout(binding = 0, offset = 4) uniform atomic_uint gc;"
			"const float mult = 1.0/128.0;"
			"uniform float gc_int;"

			"in vec3 vertColor[3];"

			"out vec3 geomColor;"

			"void main(void)"
			"{"
			"	vec3 Color = vec3("
			"		0.0,"
			"		fract(atomicCounterIncrement(gc)*mult),"
			"		0.0 "
			"	)*max(gc_int, 0.0);"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		gl_Position = gl_in[v].gl_Position;"
			"		geomColor = vertColor[v] + Color;"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();
		prog.AttachShader(gs);

		FragmentShader fs;
		fs.Source(
			"#version 420\n"

			"layout(binding = 0, offset = 8) uniform atomic_uint fc;"
			"const float mult = 1.0/4096.0;"
			"uniform float fc_int;"

			"in vec3 geomColor;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec3 Color = vec3("
			"		0.0,"
			"		0.0,"
			"		sqrt(fract(atomicCounterIncrement(fc)*mult))"
			"	)*max(fc_int, 0.0);"
			"	fragColor = geomColor + Color;"
			"}"
		);
		fs.Compile();
		prog.AttachShader(fs);

		prog.Link();
		prog.Use();

		projection_matrix = UniformLoc(prog, "ProjectionMatrix");
		camera_matrix = UniformLoc(prog, "CameraMatrix");
		vc_int = UniformLoc(prog, "vc_int");
		gc_int = UniformLoc(prog, "gc_int");
		fc_int = UniformLoc(prog, "fc_int");

		// bind the VAO for the plane
		plane.Bind();

		// bind the VBO for the plane vertices
		positions.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_plane.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		counters.Bind(Buffer::Target::AtomicCounter);
		{
			const GLuint tmp[3] = {0u, 0u, 0u};
			Buffer::Data(
				Buffer::Target::AtomicCounter,
				3, tmp,
				BufferUsage::DynamicDraw
			);
		}
		counters.BindBase(Buffer::IndexedTarget::AtomicCounter, 0);

		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		plane.Bind();
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				width, height,
				1, 20
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(0.0f, 0.5f, 0.0f),
				2.8f,
				FullCircles(time / 90.0),
				Degrees(85)
			)
		);
		vc_int.Set(SineWave(GLfloat(time / 5.0 - 0.25)));
		gc_int.Set(SineWave(GLfloat(time / 6.0)));
		fc_int.Set(SineWave(GLfloat(time / 7.0 + 0.25)));

		plane_instr.Draw(plane_indices);

		const GLuint tmp[3] = {0u, 0u, 0u};
		Buffer::SubData(
			BufferTarget::AtomicCounter,
			0, 3, tmp
		);
	}

	bool Continue(double time)
	{
		return time < 30.0;
	}
};

void setupExample(ExampleParams& /*params*/){ }

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& /*example*/,
	unsigned /*thread_id*/,
	const ExampleParams& /*params*/
){ return std::unique_ptr<ExampleThread>(); }

std::unique_ptr<Example> makeExample(const ExampleParams& /*params*/)
{
	return std::unique_ptr<Example>(new LandscapeExample);
}

} // namespace oglplus
