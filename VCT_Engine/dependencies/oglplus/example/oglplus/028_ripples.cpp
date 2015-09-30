/**
 *  @example oglplus/028_ripples.cpp
 *  @brief Shows implicit surface polygonization
 *
 *  @oglplus_screenshot{028_ripples}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/shapes/tetrahedrons.hpp>

#include <oglplus/images/newton.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class LiquidVertShader
 : public VertexShader
{
public:
	LiquidVertShader(void)
	 : VertexShader(
		ObjectDesc("Liquid vertex shader"),
		StrCRef("#version 150\n"
		"uniform vec3 GridOffset;"
		"uniform float Time;"

		"in vec4 Position;"

		"out vec3 vertNormal;"
		"out float vertValue;"
		"const vec4 Source[4] = vec4[4]("
		"	vec4( 0.2, 0.3, 0.1, 0.1),"
		"	vec4(-0.5, 0.1,-0.1, 0.2),"
		"	vec4(-0.1, 0.2,-0.5, 0.1),"
		"	vec4(-0.2, 0.2,-0.4, 0.2) "
		");"

		"void main(void)"
		"{"
		"	gl_Position = Position + vec4(GridOffset, 0.0);"
		"	vertValue = -gl_Position.y;"
		"	vertNormal = vec3(0.0, 1.0, 0.0);"
		"	for(int s=0; s!=4; ++s)"
		"	{"
		"		float x = gl_Position.x - Source[s].x;"
		"		float z = gl_Position.z - Source[s].z;"

		"		float a = Source[s].y;"
		"		float w = Source[s].w*50.0;"
		"		float r = x*x + z*z;"
		"		float t = r + s - Time*(0.5 - 0.1*s);"
		"		float g = w * t;"
		"		float d = 2.0*a*exp(-r-1.0)*(w*cos(g)-sin(g));"

		"		vertValue += sin(g)*exp(-r-1.0)*a;"
		"		vertNormal += vec3(x*d, 0.0, z*d);"
		"	}"
		"}")
	)
	{ }
};

class LiquidGeomShader
 : public GeometryShader
{
public:
	LiquidGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Liquid geometry shader"),
		StrCRef("#version 150\n"
		"layout(triangles_adjacency) in;"
		"layout(triangle_strip, max_vertices = 4) out;"

		"uniform mat4 CameraMatrix;"
		"uniform vec3 CameraPosition, LightPosition;"

		"in vec3 vertNormal[];"
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
		"		vertNormal[i1],"
		"		vertNormal[i2],"
		"		t"
		"	);"
		"	geomLightDir = LightPosition - gl_Position.xyz;"
		"	geomViewDir = CameraPosition - gl_Position.xyz;"
		"	gl_Position = CameraMatrix * gl_Position;"
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

class LiquidFragShader
 : public FragmentShader
{
public:
	LiquidFragShader(void)
	 : FragmentShader(
		ObjectDesc("Liquid fragment shader"),
		StrCRef("#version 150\n"

		"uniform samplerCube EnvMap;"

		"in vec3 geomNormal, geomLightDir, geomViewDir;"

		"out vec3 fragColor;"

		"void main(void)"
		"{"
		"	vec3 Normal = normalize(geomNormal);"
		"	vec3 LightDir = normalize(geomLightDir);"
		"	vec3 ViewDir = normalize(geomViewDir);"

		"	float LightRefl = dot(reflect(-LightDir, Normal), ViewDir);"
		"	float LightHit = dot(Normal, LightDir);"

		"	float Specular = pow(clamp(LightRefl+0.1, 0.0, 1.0), 32);"

		"	float Diffuse1 = pow(max(LightHit*0.6+0.4, 0.0), 2.0);"
		"	float Diffuse2 = sqrt(max(LightHit+0.2, 0.0));"
		"	float Diffuse = Diffuse1 * 0.8 + Diffuse2 * 0.2;"

		"	float ViewLight = max(0.3-dot(ViewDir, LightDir), 0.0);"

		"	float Ambient = 0.9;"

		"	vec3 Environ = texture(EnvMap, reflect(-ViewDir, Normal)).rgb;"
		"	fragColor = "
		"		Environ * Ambient +"
		"		vec3(0.4, 0.8, 0.4) * Diffuse+"
		"		vec3(0.2, 0.2, 0.2) * Specular;"
		"}")
	)
	{ }
};

class LiquidProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(LiquidVertShader());
		prog.AttachShader(LiquidGeomShader());
		prog.AttachShader(LiquidFragShader());
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix;
	ProgramUniform<Vec3f> grid_offset, camera_position, light_position;
	ProgramUniform<GLfloat> time;

	LiquidProgram(void)
	 : Program(make())
	 , camera_matrix(prog(), "CameraMatrix")
	 , grid_offset(prog(), "GridOffset")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	 , time(prog(), "Time")
	{ }
};

class Grid
{
protected:
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
	 : make_grid(1.0, unsigned(16 + quality*quality*64))
	 , grid_instr(make_grid.Instructions())
	 , grid_indices(make_grid.Indices())
	{
		// bind the VAO for the shape
		vao.Bind();

		std::vector<GLfloat> data;
		// bind the VBO for the positions
		positions.Bind(Buffer::Target::Array);
		GLuint n_per_vertex = make_grid.Positions(data);
		// upload the data
		Buffer::Data(Buffer::Target::Array, data);
		// setup the vertex attribs array
		VertexArrayAttrib attr(prog, "Position");
		attr.Setup<GLfloat>(n_per_vertex);
		attr.Enable();

		// bind the VBO for the indices
		indices.Bind(Buffer::Target::ElementArray);
		// upload them
		Buffer::Data(Buffer::Target::ElementArray, grid_indices);
		// clear the indexs buffer
		grid_indices.clear();
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

class LiquidExample : public Example
{
private:

	// wrapper around the current OpenGL context
	Context gl;

	LiquidProgram liquid_prog;

	Grid grid;

	Mat4f perspective;

	Texture env_map;

	const int grid_repeat;
public:
	LiquidExample(const ExampleParams& params)
	 : liquid_prog()
	 , grid(liquid_prog, params.quality)
	 , grid_repeat(int(1 + params.quality*2))
	{
		Texture::Active(0);
		{
			auto image = images::NewtonFractal(
				256, 256,
				Vec3f(0.1f, 0.1f, 0.1f),
				Vec3f(1.0f, 1.0f, 1.0f),
				Vec2f(-1.0f, -1.0f),
				Vec2f( 1.0f,  1.0f),
				images::NewtonFractal::X4Minus1(),
				images::NewtonFractal::DefaultMixer()
			);
			auto bound_tex = oglplus::Context::Bound(
				Texture::Target::CubeMap,
				env_map
			);
			for(int i=0; i!=6; ++i)
				Texture::ImageCM(i, image);
			bound_tex.GenerateMipmap();
			bound_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::ClampToEdge);
			bound_tex.WrapT(TextureWrap::ClampToEdge);
			bound_tex.WrapR(TextureWrap::ClampToEdge);
		}
		ProgramUniformSampler(liquid_prog, "EnvMap").Set(0);

		const Vec3f light_position(12.0, 1.0, 8.0);
		liquid_prog.light_position.Set(light_position);

		gl.ClearColor(0.7f, 0.65f, 0.55f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(FaceOrientation::CW);
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		perspective = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 100
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		liquid_prog.time = GLfloat(time);


		auto camera = CamMatrixf::Orbiting(
			Vec3f(0, 0, 0),
			GLfloat(4.5 - SineWave(time / 14.0)),
			FullCircles(time / 26.0),
			Degrees(55 + SineWave(time / 14.0) * 30)
		);
		Vec3f camera_position = camera.Position();

		liquid_prog.camera_position = camera_position;
		liquid_prog.camera_matrix = perspective*camera;

		for(int z=-grid_repeat; z!=grid_repeat; ++z)
		for(int x=-grid_repeat; x!=grid_repeat; ++x)
		{
			liquid_prog.grid_offset.Set(x, -0.5f, z);
			grid.Draw();
		}

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
	return std::unique_ptr<Example>(new LiquidExample(params));
}

} // namespace oglplus
