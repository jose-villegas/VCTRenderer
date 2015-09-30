/**
 *  @example oglplus/027_smoke_trails.cpp
 *  @brief Shows how to draw multpile lighted smoke trails
 *
 *  @oglplus_screenshot{027_smoke_trails}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_cpp_feat{INITIALIZER_LISTS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/images/cloud.hpp>

#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#include "example.hpp"

namespace oglplus {

// Encapsulates a particle emitter
class ParticleSystem
{
private:
	const CubicBezierLoop<Vec3f, double> path;
	const double cycle_time;
	const double lifetime;
	const double spawn_interval;
	double spawn_time;

	std::vector<Vec3f> positions;
	std::vector<Vec3f> directions;
	std::vector<float> ages;
	std::vector<int> ids;

	// Creates a directional vector for a new particle
	static Vec3f NewDirection(void)
	{
		float disp = 0.6f;
		float dx = (0.5f - (float(std::rand())/RAND_MAX))*disp;
		float dy = (0.5f - (float(std::rand())/RAND_MAX))*disp;
		float dz = (0.5f - (float(std::rand())/RAND_MAX))*disp;
		return Vec3f(dx, dy, dz);
	}

	// Returns a new particle identifier
	static int NewID(void)
	{
		return rand();
	}

	// Spawns a new particle if the time is right
	bool SpawnParticle(
		double time,
		Vec3f& position,
		Vec3f& direction,
		float& age,
		int& id
	)
	{
		float new_age = float(time - spawn_time - spawn_interval);
		if(new_age >= 0.0f)
		{
			spawn_time += spawn_interval;
			direction = NewDirection();
			Vec3f emitter_pos = path.Position(spawn_time/cycle_time);
			position = emitter_pos + direction;
			age = new_age;
			id = NewID();
			return true;
		}
		return false;
	}
public:
	ParticleSystem(
		const std::initializer_list<Vec3f>& path_points,
		double path_time,
		double part_per_sec
	): path(path_points)
	 , cycle_time(path_time)
	 , lifetime(10.0)
	 , spawn_interval(1.0 / part_per_sec)
	 , spawn_time(0.0)
	{
		assert(cycle_time > 0.0);
	}

	// Updates the emitter
	// Changes its position, emits new particles
	void Update(ExampleClock& clock)
	{
		assert(positions.size() == directions.size());
		assert(positions.size() == ages.size());
		assert(positions.size() == ids.size());

		double time_diff = clock.Interval().Seconds();
		double drag = 0.1 * time_diff;
		if(drag > 1.0) drag = 1.0;

		// go through the existing particles
		for(std::size_t i=0, n=positions.size(); i!=n; ++i)
		{
			// update the age
			ages[i] += time_diff / lifetime;
			// if the particle is "too old"
			if(ages[i] > 1.0)
			{
				// try to spawn a new one in its place
				SpawnParticle(
					clock.Now().Seconds(),
					positions[i],
					directions[i],
					ages[i],
					ids[i]
				);
			}
			else
			{
				// otherwise just update its motion
				directions[i] *= float(1.0 - drag);
				positions[i] += directions[i] * float(time_diff);
			}
		}
		Vec3f position;
		Vec3f direction;
		float age;
		int id;
		// spawn new particles if necessary
		while(SpawnParticle(clock.Now().Seconds(), position, direction, age, id))
		{
			positions.push_back(position);
			directions.push_back(direction);
			ages.push_back(age);
			ids.push_back(id);
		}
	}

	void Upload(
		std::vector<Vec3f>& pos,
		std::vector<float>& age,
		std::vector<int>& id
	)
	{
		pos.insert(pos.end(), positions.begin(), positions.end());
		age.insert(age.end(), ages.begin(), ages.end());
		id.insert(id.end(), ids.begin(), ids.end());
	}
};

class SmokeExample : public Example
{
private:
	// A list of particle emitters
	std::vector<ParticleSystem> emitters;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex, geometry and fragment shader
	Shader vs, gs, fs;

	// Program
	Program prog;

	// Uniforms
	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix;
	Lazy<Uniform<Vec3f>> light_cam_pos;

	// A vertex array object for the particles
	VertexArray particles;

	// VBOs for the particle positions, ages and identifiers
	Buffer pos_buf, age_buf, id_buf;

	// The smoke texture for the particles
	Texture smoke_tex;

	std::vector<Vec3f> positions;
	std::vector<float> ages;
	std::vector<int> ids;
public:
	SmokeExample(void)
	 : emitters(
		{
			{
				{
					{-20.0f, -10.0f,  10.0f},
					{ 20.0f,   0.0f, -20.0f},
					{ 20.0f,  10.0f,  20.0f},
					{-20.0f,   0.0f, -10.0f}
				}, 15.0, 200.0
			},
			{
				{
					{ 30.0f,   0.0f,   5.0f},
					{-30.0f,   0.0f,  -5.0f},
					{-20.0f,  20.0f,   5.0f},
					{ 20.0f, -10.0f,  -5.0f}
				}, 17.0, 200.0
			},
		}
	), vs(ShaderType::Vertex, ObjectDesc("Vertex"))
	 , gs(ShaderType::Geometry, ObjectDesc("Geometry"))
	 , fs(ShaderType::Fragment, ObjectDesc("Fragment"))
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , light_cam_pos(prog, "LightCamPos")
	{
		// Set the vertex shader source
		vs.Source(
			"#version 150\n"
			"uniform mat4 CameraMatrix;"
			"in vec4 Position;"
			"in float Age;"
			"in int Id;"
			"out float vertAge;"
			"out int vertId;"
			"void main(void)"
			"{"
			"	gl_Position = CameraMatrix * Position;"
			"	vertAge = Age;"
			"	vertId = Id;"
			"}"
		);
		// compile it
		vs.Compile();

		// Set the geometry shader source
		gs.Source(
			"#version 150\n"
			"layout(points) in;"
			"layout(triangle_strip, max_vertices = 4) out;"
			"uniform vec3 LightCamPos;"
			"uniform mat4 ProjectionMatrix;"
			"in float vertAge[];"
			"in int vertId[];"
			"out vec2 geomTexCoord;"
			"out float geomAge;"
			"out float geomLightVal;"
			"out float geomLightBias;"
			"void main(void)"
			"{"
			"	if(vertAge[0] > 1.0) return;"
			"	vec3 pos = gl_in[0].gl_Position.xyz;"
			"	vec3 lightDir = normalize(LightCamPos - pos);"
			"	float s = 0.8, g = 3.0;"
			"	float yo[2] = float[2](-1.0, 1.0);"
			"	float xo[2] = float[2](-1.0, 1.0);"
			"	float angle = vertId[0];"
			"	float cx = cos(angle), sx = sin(angle);"
			"	mat2 rot = mat2(cx, sx, -sx, cx);"
			"	for(int j=0;j!=2;++j)"
			"	for(int i=0;i!=2;++i)"
			"	{"
			"		float xoffs = xo[i]*(1.0+vertAge[0]*g)*s;"
			"		float yoffs = yo[j]*(1.0+vertAge[0]*g)*s;"
			"		vec2 offs = rot*vec2(xoffs, yoffs);"
			"		gl_Position = ProjectionMatrix * vec4("
			"			pos.x-offs.x,"
			"			pos.y-offs.y,"
			"			pos.z,"
			"			1.0"
			"		);"
			"		geomTexCoord = vec2(float(i), float(j));"
			"		geomAge = vertAge[0];"
			"		geomLightVal = lightDir.z;"
			"		geomLightBias = -dot("
			"			normalize(vec3(offs, 0.0)),"
			"			lightDir"
			"		);"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		// compile it
		gs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 150\n"
			"uniform sampler2D SmokeTex;"
			"in vec2 geomTexCoord;"
			"in float geomAge;"
			"in float geomLightVal;"
			"in float geomLightBias;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec3 c = texture(SmokeTex, geomTexCoord).rgb;"
			"	float depth = c.g - c.r;"
			"	if(depth == 0.0) discard;"
			"	float density = min(depth * c.b * 2.0, 1.0);"
			"	float intensity = min("
			"		max("
			"			geomLightVal*0.5+"
			"			geomLightBias,"
			"			0.0"
			"		)+max("
			"			-geomLightVal*"
			"			(1.0 - density)*"
			"			geomLightBias * 5.0,"
			"			0.0"
			"		),"
			"		1.0"
			"	) + 0.1;"
			"	fragColor = vec4("
			"		intensity,"
			"		intensity,"
			"		intensity,"
			"		(1.0 - geomAge)*density"
			"	);"
			"}"
		);
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(gs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the particles
		particles.Bind();

		// bind the VBO for the particle positions
		pos_buf.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, positions);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<Vec3f>();
			attr.Enable();
		}

		// bind the VBO for the particle ages
		age_buf.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, ages);
			VertexArrayAttrib attr(prog, "Age");
			attr.Setup<GLfloat>();
			attr.Enable();
		}

		// bind the VBO for the particle identifiers
		id_buf.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, ids);
			VertexArrayAttrib attr(prog, "Id");
			attr.Setup<GLint>();
			attr.Enable();
		}

		Texture::Active(0);
		UniformSampler(prog, "SmokeTex").Set(0);
		gl.Bound(Texture::Target::_2D, smoke_tex)
			.MinFilter(TextureMinFilter::LinearMipmapLinear)
			.MagFilter(TextureMagFilter::Linear)
			.BorderColor(Vec4f(0.0f, 0.0f, 0.0f, 0.0f))
			.WrapS(TextureWrap::ClampToBorder)
			.WrapT(TextureWrap::ClampToBorder)
			.Image2D(
				images::Cloud2D(
					images::Cloud(
						128, 128, 128,
						Vec3f(0.1f, -0.5f, 0.3f),
						0.5f
					)
				)
			).GenerateMipmap();
		//
		gl.ClearColor(0.0f, 0.1f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::Blend);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(75),
				float(width)/height,
				1, 100
			)
		);
	}

	void Render(ExampleClock& clock)
	{
		positions.clear();
		ages.clear();
		ids.clear();

		// update the emitters and get the particle data
		for(auto i=emitters.begin(), e=emitters.end(); i!=e; ++i)
		{
			i->Update(clock);
			i->Upload(positions, ages, ids);
		}
		assert(positions.size() == ages.size());
		assert(positions.size() == ids.size());

		// make a camera matrix
		auto cameraMatrix = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(38.0 - SineWave(clock.Now().Seconds() / 6.0) * 10.0),
			FullCircles(clock.Now().Seconds() * 0.1),
			Degrees(SineWave(clock.Now().Seconds() / 20.0) * 60)
		);

		std::vector<float> depths(positions.size());
		std::vector<GLuint> indices(positions.size());
		// calculate the depths of the particles
		for(std::size_t i=0, n=positions.size(); i!=n; ++i)
		{
			depths[i] = (cameraMatrix * Vec4f(positions[i], 1.0)).z();
			indices[i] = GLuint(i);
		}

		// sort the indices by the depths
		std::sort(
			indices.begin(),
			indices.end(),
			[&depths](GLuint i, GLuint j)
			{
				return depths[i] < depths[j];
			}
		);

		// upload the particle positions
		pos_buf.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, positions);
		// upload the particle ages
		age_buf.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, ages);
		// upload the particle identifiers
		id_buf.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, ids);

		gl.Clear().ColorBuffer().DepthBuffer();
		light_cam_pos.Set(
			(cameraMatrix*Vec4f(30.0f, 30.0f, 30.0f, 1.0f)).xyz()
		);
		camera_matrix.Set(cameraMatrix);
		// use the indices to draw the particles
		gl.DrawElements(
			PrimitiveType::Points,
			indices.size(),
			indices.data()
		);
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}

	double ScreenshotTime(void) const
	{
		return 5.0;
	}

	double HeatUpTime(void) const
	{
		return 0.0;
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
	return std::unique_ptr<Example>(new SmokeExample);
}

} // namespace oglplus
