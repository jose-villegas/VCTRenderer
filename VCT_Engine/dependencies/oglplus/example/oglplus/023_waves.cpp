/**
 *  @example oglplus/023_waves.cpp
 *  @brief Shows tessellation and dynamic deformation of a plane
 *
 *  @oglplus_screenshot{023_waves}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_tessellation_shader}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/plane.hpp>
#include <oglplus/images/squares.hpp>
#include <oglplus/bound/texture.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class WavesExample : public Example
{
private:
	shapes::Plane make_plane;
	shapes::DrawingInstructions plane_instr;
	shapes::Plane::IndexArray plane_indices;

	Context gl;

	Program prog;

	Lazy<Uniform<Mat4f>> camera_matrix;
	Lazy<Uniform<Vec3f>> camera_position;
	Lazy<Uniform<GLfloat>> anim_time;

	VertexArray plane;

	Buffer verts;

	Texture env_map;

	int prev_period;
public:
	WavesExample(void)
	 : make_plane(
		Vec3f(  0.0f,   0.0f,   0.0f),
		Vec3f(100.0f,   0.0f,   0.0f),
		Vec3f(  0.0f,   0.0f,-100.0f),
		50, 50
	), plane_instr(make_plane.Instructions(shapes::DrawMode::Patches()))
	 , plane_indices(make_plane.Indices(shapes::DrawMode::Patches()))
	 , camera_matrix(prog, "CameraMatrix")
	 , camera_position(prog, "CameraPosition")
	 , anim_time(prog, "Time")
	 , prev_period(-1)
	{
		VertexShader vs(ObjectDesc("Vertex"));
		vs.Source(
			"#version 150\n"

			"uniform vec3 CameraPosition;"

			"in vec3 Position;"

			"out vec3 vertPosition;"
			"out float vertDistance;"

			"void main(void)"
			"{"
			"	vertPosition = Position;"
			"	vertDistance = distance(CameraPosition, Position);"
			"}"
		);
		prog.AttachShader(vs);

		TessControlShader cs(ObjectDesc("TessControl"));
		cs.Source(
			"#version 150\n"
			"#extension ARB_tessellation_shader : enable\n"

			"layout(vertices = 3) out;"

			"in vec3 vertPosition[];"
			"in float vertDistance[];"

			"out vec3 tecoPosition[];"

			"int tessLevel(float dist)"
			"{"
			"	return clamp(int(150.0 / (dist+0.1)), 1, 10);"
			"}"

			"void main(void)"
			"{"
			"	tecoPosition[gl_InvocationID] ="
			"		vertPosition[gl_InvocationID];"

			"	if(gl_InvocationID == 0)"
			"	{"
			"		gl_TessLevelInner[0] = tessLevel(("
			"			vertDistance[0]+"
			"			vertDistance[1]+"
			"			vertDistance[2] "
			"		)*0.333);"
			"		gl_TessLevelOuter[0] = tessLevel(("
			"			vertDistance[1]+"
			"			vertDistance[2] "
			"		)*0.5);"
			"		gl_TessLevelOuter[1] = tessLevel(("
			"			vertDistance[2]+"
			"			vertDistance[0] "
			"		)*0.5);"
			"		gl_TessLevelOuter[2] = tessLevel(("
			"			vertDistance[0]+"
			"			vertDistance[1] "
			"		)*0.5);"
			"	}"
			"}"
		);
		prog.AttachShader(cs);

		TessEvaluationShader es(ObjectDesc("TessEvaluation"));
		es.Source(
			"#version 150\n"
			"#extension ARB_tessellation_shader : enable\n"
			"#define MaxWaves 5\n"

			"layout(triangles, equal_spacing, ccw) in;"

			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"uniform vec3 LightPosition;"
			"uniform vec3 CameraPosition;"

			"uniform float Time;"

			"uniform int WaveCount;"
			"uniform vec3 WaveDirections[MaxWaves];"
			"uniform vec3 WaveDimensions[MaxWaves];"

			"in vec3 tecoPosition[];"

			"out vec3 teevNormal;"
			"out vec3 teevLightDir;"
			"out vec3 teevViewDir;"

			"out float teevDistance;"

			"void main(void)"
			"{"
			"	const vec3 Up = vec3(0.0, 1.0, 0.0);"
			"	vec3 Position ="
			"		gl_TessCoord.x * tecoPosition[0]+"
			"		gl_TessCoord.y * tecoPosition[1]+"
			"		gl_TessCoord.z * tecoPosition[2];"

			"	vec3 Pos = Position;"
			"	vec3 Nml = Up;"
			"	for(int wave=0; wave!=WaveCount; ++wave)"
			"	{"
			"		vec3 Dir = WaveDirections[wave];"
			"		vec3 Dim = WaveDimensions[wave];"
			"		float Dist = dot(Position, Dir);"

			"		float u = Dim.y*sin(Dist/Dim.x + Time*Dim.z);"
			"		Pos += Up * u;"

			"		float w = (Dim.y/Dim.x)*cos(Dist/Dim.x + Time*Dim.z);"
			"		Nml -= Dir * w;"

			"		float d = -0.125*Dim.x*sin(2.0*Dist/Dim.x + Time*Dim.z);"
			"		Pos += Dir * d;"
			"	}"

			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		vec4(Pos, 1.0);"

			"	teevNormal = normalize(Nml);"
			"	teevLightDir = normalize(LightPosition - Pos);"
			"	teevViewDir = normalize(CameraPosition - Pos);"
			"	teevDistance = distance(CameraPosition, Pos);"
			"}"
		);
		prog.AttachShader(es);

		FragmentShader fs(ObjectDesc("Fragment"));
		fs.Source(
			"#version 150\n"

			"uniform samplerCube EnvMap;"

			"in vec3 teevNormal;"
			"in vec3 teevLightDir;"
			"in vec3 teevViewDir;"
			"in float teevDistance;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float Dim = clamp(30.0/teevDistance, 0.0, 1.0);"
			"	float LightRefl = dot(reflect(-teevLightDir, teevNormal), teevViewDir);"
			"	float LightHit = dot(teevNormal, teevLightDir);"

			"	float Diffuse = clamp(LightHit+0.1, 0.0, 1.0);"
			"	float Specular = pow(clamp(LightRefl, 0.0, 0.91), 32);"

			"	vec3 Environ=texture(EnvMap,reflect(-teevViewDir, teevNormal)).rgb;"
			"	vec3 WaterColor = vec3(0.4, 0.5, 0.5);"
			"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"
			"	vec3 FogColor = vec3(0.9, 0.9, 0.9);"

			"	vec3 WaveColor ="
			"		LightColor*Specular+"
			"		WaterColor*Diffuse+"
			"		Environ*0.02;"

			"	fragColor = mix(WaveColor, FogColor, 1.0-Dim);"
			"}"
		);
		prog.AttachShader(fs);

		prog.Build();
		gl.Use(prog);

		gl.Bind(plane);
		gl.Bind(Buffer::Target::Array, verts);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_plane.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		Uniform<Vec3f>(prog, "LightPosition").Set(-100, 100, 20);

		Uniform<Vec3f> wave_directions(prog, "WaveDirections");
		Uniform<Vec3f> wave_dimensions(prog, "WaveDimensions");
		Uniform<GLint>(prog, "WaveCount").Set(5);

		wave_directions[0] = Normalized(Vec3f(1.0f, 0.0f, 1.0f));
		wave_dimensions[0] = Vec3f(5.0f, 1.5f, 1.2f);

		wave_directions[1] = Normalized(Vec3f(1.0f, 0.0f, 0.5f));
		wave_dimensions[1] = Vec3f(4.0f, 0.8f, 1.2f);

		wave_directions[2] = Normalized(Vec3f(1.0f, 0.0f, 0.1f));
		wave_dimensions[2] = Vec3f(2.0f, 0.5f, 2.4f);

		wave_directions[3] = Normalized(Vec3f(1.0f, 0.0f,-0.1f));
		wave_dimensions[3] = Vec3f(1.5f, 0.2f, 3.7f);

		wave_directions[4] = Normalized(Vec3f(1.0f, 0.0f, 0.4f));
		wave_dimensions[4] = Vec3f(1.1f, 0.2f, 4.7f);

		Texture::Active(0);
		{
			auto image = images::Squares(512, 512, 0.9f, 16, 16);
			gl.Bind(Texture::Target::CubeMap, env_map);

			for(int i=0; i!=6; ++i)
				Texture::Image2D(Texture::CubeMapFace(i), image);

			gl.Current(Texture::Target::CubeMap)
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::ClampToEdge)
				.WrapT(TextureWrap::ClampToEdge)
				.WrapR(TextureWrap::ClampToEdge)
				.SwizzleG(TextureSwizzle::Red)
				.SwizzleB(TextureSwizzle::Red)
				.GenerateMipmap();
		}
		UniformSampler(prog, "EnvMap").Set(0);

		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		Uniform<Mat4f>(prog, "ProjectionMatrix").Set(
			CamMatrixf::PerspectiveX(
				Degrees(75),
				float(width)/height,
				1, 150
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		int period = int(time * 0.125);
		if(prev_period < period)
		{
			if(period % 2)
				gl.PolygonMode(PolygonMode::Line);
			else gl.PolygonMode(PolygonMode::Fill);
			prev_period = period;
		}

		auto camera = CamMatrixf::Orbiting(
			Vec3f(0.0f, 2.0f, 0.0f),
			GLfloat(17.0 - CosineWave(time / 31.0) * 10.0),
			FullCircles(time / 43.0f),
			Degrees(45 - SineWave(time / 29.0f) * 35)
		);
		camera_matrix.Set(camera);
		camera_position.Set(camera.Position());

		anim_time.Set(GLfloat(time));

		plane_instr.Draw(plane_indices);
	}

	bool Continue(double time)
	{
		return time < 90.0;
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
	return std::unique_ptr<Example>(new WavesExample);
}

} // namespace oglplus
