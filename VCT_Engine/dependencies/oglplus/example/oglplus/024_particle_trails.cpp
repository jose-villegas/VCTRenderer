/**
 *  @example oglplus/024_particle_trails.cpp
 *  @brief Shows how to draw multpile properly sorted particle systems
 *
 *  @oglplus_screenshot{024_particle_trails}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/opt/list_init.hpp>

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
	CubicBezierLoop<Vec3f, double> path;
	double cycle_time;
	double lifetime;
	double spawn_interval;
	double spawn_time;

	std::vector<Vec3f> positions;
	std::vector<Vec3f> directions;
	std::vector<float> ages;

	// Creates a directional vector for a new particle
	static Vec3f NewDirection(void)
	{
		float disp = 0.4f;
		float dx = (0.5f - (float(std::rand())/RAND_MAX))*disp;
		float dy = (0.5f - (float(std::rand())/RAND_MAX))*disp;
		float dz = (0.5f - (float(std::rand())/RAND_MAX))*disp;
		return Vec3f(dx, dy, dz);
	}

	// Spawns a new particle if the time is right
	bool SpawnParticle(
		double time,
		Vec3f& position,
		Vec3f& direction,
		float& age
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
			return true;
		}
		return false;
	}
public:
	ParticleSystem(
		const std::vector<Vec3f>& path_points,
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
					ages[i]
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
		// spawn new particles if necessary
		while(SpawnParticle(clock.Now().Seconds(), position, direction, age))
		{
			positions.push_back(position);
			directions.push_back(direction);
			ages.push_back(age);
		}
	}

	void Upload(std::vector<Vec3f>& pos, std::vector<float>& age)
	{
		pos.insert(pos.end(), positions.begin(), positions.end());
		age.insert(age.end(), ages.begin(), ages.end());
	}
};

class SmokeExample : public Example
{
private:
	// A list of particle emitters
	std::vector<ParticleSystem> emitters;

	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;
	static Program make_prog(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4 CameraMatrix;"
			"in vec4 Position;"
			"in float Age;"
			"out float vertAge;"
			"void main(void)"
			"{"
			"	gl_Position = CameraMatrix * Position;"
			"	vertAge = Age;"
			"}"
		);
		vs.Compile();

		GeometryShader gs;
		gs.Source(
			"#version 150\n"
			"layout(points) in;"
			"layout(triangle_strip, max_vertices = 4) out;"
			"uniform mat4 ProjectionMatrix;"
			"in float vertAge[];"
			"out float geomAge;"
			"void main(void)"
			"{"
			"	if(vertAge[0] > 1.0) return;"
			"	float s = 0.5;"
			"	float yo[2] = float[2](-1.0, 1.0);"
			"	float xo[2] = float[2](-1.0, 1.0);"
			"	for(int j=0;j!=2;++j)"
			"	for(int i=0;i!=2;++i)"
			"	{"
			"		float xoffs = xo[i]*(1.0+vertAge[0])*s;"
			"		float yoffs = yo[j]*(1.0+vertAge[0])*s;"
			"		gl_Position = ProjectionMatrix * vec4("
			"			gl_in[0].gl_Position.x-xoffs,"
			"			gl_in[0].gl_Position.y-yoffs,"
			"			gl_in[0].gl_Position.z,"
			"			1.0"
			"		);"
			"		geomAge = vertAge[0];"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();

		FragmentShader fs;
		fs.Source(
			"#version 150\n"
			"in float geomAge;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec3 Color1 = vec3(1.0, 0.5, 0.5);"
			"	vec3 Color2 = vec3(0.3, 0.1, 0.1);"
			"	fragColor = vec4("
			"		mix(Color1, Color2, geomAge),"
			"		1.0 - geomAge"
			"	);"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(gs);
		prog.AttachShader(fs);
		prog.Link();
		prog.Use();

		return prog;
	}

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix;

	// A vertex array object for the particles
	VertexArray particles;

	// VBOs for the particle positions and ages
	Buffer pos_buf, age_buf;

	std::vector<Vec3f> positions;
	std::vector<float> ages;
public:
	SmokeExample(void)
	 : emitters()
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	{
		emitters.push_back(
			ParticleSystem(
				ListOf<Vec3f>
					(Vec3f(-20.0f, -10.0f,  10.0f))
					(Vec3f( 20.0f,   0.0f, -20.0f))
					(Vec3f( 20.0f,  10.0f,  20.0f))
					(Vec3f(-20.0f,   0.0f, -10.0f))
				.As<std::vector<Vec3f>>(), 5.0, 200.0
			)
		);
		emitters.push_back(
			ParticleSystem(
				ListOf<Vec3f>
					(Vec3f( 30.0f,   0.0f,   0.0f))
					(Vec3f(-30.0f,   0.0f,   0.0f))
					(Vec3f(-20.0f,  20.0f,   0.0f))
					(Vec3f( 20.0f, -10.0f,   0.0f))
				.As<std::vector<Vec3f>>(), 3.0, 200.0
			)
		);
		emitters.push_back(
			ParticleSystem(
				ListOf<Vec3f>
					(Vec3f(  5.0f,  20.0f,  20.0f))
					(Vec3f( -5.0f,  20.0f, -20.0f))
					(Vec3f(  5.0f, -20.0f, -20.0f))
					(Vec3f( -5.0f, -20.0f,  20.0f))
				.As<std::vector<Vec3f>>(), 20.0, 100.0
			)
		);

		// bind the VAO for the particles
		particles.Bind();

		// bind the VBO for the particle positions
		pos_buf.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, positions, BufferUsage::DynamicDraw);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(3);
			attr.Enable();
		}

		// bind the VBO for the particle ages
		age_buf.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, ages, BufferUsage::DynamicDraw);
			VertexArrayAttrib attr(prog, "Age");
			attr.Setup<GLfloat>(1);
			attr.Enable();
		}
		//
		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
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
				Degrees(60),
				float(width)/height,
				1, 100
			)
		);
	}

	void Render(ExampleClock& clock)
	{
		positions.clear();
		ages.clear();

		// update the emitters and get the particle data
		for(auto i=emitters.begin(), e=emitters.end(); i!=e; ++i)
		{
			i->Update(clock);
			i->Upload(positions, ages);
		}
		assert(positions.size() == ages.size());

		// make a camera matrix
		auto cameraMatrix = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(38.0 - SineWave(clock.Now().Seconds() / 6.0) * 17.0),
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
		Buffer::Data(Buffer::Target::Array, positions, BufferUsage::DynamicDraw);
		// upload the particle ages
		age_buf.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, ages, BufferUsage::DynamicDraw);

		gl.Clear().ColorBuffer().DepthBuffer();
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
		return 4.5;
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
