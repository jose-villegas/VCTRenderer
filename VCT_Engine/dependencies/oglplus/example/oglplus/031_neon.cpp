/**
 *  @example oglplus/031_neon.cpp
 *  @brief Shows how to draw a naive neon atom model
 *
 *  @oglplus_screenshot{031_neon}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_transform_feedback2}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/sphere.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/opt/list_init.hpp>

#include <cstdlib>
#include <cassert>

#include "example.hpp"

namespace oglplus {

class PhysicsProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		VertexShader xfbs(ObjectDesc("XFB"));
		xfbs.Source(
			"#version 140\n"
			"#define ParticleCount 20\n"

			"uniform float Time, Interval;"
			"const float Mass = 1.0;"

			"layout(std140) uniform ParticleForceBlock {"
			"	vec4 Forces[ParticleCount];"
			"};"

			"layout(std140) uniform ParticlePositionBlock {"
			"	vec4 Positions[ParticleCount];"
			"};"

			"in vec3 Position;"
			"in vec3 Velocity;"

			"out vec4 xfbPosition;"
			"out vec4 xfbVelocity;"

			"vec3 AnchorSpring(vec3 v, float l, float k)"
			"{"
			"	float ds = (l - length(v));"
			"	return k * sign(ds) * min(abs(ds), l) * normalize(v);"
			"}"

			"vec3 NuclearForce(vec3 v, float c, float ci)"
			"{"
			"	float coef = c*ci;"
			"	if(coef > 0.0)"
			"	{"
			"		float d = length(v);"
			"		float e = 1.3;"
			"		if(d <= 1.0) return 20.0*v/log(d);"
			"		if(d >= e) return 20.0*v*pow(d-e, 2.0)*exp(-d+e);"
			"	}"
			"	return vec3(0.0, 0.0, 0.0);"
			"}"

			"vec3 ElectrostaticForce(vec3 v, float c, float ci)"
			"{"
			"	return -300.0*c*ci*normalize(v)/dot(v, v);"
			"}"

			"vec3 ParticleForces(vec3 p, vec3 pi, vec4 fc, vec4 fci)"
			"{"
			"	vec3 v = pi - p;"
			"	return NuclearForce(v, fc.z, fci.z)+"
			"		ElectrostaticForce(v, fc.w, fci.w);"
			"}"

			"vec3 Force(void)"
			"{"
			"	vec3 f = AnchorSpring("
			"		Position,"
			"		Forces[gl_VertexID].x,"
			"		Forces[gl_VertexID].y "
			"	);"
			"	for(int i=0; i<gl_VertexID; ++i)"
			"	{"
			"		f += ParticleForces("
			"			Position,"
			"			Positions[i].xyz,"
			"			Forces[gl_VertexID],"
			"			Forces[i]"
			"		);"
			"	}"
			"	for(int i=gl_VertexID+1; i<ParticleCount; ++i)"
			"	{"
			"		f += ParticleForces("
			"			Position,"
			"			Positions[i].xyz,"
			"			Forces[gl_VertexID],"
			"			Forces[i]"
			"		);"
			"	}"
			"	return f;"
			"}"

			"void main(void)"
			"{"
			"	xfbVelocity = vec4(Velocity + (Force()* Interval) / Mass, 0.0);"
			"	xfbPosition = vec4(Position + xfbVelocity.xyz * Interval, Time);"
			"}"
		).Compile();

		prog	<< xfbs
			<< TransformFeedbackMode::SeparateAttribs
			<< "xfbPosition"
			<< "xfbVelocity";

		prog.Link().Use();

		return std::move(prog);
	}

	const Program& self(void) { return *this; }
public:
	ProgramUniform<GLfloat> time, interval;

	PhysicsProg(void)
	 : Program(make())
	 , time(self(), "Time")
	 , interval(self(), "Interval")
	{ }
};

class PhysicsSimulator
{
private:
	Context gl;

	const GLuint particle_count;

	PhysicsProg prog;

	Array<VertexArray> vao;
	Array<TransformFeedback> xfb;
	Array<Buffer> positions, velocities;

	Buffer forces;
public:
	PhysicsSimulator(void)
	 : particle_count(30)
	 , vao(2)
	 , xfb(2)
	 , positions(2)
	 , velocities(2)
	{

		float neutron_orbital = 2.0f;
		float proton_orbital = 1.8f;
		float electron_orbital_1 = 30.0f;
		float electron_orbital_2 = 45.0f;
		// initial positions
		std::vector<GLfloat> pos_data(particle_count*4, 0.0);

		GLuint k = 0;
		for(GLuint i=0; i!=particle_count; ++i)
		{
			Vec3f pos(
				(std::rand()/float(RAND_MAX)-0.5f),
				(std::rand()/float(RAND_MAX)-0.5f),
				(std::rand()/float(RAND_MAX)-0.5f)
			);
			pos = Normalized(pos)*(1.0f+(std::rand()/float(RAND_MAX)-0.5f)*0.1);
			if(i < 10) pos = pos*neutron_orbital;
			else if(i < 20) pos = pos*proton_orbital;
			else if(i < 22) pos = pos*electron_orbital_1;
			else if(i < 30) pos = pos*electron_orbital_2;

			pos_data[k++] = pos.x();
			pos_data[k++] = pos.y();
			pos_data[k++] = pos.z();
			pos_data[k++] = 1.0f;
		}
		assert(k == pos_data.size());

		// initial velocities
		std::vector<GLfloat> vel_data(particle_count*4, 0.0);
		k = 0;
		for(GLuint i=0; i!=particle_count; ++i)
		{
			float v = 0.5f;
			if(i < 10) v *= neutron_orbital;
			else if(i < 20) v *= proton_orbital;
			else if(i < 22) v *= electron_orbital_1;
			else if(i < 30) v *= electron_orbital_2;

			for(GLuint c=0; c!=3; ++c)
				vel_data[k++] = v*v*(1.0f*std::rand()/float(RAND_MAX)-0.5f);
			vel_data[k++] = 0.0f;
		}
		assert(k == vel_data.size());

		for(GLuint b=0; b!=2; ++b)
		{
			vao[b].Bind();

			positions[b]
				<< Buffer::Target::Array
				<< BufferUsage::DynamicDraw
				<< pos_data;
			(prog|"Position").Setup<Vec4f>().Enable();

			velocities[b]
				<< Buffer::Target::Array
				<< BufferUsage::DynamicDraw
				<< vel_data;
			(prog|"Velocity").Setup<Vec4f>().Enable();
		}
		NoVertexArray().Bind();
		for(GLuint b=0; b!=2; ++b)
		{
			GLuint nb = (b+1)%2;
			xfb[b].Bind();

			positions[nb]
				<< BufferIndexedTarget::TransformFeedback << 0
				<< BufferTarget::TransformFeedback;

			velocities[nb]
				<< BufferIndexedTarget::TransformFeedback << 1
				<< BufferTarget::TransformFeedback;
		}
		DefaultTransformFeedback().Bind();

		// force coefficients
		std::vector<GLfloat> force_data(particle_count*4, 0.0);
		k = 0;
		//[0] = anchor spring length
		//[1] = anchor spring strength
		//[2] = nuclear
		//[3] = electrostatic charge
		// neutrons
		for(GLuint i=0; i!=10; ++i)
		{
			force_data[k++] = neutron_orbital;
			force_data[k++] = 190.0f;
			force_data[k++] = 1.0f;
			force_data[k++] = 0.0f;
		}
		// protons
		for(GLuint i=0; i!=10; ++i)
		{
			force_data[k++] = proton_orbital;
			force_data[k++] = 300.0f;
			force_data[k++] = 1.0f;
			force_data[k++] = 1.0f;
		}
		// electrons
		for(GLuint i=0; i!=2; ++i)
		{
			force_data[k++] = electron_orbital_1;
			force_data[k++] = 30.0f;
			force_data[k++] = 0.0f;
			force_data[k++] =-1.0f;
		}
		for(GLuint i=0; i!=8; ++i)
		{
			force_data[k++] = electron_orbital_2;
			force_data[k++] = 25.0f;
			force_data[k++] = 0.0f;
			force_data[k++] =-1.0f;
		}
		assert(k == force_data.size());

		forces	<< BufferIndexedTarget::Uniform << 0
			<< BufferTarget::Uniform
			<< force_data;

		prog.Use();
		UniformBlock(prog, "ParticleForceBlock").Binding(0);
		UniformBlock(prog, "ParticlePositionBlock").Binding(1);
	}

	GLuint ParticleCount(void) const
	{
		return particle_count;
	}

	Reference<Buffer> Offsets(void) const
	{
		return positions[0];
	}

	void Update(double time, double time_diff)
	{
		if(time_diff <= 0.0) return;
		const GLuint nsteps = 10;
		assert(nsteps % 2 == 0);

		prog.Use();
		prog.time.Set(GLfloat(time));
		prog.interval.Set(GLfloat(time_diff / nsteps));

		for(GLuint s=0; s!=nsteps; ++s)
		{
			GLuint b = s%2;

			vao[b].Bind();
			xfb[b].Bind();

			TransformFeedback::Activator xfba(TransformFeedbackPrimitiveType::Points);
			gl.DrawArrays(PrimitiveType::Points, 0, particle_count);
			xfba.Finish();
		}
		NoVertexArray().Bind();
		DefaultTransformFeedback().Bind();
	}
};

class ParticleProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Particle"));

		VertexShader vs(ObjectDesc("Particle vertex"));
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"uniform vec3 LightPosition;"

			"in vec3 Position;"
			"in vec3 Normal;"
			"in vec3 Offset;"

 			"out vec3 vertNormal;"
			"out vec3 vertLight;"
			"out vec3 vertViewNormal;"
			"flat out int vertType;"

			"void main(void)"
			"{"
			"	gl_Position = vec4(Position+Offset, 1.0);"
			"	vertNormal = Normal;"
			"	vertViewNormal = mat3(CameraMatrix)*vertNormal;"
			"	vertLight = LightPosition - gl_Position.xyz;"
			"	vertType = gl_InstanceID / 10;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();
		prog.AttachShader(vs);

		FragmentShader fs(ObjectDesc("Particle fragment"));
		fs.Source(
			"#version 140\n"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"in vec3 vertViewNormal;"
			"flat in int vertType;"
			"out vec4 fragColor;"

			"bool neutron_sign(void)"
			"{"
			"	return false;"
			"}"

			"bool electron_sign(void)"
			"{"
			"	return ("
			"		abs(vertViewNormal.x) < 0.5 &&"
			"		abs(vertViewNormal.y) < 0.2"
			"	);"
			"}"

			"bool proton_sign(void)"
			"{"
			"	return  electron_sign() || ("
			"		abs(vertViewNormal.y) < 0.5 &&"
			"		abs(vertViewNormal.x) < 0.2 "
			"	);"
			"}"

			"vec3 neutron_color(void)"
			"{"
			"	return vec3(0.5, 0.5, 0.5);"
			"}"

			"vec3 electron_color(void)"
			"{"
			"	return vec3(0.0, 0.0, 1.0);"
			"}"

			"vec3 proton_color(void)"
			"{"
			"	return vec3(1.0, 0.0, 0.0);"
			"}"

			"bool sign[3] = bool[3]("
			"	neutron_sign(),"
			"	proton_sign(),"
			"	electron_sign()"
			");"
			"vec3 color[3] = vec3[3]("
			"	neutron_color(),"
			"	proton_color(),"
			"	electron_color()"
			");"

			"void main(void)"
			"{"
			"	float lighting = dot("
			"		vertNormal, "
			"		normalize(vertLight)"
			"	);"
			"	float intensity = clamp("
			"		0.4 + lighting * 1.0,"
			"		0.0,"
			"		1.0"
			"	);"
			"	fragColor = sign[vertType]?"
			"		vec4(1.0, 1.0, 1.0, 1.0):"
			"		vec4(color[vertType] * intensity, 1.0);"
			"}"
		).Compile();
		prog.AttachShader(fs);

		prog.Link().Use();

		return std::move(prog);
	}

	const Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<Vec3f> light_position;

	ParticleProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , light_position(self(), "LightPosition")
	{ }
};

class Particle
 : public shapes::ShapeWrapper
{
public:
	Particle(const Program& program, const BufferOps& offsets)
	 : shapes::ShapeWrapper(
		List("Position")("Normal").Get(),
		shapes::Sphere(),
		program
	)
	{
		offsets.Bind(Buffer::Target::Array);
		(program|"Offset").Setup<Vec4f>().Enable().Divisor(1);
	}
};

class TrailProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Trail"));

		VertexShader vs(ObjectDesc("Trail vertex"));
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"uniform float Time;"

			"in vec4 PositionAndTime;"

			"out float vertAge;"

			"void main(void)"
			"{"
			"	vec4 Position = vec4(PositionAndTime.xyz, 1.0);"
			"	vertAge = Time - PositionAndTime.w;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * Position;"
			"}"
		).Compile();
		prog.AttachShader(vs);

		FragmentShader fs(ObjectDesc("Trail fragment"));
		fs.Source(
			"#version 140\n"

			"in float vertAge;"

			"out vec4 fragColor;"

			"void main(void)"
			"{"
			"	float a = exp(-vertAge);"
			"	fragColor = vec4(a, a, a, a);"
			"}"
		).Compile();
		prog.AttachShader(fs);

		prog.Link().Use();
		return std::move(prog);
	}

	const Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<GLfloat> time;

	TrailProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , time(self(), "Time")
	{ }
};

class Trails
{
private:
	Context gl;

	const GLuint electron_offset;
	const GLuint electron_count;
	const GLuint max_trail_points;
	GLuint trail_points;
	GLuint curr_trail_point;

	VertexArray vao;
	Buffer positions, indices;
public:
	Trails(const Program& program)
	 : electron_offset(20)
	 , electron_count(10)
	 , max_trail_points(128)
	 , trail_points(0)
	 , curr_trail_point(0)
	{
		vao.Bind();

		std::vector<GLfloat> pos_data(electron_count*max_trail_points*4, 0.0);

		positions.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, pos_data, BufferUsage::DynamicDraw);
		(program|"PositionAndTime").Setup<Vec4f>().Enable();

		std::vector<GLshort> idx_data(electron_count*(max_trail_points)*2);

		GLuint k = 0;

		for(GLuint p=1; p!=max_trail_points; ++p)
		{
			for(GLuint e=0; e!=electron_count; ++e)
			{
				idx_data[k++] = GLshort(e+electron_count*(p-1));
				idx_data[k++] = GLshort(e+electron_count*p);
			}
		}
		for(GLuint e=0; e!=electron_count; ++e)
		{
			idx_data[k++] = GLshort(e+electron_count*(max_trail_points-1));
			idx_data[k++] = GLshort(e);
		}
		assert(k == idx_data.size());

		indices.Bind(Buffer::Target::ElementArray);
		Buffer::Data(Buffer::Target::ElementArray, idx_data);
	}

	void Update(const BufferOps& offsets)
	{
		offsets.Bind(Buffer::Target::CopyRead);
		positions.Bind(Buffer::Target::CopyWrite);

		if(curr_trail_point >= max_trail_points)
			curr_trail_point = 0;

		Buffer::CopySubData(
			Buffer::Target::CopyRead,
			Buffer::Target::CopyWrite,
			electron_offset*4*sizeof(GLfloat),
			curr_trail_point*electron_count*4*sizeof(GLfloat),
			electron_count*4*sizeof(GLfloat)
		);
		if(trail_points < max_trail_points)
			++trail_points;
		++curr_trail_point;
	}

	void Use(void)
	{
		vao.Bind();
	}

	void Draw(void)
	{
		if(curr_trail_point>1)
		{
			gl.DrawElements(
				PrimitiveType::Lines,
				electron_count*(curr_trail_point-1)*2,
				DataType::UnsignedShort
			);
		}
		if(trail_points >= max_trail_points)
		{
			gl.DrawElements(
				PrimitiveType::Lines,
				electron_count*(max_trail_points-curr_trail_point)*2,
				reinterpret_cast<GLushort*>(
					electron_count*curr_trail_point*2*sizeof(GLushort)
				)
			);
		}
	}
};

class NeonExample : public Example
{
private:
	Context gl;

	PhysicsSimulator physics;

	ParticleProg particle_prog;
	Particle particle;

	TrailProg trail_prog;
	Trails trails;
public:
	NeonExample(void)
	 : gl()
	 , physics()
	 , particle_prog()
	 , particle(particle_prog, physics.Offsets())
	 , trails(trail_prog)
	{
		gl.ClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);

		gl.LineWidth(1.5f);

		particle_prog.light_position.Set(80.0f, 80.0f, 80.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		auto perspective = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 1000
		);
		particle_prog.projection_matrix.Set(perspective);
		trail_prog.projection_matrix.Set(perspective);
	}

	void Render(ExampleClock& clock)
	{
		double time = clock.Now().Seconds();
		double interval = clock.Interval().Seconds();
		physics.Update(time, interval);

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(410.0+SineWave(time/21.0)*370.0),
			Degrees(time* 15),
			Degrees(SineWave(time*0.15) * 45)
		);

		gl.Clear().ColorBuffer().DepthBuffer();

		particle_prog.Use();
		particle_prog.camera_matrix.Set(camera);
		particle.Use();
		particle.Draw(physics.ParticleCount());

		gl.Enable(Capability::Blend);
		gl.DepthMask(false);
		trail_prog.Use();
		trail_prog.camera_matrix.Set(camera);
		trail_prog.time.Set(GLfloat(time));
		trails.Update(physics.Offsets());
		trails.Use();
		trails.Draw();
		gl.DepthMask(true);
		gl.Disable(Capability::Blend);
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double HeatUpTime(void) const
	{
		return 0.0;
	}

	double ScreenshotTime(void) const
	{
		return 14.0;
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
	std::srand(94837);
	return std::unique_ptr<Example>(new NeonExample);
}

} // namespace oglplus
