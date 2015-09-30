/**
 *  @example oglplus/029_gpu_sort_tfb.cpp
 *  @brief Shows sorting on the GPU using a sorting NW and transform feedback
 *
 *  @oglplus_screenshot{029_gpu_sort_tfb}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/bound/buffer.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/images/sort_nw.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/spiral_sphere.hpp>
#include <oglplus/opt/list_init.hpp>

#include "example.hpp"

#include <vector>
#include <cstdlib>

namespace oglplus {

class SortNWTex
 : public Texture
{
private:
	GLuint pass_count;
	Texture& self(void) { return *this; }
public:
	SortNWTex(GLuint n)
	{
		images::Image sort_nw_map = images::SortNWMap(n);
		pass_count = sort_nw_map.Height();

		Context gl;
		Texture::Active(0);
		gl.Bound(TextureTarget::Rectangle, self())
			.Filter(TextureFilter::Nearest)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.Image2D(sort_nw_map);
	}

	GLuint PassCount(void) const { return pass_count; }
};

class Particles
{
private:
	GLuint count;
public:
	SortNWTex sort_nw;
	Buffer positions;
	Buffer distances;
	Buffer indices_f;
	Buffer indices_d;

	Particles(GLuint n)
	 : count(n)
	 , sort_nw(n)
	{
		Context gl;
		const GLfloat irm = 1.0f/RAND_MAX;
		std::vector<GLfloat> pos_data(count*3);

		for(GLuint p=0; p!=count; ++p)
		{
			for(GLuint c=0; c!=3; ++c)
			{
				pos_data[p*3+c] = 2.0f*(0.5f-std::rand()*irm);
			}
		}

		gl.Bound(BufferTarget::Array, positions).Data(pos_data);
		gl.Bound(BufferTarget::Uniform, distances).Data<GLfloat>(
			count,
			nullptr,
			BufferUsage::DynamicDraw
		);
		gl.Bound(BufferTarget::CopyRead, indices_f).Data<GLuint>(
			count,
			nullptr,
			BufferUsage::DynamicRead
		);
		gl.Bound(BufferTarget::CopyWrite,indices_d).Data<GLuint>(
			count,
			nullptr,
			BufferUsage::DynamicDraw
		);

	}

	GLuint Count(void) const { return count; }
};

class MeshProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		prog << VertexShader(ObjectDesc("Mesh"),
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"

			"void main(void)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix*CameraMatrix*ModelMatrix*Position;"
			"}"
		);

		prog << FragmentShader(ObjectDesc("Mesh"),
			"#version 150\n"
			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	fragColor = vec3(0.1);"
			"}"
		);
		prog.Link();

		return prog;
	}
	Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	MeshProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_matrix(self(), "ModelMatrix")
	{ }
};

class DistProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		prog << VertexShader(ObjectDesc("Dist"),
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"in vec4 Position;"
			"out uint xfbIndex;"
			"out float xfbDistance;"
			"void main(void)"
			"{"
			"	vec4 p = ProjectionMatrix*CameraMatrix*Position;"
			"	xfbIndex = uint(gl_VertexID);"
			"	xfbDistance = p.z;"
			"}"
		);

		prog << TransformFeedbackMode::SeparateAttribs
			<< "xfbIndex"
			<< "xfbDistance";

		prog.Link();

		return prog;
	}
	Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;

	DistProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	{ }
};

class SortProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		prog << VertexShader(ObjectDesc("Sort"),
			"#version 150\n"
			"uniform usampler2DRect SortNW;"
			"uniform int Pass;"
			"layout (packed) uniform IndexBlock{ uint  Indices[4096]; };"
			"layout (packed) uniform DistBlock { float Distances[4096]; };"

			"in uint Index;"

			"out uint xfbIndex;"

			"void main(void)"
			"{"
			"	uint enc = texelFetch(SortNW, ivec2(gl_VertexID, Pass)).r;"
			"	if(enc != 0u)"
			"	{"
			"		int Sig1 = ((enc & 0x1u) != 0u)?-1:1;"
			"		int Sig2 = ((enc & 0x2u) != 0u)?-1:1;"
			"		int Offs = Sig2 * int(enc >> 2);"
			"		uint Index2 = Indices[gl_VertexID+Offs];"
			"		float Diff = Distances[Index] - Distances[Index2];"
			"		if(Diff*Sig1*Sig2 < 0)"
			"		{"
			"			xfbIndex = Index2;"
			"			return;"
			"		}"
			"	}"
			"	xfbIndex = Index;"
			"}"
		);

		prog.TransformFeedbackVarying("xfbIndex");
		prog.Link();

		return prog;
	}
	Program& self(void) { return *this; }
public:
	ProgramUniform<GLint> pass;

	SortProg(void)
	 : Program(make())
	 , pass(self(), "Pass")
	{
		ProgramUniformSampler(self(), "SortNW").Set(0);
	}
};

class DrawProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		prog << VertexShader(ObjectDesc("Draw"),
			"#version 150\n"
			"uniform mat4 CameraMatrix;"
			"in vec4 Position;"
			"out vec3 vertColor;"

			"void main(void)"
			"{"
			"	gl_Position = CameraMatrix * Position;"
			"	vertColor = normalize(vec3(1)-Position.rgb);"
			"}"
		);

		prog << GeometryShader(ObjectDesc("Draw"),
			"#version 150\n"
			"layout(points) in;"
			"layout(triangle_strip, max_vertices = 4) out;"

			"uniform mat4 ProjectionMatrix;"
			"in vec3 vertColor[1];"
			"out vec3 geomColor;"

			"void main(void)"
			"{"
			"	geomColor = vertColor[0];"
			"	float s = 0.1;"
			"	float yo[2] = float[2](-1.0, 1.0);"
			"	float xo[2] = float[2](-1.0, 1.0);"
			"	for(int j=0;j!=2;++j)"
			"	for(int i=0;i!=2;++i)"
			"	{"
			"		float xoffs = xo[i]*s;"
			"		float yoffs = yo[j]*s;"
			"		gl_Position = ProjectionMatrix * vec4("
			"			gl_in[0].gl_Position.x-xoffs,"
			"			gl_in[0].gl_Position.y-yoffs,"
			"			gl_in[0].gl_Position.z,"
			"			1.0"
			"		);"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);

		prog << FragmentShader(ObjectDesc("Draw"),
			"#version 150\n"
			"in vec3 geomColor;"
			"out vec4 fragColor;"

			"void main(void)"
			"{"
			"	float a = 1.0/16.0;"
			"	fragColor = vec4(geomColor, a);"
			"}"
		);

		prog.Link();

		return prog;
	}
	Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;

	DrawProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	{ }
};

class MeshProc
{
private:
	Context gl;
public:
	MeshProg prog;
	shapes::ShapeWrapper mesh;

	MeshProc(void)
	 : mesh(List("Position").Get(), shapes::SpiralSphere(), prog)
	{ }

	void operator()(void)
	{
		gl.Use(prog);
		mesh.Use();
		mesh.Draw();
	}
};

class DistProc
{
private:
	Context gl;
	VertexArray vao;
	DefaultTransformFeedback xfb;
public:
	DistProg prog;

	DistProc(const Particles& particles)
	{
		gl.Bind(vao);

		gl.Bind(BufferTarget::Array, particles.positions);
		(prog|"Position").Setup<Vec3f>().Enable();

		gl.Bind(NoVertexArray());

		gl.Bind(BufferIndexedTarget::TransformFeedback, 0, particles.indices_f);
		gl.Bind(BufferIndexedTarget::TransformFeedback, 1, particles.distances);
	}

	void operator()(const Particles& particles)
	{
		gl.Bind(vao);
		gl.Use(prog);
		xfb.BeginPoints();
		gl.DrawArrays(PrimitiveType::Points, 0, particles.Count());
		xfb.End();
	}
};

class SortProc
{
private:
	Context gl;
	VertexArray vao;
	DefaultTransformFeedback xfb;
public:
	SortProg prog;

	SortProc(const Particles& particles)
	{
		gl.Bind(vao);

		gl.Bind(BufferTarget::Array, particles.indices_d);
		(prog|"Index").Setup<GLint>().Enable();

		gl.Bind(NoVertexArray());

		gl.Bind(BufferIndexedTarget::TransformFeedback, 0, particles.indices_f);

		gl.Bind(BufferIndexedTarget::Uniform, 0, particles.indices_d);
		gl.Bind(BufferIndexedTarget::Uniform, 1, particles.distances);

		gl.Use(prog);
		UniformBlock(prog,"IndexBlock").Binding(0);
		UniformBlock(prog, "DistBlock").Binding(1);
		gl.Use(NoProgram());
	}

	void operator()(const Particles& particles)
	{
		gl.Bind(vao);
		gl.Use(prog);
		for(GLuint p=0; p!=particles.sort_nw.PassCount(); ++p)
		{
			Buffer::CopySubData(
				BufferTarget::CopyRead,
				BufferTarget::CopyWrite,
				0, 0, particles.Count()*sizeof(GLuint)
			);
			prog.pass.Set(p);

			xfb.BeginPoints();
			gl.DrawArrays(PrimitiveType::Points, 0, particles.Count());
			xfb.End();
		}
		Buffer::CopySubData(
			BufferTarget::CopyRead,
			BufferTarget::CopyWrite,
			0, 0, particles.Count()*sizeof(GLint)
		);
	}
};

class DrawProc
{
private:
	Context gl;
	VertexArray vao;
public:
	DrawProg prog;

	DrawProc(const Particles& particles)
	{
		gl.Bind(vao);

		gl.Bind(BufferTarget::ElementArray, particles.indices_d);

		gl.Bind(BufferTarget::Array, particles.positions);
		(prog|"Position").Setup<Vec3f>().Enable();

		gl.Bind(NoVertexArray());
	}

	void operator()(const Particles& particles)
	{
		gl.Enable(Capability::Blend);
		gl.Bind(vao);
		gl.Use(prog);
		gl.DrawElements(
			PrimitiveType::Points,
			particles.Count(),
			DataType::UnsignedInt
		);
		gl.Disable(Capability::Blend);
	}
};

class SortingExample : public Example
{
private:
	Context gl;

	Particles particles;

	MeshProc mesh_proc;
	DistProc dist_proc;
	SortProc sort_proc;
	DrawProc draw_proc;
public:
	SortingExample(void)
	 : gl()
	 , particles(4096)
	 , dist_proc(particles)
	 , sort_proc(particles)
	 , draw_proc(particles)
	{
		gl.ClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		gl.Enable(Capability::DepthTest);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		auto projection = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 60
		);
		mesh_proc.prog.projection_matrix = projection;
		dist_proc.prog.projection_matrix = projection;
		draw_proc.prog.projection_matrix = projection;
	}

	Mat4f CameraMat(double time)
	{
		return CamMatrixf::Orbiting(
			Vec3f(0, 0, 0),
			GLfloat(5.0 - SineWave(time / 11.0)),
			FullCircles(time / 23.0),
			Degrees(SineWave(time / 17.0) * 80)
		);
	}

	Mat4f ModelMat(double time)
	{
		return ModelMatrixf::RotationA(
			Vec3f(1),
			FullCircles(time / 13.0)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CameraMat(time);

		mesh_proc.prog.model_matrix = ModelMat(time);
		mesh_proc.prog.camera_matrix = camera;
		dist_proc.prog.camera_matrix = camera;
		draw_proc.prog.camera_matrix = camera;

		mesh_proc();
		dist_proc(particles);
		sort_proc(particles);
		draw_proc(particles);
	}

	bool Continue(double time)
	{
		return time < 60.0;
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
	return std::unique_ptr<Example>(new SortingExample);
}

} // namespace oglplus
