/**
 *  @example oglplus/029_waves.cpp
 *  @brief Shows implicit surface polygonization
 *
 *  @oglplus_screenshot{029_waves}
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

#include <oglplus/images/squares.hpp>

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
		"flat out int vertSign;"
		"const vec4 Source[4] = vec4[4]("
		"	vec4(-3.3, 0.1, 1.9, 0.21),"
		"	vec4(-3.9, 0.2,-2.1, 0.15),"
		"	vec4(-3.3, 0.2, 3.9, 0.17),"
		"	vec4(-2.6, 0.3,-3.9, 0.11) "
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
		"		float w = Source[s].w*8.0;"
		"		float r = x*x + z*z;"
		"		float t = r + s - (Time*1.9 - 0.2*s);"
		"		float g = w * t;"
		"		float d = a*exp((-r-1.0)*0.17)*(w*cos(g)-sin(g));"

		"		vertValue += sin(g)*exp((-r-1.0)*0.17)*a;"
		"		vertNormal += vec3(x*d, 0.0, z*d);"
		"	}"
		"	vertSign = vertValue < 0.0 ? 0 : 1;"
		"}")
	)
	{ }
};

class LiquidGeomShader
 : public GeometryShader
{
private:
	Texture _configurations;
public:
	LiquidGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Liquid geometry shader"),
		StrCRef("#version 150\n"
		"layout(triangles_adjacency) in;"
		"layout(triangle_strip, max_vertices = 4) out;"

		"uniform isampler1D Configurations;"
		"uniform mat4 CameraMatrix;"
		"uniform vec3 CameraPosition, LightPosition;"

		"in vec3 vertNormal[];"
		"in float vertValue[];"
		"flat in int vertSign[];"

		"out vec3 geomNormal, geomLightDir, geomViewDir;"

		"void make_vertex(int i1, int i2)"
		"{"
		"	float t = vertValue[i1]/(vertValue[i1] - vertValue[i2]);"
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

		"void make_triangle(const ivec4 v1, const ivec4 v2)"
		"{"
		"	make_vertex(v1.x, v2.x);"
		"	make_vertex(v1.y, v2.y);"
		"	make_vertex(v1.z, v2.z);"
		"	EndPrimitive();"
		"}"

		"void make_quad(const ivec4 v1, const ivec4 v2)"
		"{"
		"	make_vertex(v1.x, v2.x);"
		"	make_vertex(v1.y, v2.y);"
		"	make_vertex(v1.z, v2.z);"
		"	make_vertex(v1.w, v2.w);"
		"	EndPrimitive();"
		"}"

		"void process_tetrahedron(int a, int b, int c, int d)"
		"{"
		"	ivec4 i = ivec4(vertSign[a],vertSign[b],vertSign[c],vertSign[d]);"
		"	int si = int(dot(i, ivec4(1, 1, 1, 1))) % 4;"
		"	if(si != 0)"
		"	{"
		"		int iv = int(dot(i, ivec4(16, 8, 4, 2)));"
		"		ivec4 v1 = texelFetch(Configurations, iv+0, 0);"
		"		ivec4 v2 = texelFetch(Configurations, iv+1, 0);"
		"		if(si % 2 == 0) make_quad(v1, v2);"
		"		else make_triangle(v1, v2);"
		"	}"
		"}"

		"void main(void)"
		"{"
		"	process_tetrahedron(0, 2, 4, 1);"
		"}")
	)
	{
		Texture::Active(0);
		{
			const GLubyte a = 0x0, b = 0x2, c = 0x4, d = 0x1, x = 0xFF;
			const GLubyte  tex_data[] = {
				x, x, x, x,   x, x, x, x,
				a, c, b, x,   d, d, d, x,
				b, d, a, x,   c, c, c, x,
				b, b, a, a,   c, d, c, d,
				a, d, c, x,   b, b, b, x,
				a, a, c, c,   b, d, b, d,
				a, a, d, d,   c, b, c, b,
				a, a, a, x,   b, d, c, x,
				c, d, b, x,   a, a, a, x,
				c, c, b, b,   a, d, a, d,
				b, d, b, d,   c, c, a, a,
				b, b, b, x,   c, d, a, x,
				c, d, c, d,   a, a, b, b,
				c, c, c, x,   a, d, b, x,
				d, d, d, x,   a, b, c, x,
				x, x, x, x,   x, x, x, x
			};
			oglplus::Context::Bound(Texture::Target::_1D, _configurations)
				.Image1D(
					0,
					PixelDataInternalFormat::RGBA8UI,
					sizeof(tex_data),
					0,
					PixelDataFormat::RGBAInteger,
					PixelDataType::UnsignedByte,
					tex_data
				)
				.MinFilter(TextureMinFilter::Nearest)
				.MagFilter(TextureMagFilter::Nearest)
				.WrapS(TextureWrap::ClampToEdge);
		}
	}
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

		"	vec3 Environ = texture(EnvMap, reflect(-ViewDir, Normal)).rgb;"
		"	fragColor = "
		"		Environ * 0.1 +"
		"		vec3(0.4, 0.4, 0.8) * Diffuse+"
		"		vec3(0.2, 0.2, 0.3) * Specular;"
		"}")
	)
	{ }
};

struct LiquidProgramShaders
{
	LiquidVertShader vertex;
	LiquidGeomShader geometry;
	LiquidFragShader fragment;
};

class LiquidProgram
 : public LiquidProgramShaders
 , public Program
{
private:
	const LiquidProgramShaders& shaders(void) const { return *this; }
	static Program make(const LiquidProgramShaders& shaders)
	{
		Program prog;
		prog.AttachShader(shaders.vertex);
		prog.AttachShader(shaders.geometry);
		prog.AttachShader(shaders.fragment);
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix;
	ProgramUniform<Vec3f> grid_offset, camera_position, light_position;
	ProgramUniform<GLfloat> time;
	ProgramUniformSampler configurations;

	LiquidProgram(void)
	 : Program(make(shaders()))
	 , camera_matrix(prog(), "CameraMatrix")
	 , grid_offset(prog(), "GridOffset")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	 , time(prog(), "Time")
	 , configurations(prog(), "Configurations")
	{
		configurations = 0;
	}
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
	 : make_grid(1.0f, unsigned(16 + quality*quality*64))
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
		Texture::Active(1);
		{
			auto image = images::Squares(512, 512, 0.9f, 8, 8);
			auto bound_tex = gl.Bound(Texture::Target::CubeMap, env_map);
			for(int i=0; i!=6; ++i)
				Texture::ImageCM(i, image);
			bound_tex.GenerateMipmap();
			bound_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::ClampToEdge);
			bound_tex.WrapT(TextureWrap::ClampToEdge);
			bound_tex.WrapR(TextureWrap::ClampToEdge);
			bound_tex.SwizzleG(TextureSwizzle::Red);
			bound_tex.SwizzleB(TextureSwizzle::Red);
		}
		ProgramUniformSampler(liquid_prog, "EnvMap").Set(1);

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
			Degrees(65),
			width, height,
			1, 100
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		liquid_prog.time = GLfloat(time);

		auto camera = CamMatrixf::Orbiting(
			Vec3f(0, 0, 0),
			GLfloat(5.0 - SineWave(time / 31.0)),
			FullCircles(time / 71.0),
			Degrees(55 + SineWave(time / 27.0) * 30)
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
