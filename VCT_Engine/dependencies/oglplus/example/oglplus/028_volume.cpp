/**
 *  @example oglplus/028_volume.cpp
 *  @brief Shows volumetric data polygonization
 *
 *  @oglplus_screenshot{028_volume}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{VARIADIC_TEMPLATES}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/images/cloud.hpp>

#include <oglplus/shapes/tetrahedrons.hpp>
#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class VolumeVertShader
 : public VertexShader
{
public:
	VolumeVertShader(void)
	 : VertexShader(
		ObjectDesc("Volume vertex shader"),
		StrCRef("#version 150\n"
		"uniform sampler3D VolumeTex;"
		"uniform float Threshold;"
		"uniform float GridStep;"

		"in vec4 Position;"

		"out vec3 vertGradient;"
		"out float vertValue;"

		"void main(void)"
		"{"
		"	gl_Position = Position;"
		"	float Density = texture(VolumeTex, Position.xyz).r;"
		"	vertValue = Density - Threshold;"
		"	vertGradient = vec3(0.0, 0.0, 0.0);"
		"	for(int z=-1; z!=2; ++z)"
		"	for(int y=-1; y!=2; ++y)"
		"	for(int x=-1; x!=2; ++x)"
		"	{"
		"		vec3 Offs = vec3(GridStep*x, GridStep*y, GridStep*z);"
		"		vec3 Coord = Position.xyz + Offs;"
		"		float Diff = Density - texture(VolumeTex, Coord).r;"
		"		vertGradient += Diff * Offs;"
		"	}"
		"}")
	)
	{ }
};

class VolumeGeomShader
 : public GeometryShader
{
public:
	VolumeGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Volume geometry shader"),
		StrCRef("#version 150\n"
		"layout(triangles_adjacency) in;"
		"layout(triangle_strip, max_vertices = 4) out;"

		"uniform mat4 TransformMatrix;"
		"uniform vec3 CameraPosition, LightPosition;"

		"in vec3 vertGradient[];"
		"in float vertValue[];"

		"out vec3 geomNormal, geomLightDir, geomViewDir;"

		"void do_nothing(void){ }"

		"float find_t(int i1, int i2)"
		"{"
		"	float d = vertValue[i2] - vertValue[i1];"
		"	return -vertValue[i1]/d;"
		"}"

		"void make_vertex(int i1, int i2)"
		"{"
		"	float t = find_t(i1, i2);"
		"	gl_Position = mix("
		"		gl_in[i1].gl_Position,"
		"		gl_in[i2].gl_Position,"
		"		t"
		"	);"
		"	geomNormal = mix("
		"		vertGradient[i1], "
		"		vertGradient[i2], "
		"		t"
		"	);"
		"	geomLightDir = LightPosition - gl_Position.xyz;"
		"	geomViewDir = CameraPosition - gl_Position.xyz;"
		"	gl_Position = TransformMatrix * gl_Position;"
		"	EmitVertex();"
		"}"

		"void make_triangle(int a1, int a2, int b1, int b2, int c1, int c2)"
		"{"
		"	make_vertex(a1, a2);"
		"	make_vertex(b1, b2);"
		"	make_vertex(c1, c2);"
		"	EndPrimitive();"
		"}"

		"void make_quad(int a1,int a2,int b1,int b2,int c1,int c2,int d1,int d2)"
		"{"
		"	make_vertex(a1, a2);"
		"	make_vertex(b1, b2);"
		"	make_vertex(c1, c2);"
		"	make_vertex(d1, d2);"
		"	EndPrimitive();"
		"}"

		"void process_tetrahedron(int a, int b, int c, int d)"
		"{"
		"	if(vertValue[a] >= 0.0)"
		"	{"
		"		if(vertValue[b] >= 0.0)"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					do_nothing();"
		"				else make_triangle(d,a, d,b, d,c);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(c,a, c,d, c,b);"
		"				else make_quad(c,a, d,a, c,b, d,b);"
		"			}"
		"		}"
		"		else"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(b,c, b,d, b,a);"
		"				else make_quad(b,c, d,c, b,a, d,a);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_quad(c,a, c,d, b,a, b,d);"
		"				else make_triangle(c,a, d,a, b,a);"
		"			}"
		"		}"
		"	}"
		"	else"
		"	{"
		"		if(vertValue[b] >= 0.0)"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(a,b, a,d, a,c);"
		"				else make_quad(a,c, a,b, d,c, d,b);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_quad(a,b, a,d, c,b, c,d);"
		"				else make_triangle(a,b, d,b, c,b);"
		"			}"
		"		}"
		"		else"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_quad(b,c, b,d, a,c, a,d);"
		"				else make_triangle(b,c, d,c, a,c);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(a,d, c,d, b,d);"
		"				else do_nothing();"
		"			}"
		"		}"
		"	}"
		"}"

		"void main(void)"
		"{"
		"	process_tetrahedron(0, 2, 4, 1);"
		"}")
	)
	{ }
};

class VolumeFragShader
 : public FragmentShader
{
public:
	VolumeFragShader(void)
	 : FragmentShader(
		ObjectDesc("Volume fragment shader"),
		StrCRef("#version 150\n"

		"in vec3 geomNormal, geomLightDir, geomViewDir;"

		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	vec3 Normal = normalize(geomNormal);"
		"	vec3 LightDir = normalize(geomLightDir);"
		"	vec3 ViewDir = normalize(geomViewDir);"
		"	vec3 ViewRefl = reflect(-ViewDir, Normal);"
		"	vec3 Color = abs(vec3(1.0, 1.0, 1.0) - Normal);"

		"	float Ambient = 0.3;"
		"	float Diffuse = max(dot(Normal, LightDir), 0.0);"
		"	float Specular = pow(max(dot(ViewRefl, LightDir), 0.0), 16.0);"
		"	fragColor = vec4("
		"		Ambient * vec3(0.2, 0.1, 0.1)+"
		"		Diffuse * Color +"
		"		Specular* vec3(1.0, 1.0, 1.0),"
		"		1.0"
		"	);"
		"}")
	)
	{ }
};

class VolumeProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(VolumeVertShader());
		prog.AttachShader(VolumeGeomShader());
		prog.AttachShader(VolumeFragShader());
		prog.Link();
		prog.Use();
		return prog;
	}

	const Program& self(void) const { return *this; }
public:
	ProgramUniform<GLfloat> threshold, grid_step;
	ProgramUniform<Mat4f> transform_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniformSampler volume_tex;

	VolumeProgram(void)
	 : Program(make())
	 , threshold(self(), "Threshold")
	 , grid_step(self(), "GridStep")
	 , transform_matrix(self(), "TransformMatrix")
	 , camera_position(self(), "CameraPosition")
	 , light_position(self(), "LightPosition")
	 , volume_tex(self(), "VolumeTex")
	{ }
};

class Grid
{
protected:
	const GLuint grid_div;
	shapes::Tetrahedrons make_grid;
	shapes::DrawingInstructions grid_instr;
	shapes::Tetrahedrons::IndexArray grid_indices;

	Context gl;

	// A vertex array object for the rendered shape
	VertexArray vao;

	// VBO for the grids's vertex positions
	Buffer positions, indices;

public:
	Grid(const Program& prog, float quality)
	 : grid_div(64 + (quality > 0.75 ? 64 : 0))
	 , make_grid(1.0, grid_div)
	 , grid_instr(make_grid.Instructions())
	 , grid_indices(make_grid.Indices())
	{
		// bind the VAO for the shape
		vao.Bind();

		std::vector<GLfloat> data;
		// bind the VBO for the grid vertex positions
		positions.Bind(Buffer::Target::Array);
		GLuint n_per_vertex = make_grid.Positions(data);
		// upload the data
		Buffer::Data(Buffer::Target::Array, data);
		// setup the vertex attribs array
		VertexArrayAttrib attr(prog, "Position");
		attr.Setup<GLfloat>(n_per_vertex);
		attr.Enable();

		// bind the VBO for the tetrahedron indices
		indices.Bind(Buffer::Target::ElementArray);
		// upload them
		Buffer::Data(Buffer::Target::ElementArray, grid_indices);
		grid_indices.clear();
	}

	float Step(void) const
	{
		return 1.0f / grid_div;
	}

	void Use(void)
	{
		vao.Bind();
	}

	void Draw(void)
	{
		grid_instr.Draw(grid_indices);
	}
};

class VolumeExample : public Example
{
private:

	// wrapper around the current OpenGL context
	Context gl;

	VolumeProgram volume_prog;

	Grid grid;

	GLuint width, height;

	// A 3D texture containing density data
	Texture volume_tex;
public:
	VolumeExample(const ExampleParams& params)
	 : volume_prog()
	 , grid(volume_prog, params.quality)
	{
		volume_prog.grid_step = grid.Step();

		std::srand(3456);
		Texture::Active(0);
		volume_prog.volume_tex = 0;
		gl.Bound(Texture::Target::_3D, volume_tex)
			.Filter(TextureFilter::Linear)
			.BorderColor(Vec4f(0.0f, 0.0f, 0.0f, 0.0f))
			.Wrap(TextureWrap::ClampToBorder)
			.Image3D(
				images::Cloud(
					128, 128, 128,
					Vec3f(0.0f, 0.0f, 0.0f),
					0.5f, 0.5f, 0.5f, 0.1f
				)
			);

		const Vec3f light_position(12.0, 1.0, 8.0);
		volume_prog.light_position = light_position;

		grid.Use();
		volume_prog.Use();

		gl.ClearColor(0.8f, 0.7f, 0.6f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(FaceOrientation::CW);
		gl.CullFace(Face::Back);
	}


	void Render(double time)
	{
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();
		//

		Mat4f perspective = CamMatrixf::PerspectiveX(
			Degrees(65),
			width, height,
			1, 30
		);

		auto camera = CamMatrixf::Orbiting(
			Vec3f(0, 0, 0),
			GLfloat(2.0 - SineWave(time / 14.0) * 0.2),
			FullCircles(time / 19.0),
			Degrees(45 + SineWave(time / 17.0) * 40)
		);
		Vec3f camera_position = camera.Position();

		auto model = ModelMatrixf::Translation(-0.5, -0.5, -0.5);

		volume_prog.camera_position = camera_position;
		volume_prog.transform_matrix = perspective*camera*model;
		volume_prog.threshold = GLfloat(0.5 + SineWave(time / 7.0) * 0.4);

		grid.Draw();
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
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

std::unique_ptr<Example> makeExample(const ExampleParams& params)
{
	return std::unique_ptr<Example>(new VolumeExample(params));
}

} // namespace oglplus
