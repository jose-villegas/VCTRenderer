/**
 *  @example oglplus/040_jelly_cube.cpp
 *  @brief Shows mass-spring-based simulation of a soft body
 *
 *  @oglplus_screenshot{040_jelly_cube}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *  @oglplus_example_uses_cpp_feat{FUNCTION_TEMPLATE_DEFAULT_ARGS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_transform_feedback2}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/opt/list_init.hpp>

#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/plane.hpp>

#include <oglplus/images/brushed_metal.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/opt/smart_enums.hpp>

#include <vector>
#include <cassert>

#include "example.hpp"

namespace oglplus {

namespace se = ::oglplus::smart_enums;

class MetalProgram
 : public Program
{
private:
	static Program make(void)
	{
		Shader vs(se::Vertex(), ObjectDesc("Metal vertex"));
		vs.Source("#version 150\n"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec3 CameraPosition, LightPosition;"

			"in vec4 Position;"
			"in vec3 Normal, Tangent;"
			"in vec2 TexCoord;"

			"out vec3 vertNormal, vertTangent, vertBitangent;"
			"out vec3 vertLightDir, vertViewDir;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertViewDir = CameraPosition - gl_Position.xyz;"
			"	vertNormal = Normal;"
			"	vertTangent = Tangent;"
			"	vertBitangent = cross(vertNormal, vertTangent);"
			"	vertTexCoord = TexCoord * 100.0;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		vs.Compile();

		Shader fs(se::Fragment(), ObjectDesc("Metal fragment"));
		fs.Source("#version 150\n"
			"const vec3 Color1 = vec3(0.5, 0.7, 0.6);"
			"const vec3 Color2 = vec3(0.7, 0.9, 0.8);"

			"uniform sampler2D MetalTex;"
			"uniform float LightMultiplier;"

			"in vec3 vertNormal, vertTangent, vertBitangent;"
			"in vec3 vertLightDir, vertViewDir;"
			"in vec2 vertTexCoord;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec3 Sample = texture(MetalTex, vertTexCoord).rgb;"
			"	vec3 LightColor = vec3(1.0, 1.0, 0.9);"

			"	vec3 Normal = normalize("
			"		2.0*vertNormal + "
			"		(Sample.r - 0.5)*vertTangent + "
			"		(Sample.g - 0.5)*vertBitangent"
			"	);"

			"	vec3 LightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		Normal"
			"	);"

			"	float Specular = LightMultiplier * pow(max(dot("
			"		normalize(LightRefl),"
			"		normalize(vertViewDir)"
			"	)+0.04, 0.0), 16+Sample.b*48)*pow(0.4+Sample.b*1.6, 4.0);"

			"	Normal = normalize(vertNormal*3.0 + Normal);"

			"	float Diffuse = LightMultiplier * pow(max(dot("
			"		normalize(Normal), "
			"		normalize(vertLightDir)"
			"	), 0.0), 2.0);"

			"	float Ambient = 0.6;"

			"	vec3 Color = mix(Color1, Color2, Sample.b);"

			"	fragColor = "
			"		Color * Ambient +"
			"		LightColor * Color * Diffuse + "
			"		LightColor * Specular;"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		prog.Link();

		return prog;
	}

	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix, projection_matrix;
	ProgramUniform<Vec3f> camera_position;
	ProgramUniform<GLfloat> light_multiplier;

	MetalProgram(void)
	 : Program(make())
	 , camera_matrix(self(), "CameraMatrix")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_position(self(), "CameraPosition")
	 , light_multiplier(self(), "LightMultiplier")
	{ }
};

class MetalFloor
 : public shapes::ShapeWrapper
{
private:
	Texture metal_tex;
public:
	MetalFloor(const MetalProgram& metal_prog)
	 : shapes::ShapeWrapper(
		List("Position")("Normal")("Tangent")("TexCoord").Get(),
		shapes::Plane(Vec3f(200, 0, 0), Vec3f(0, 0,-200)),
		metal_prog
	)
	{
		Texture::Active(0);
		UniformSampler(metal_prog, "MetalTex").Set(0);
		Context::Bound(se::_2D(), metal_tex)
			.MinFilter(se::LinearMipmapLinear())
			.MagFilter(se::Linear())
			.WrapS(se::Repeat())
			.WrapT(se::Repeat())
			.Image2D(
				images::BrushedMetalUByte(
					512, 512,
					5120,
					-3, +3,
					32, 128
				)
			).GenerateMipmap();
	}

	void Draw(MetalProgram& metal_prog)
	{
		metal_prog.Use();
		shapes::ShapeWrapper::Use();
		shapes::ShapeWrapper::Draw();
	}
};

class CameraDriveProgram
 : public Program
{
private:
	static Program make(void)
	{
		Shader tfbs(se::Vertex(), ObjectDesc("Camera drive"));
		tfbs.Source(
			"#version 150\n"

			"uniform float Interval, Mass, TargetDistance, Elevation;"
			"uniform uint CubeCenterIndex;"

			"layout(std140) uniform CubePositionBlock {"
			"	vec3 CubePositions[512];"
			"};"

			"in vec3 Position, Velocity;"

			"out vec4 tfbPosition, tfbVelocity;"

			"bool is_camera(void)"
			"{"
			"	return gl_VertexID == 0;"
			"}"

			"vec3 spring_force(vec3 P, vec3 Ps, float k, float l)"
			"{"
			"	vec3 v = Ps - P;"
			"	float ds = (length(v) - l);"
			"	return k * sign(ds) * min(abs(ds), l) * normalize(v);"
			"}"

			"vec3 springs(void)"
			"{"
			"	if(is_camera())"
			"	{"
			"		return spring_force("
			"			Position,"
			"			CubePositions[CubeCenterIndex],"
			"			3.0,"
			"			TargetDistance"
			"		);"
			"	}"
			"	else"
			"	{"
			"		return spring_force("
			"			Position,"
			"			CubePositions[CubeCenterIndex],"
			"			9.0,"
			"			0.1"
			"		);"
			"	}"
			"}"

			"vec3 elevation(void)"
			"{"
			"	if(is_camera())"
			"	{"
			"		float ds = (Elevation - Position.y);"
			"		float k = 7.0;"
			"		vec3 v = vec3(0.0, 1.0, 0.0);"
				"	return k * sign(ds) * min(abs(ds), Elevation) * v;"
			"	}"
			"	else return vec3(0.0, 0.0, 0.0);"
			"}"


			"vec3 drag(void)"
			"{"
			"	return -0.08 * Velocity;"
			"}"

			"void main(void)"
			"{"
			"	vec3 Force = drag() + elevation() + springs();"
			"	tfbVelocity = vec4(Velocity + (Force * Interval) / Mass, 1.0);"
			"	tfbPosition = vec4(Position + tfbVelocity.xyz * Interval,1.0);"
			"}"
		);

		tfbs.Compile();

		Program prog;
		prog.AttachShader(tfbs);

		const GLchar* var_names[2] = {"tfbPosition", "tfbVelocity"};
		prog.TransformFeedbackVaryings(2, var_names, se::SeparateAttribs());
		prog.Link();

		return prog;
	}

	const Program& self(void) { return *this; }

public:
	ProgramUniform<GLfloat> interval;

	CameraDriveProgram(void)
	 : Program(make())
	 , interval(self(), "Interval")
	{ }
};

class ChasingCamera
{
private:
	Context gl;

	VertexArray cam_vao;

	Buffer positions, velocities;
	Buffer tfb_positions, tfb_velocities;

	void CopyFeedback(Buffer& src, Buffer& dst)
	{
		src.Bind(se::CopyRead());
		dst.Bind(se::CopyWrite());

		Buffer::CopySubData(
			se::CopyRead(),
			se::CopyWrite(),
			0, 0,
			2 * 4 * sizeof(GLfloat)
		);
	}

	Vec3f cam_pos, tgt_pos;
public:
	ChasingCamera(
		CameraDriveProgram& cam_prog,
		GLfloat mass,
		GLfloat elevation,
		GLfloat target_distance
	)
	{
		std::vector<GLfloat> vel_data(2*4, 0.0f);
		std::vector<GLfloat> pos_data(2*4);
		// camera x,y,z
		pos_data[0] = 0.0f;
		pos_data[1] = elevation;
		pos_data[2] = target_distance;
		pos_data[3] = 1.0f;
		// target x,y,z,w
		pos_data[4] = 0.0f;
		pos_data[5] = elevation;
		pos_data[6] = 0.0f;
		pos_data[7] = 1.0f;

		cam_vao.Bind();

		// setup position buffer
		positions.Bind(se::Array());
		tfb_positions.Bind(se::TransformFeedback());

		Buffer::Data(se::Array(), pos_data);
		Buffer::Resize(se::TransformFeedback(), pos_data);

		tfb_positions.BindBase(se::TransformFeedback(), 0);

		VertexArrayAttrib pos_vert_attr(cam_prog, "Position");
		pos_vert_attr.Setup<Vec4f>();
		pos_vert_attr.Enable();

		// setup velocity buffer
		velocities.Bind(se::Array());
		tfb_velocities.Bind(se::TransformFeedback());

		Buffer::Data(se::Array(), vel_data);
		Buffer::Resize(se::TransformFeedback(), vel_data);

		tfb_velocities.BindBase(se::TransformFeedback(), 1);

		VertexArrayAttrib vel_vert_attr(cam_prog, "Velocity");
		vel_vert_attr.Setup<Vec4f>();
		vel_vert_attr.Enable();

		// setup the uniforms
		cam_prog.Use();
		Uniform<GLfloat>(cam_prog, "Mass").Set(mass);
		Uniform<GLfloat>(cam_prog, "Elevation").Set(elevation);
		Uniform<GLfloat>(cam_prog, "TargetDistance").Set(target_distance);
	}

	void UpdatePhysics(CameraDriveProgram& cam_prog, double interval)
	{
		cam_vao.Bind();
		cam_prog.Use();

		GLuint n = 10;
		cam_prog.interval.Set(GLfloat(interval / n));

		tfb_positions.BindBase(se::TransformFeedback(), 0);
		tfb_velocities.BindBase(se::TransformFeedback(), 1);

		for(GLuint i=0; i!=n; ++i)
		{
			{
				TransformFeedbackPrimitiveType tfbmode = se::Points();
				TransformFeedback::Activator tfb(tfbmode);
				gl.DrawArrays(se::Points(), 0, 2);
			}
			CopyFeedback(tfb_positions, positions);
			CopyFeedback(tfb_velocities, velocities);
		}
		tfb_positions.Bind(se::TransformFeedback());
		BufferTypedMap<Vec4f> map(
			BufferTarget::TransformFeedback,
			BufferMapAccess::Read
		);
		cam_pos = map.At(0).xyz();
		tgt_pos = map.At(1).xyz();
	}

	Vec3f Position(void) const
	{
		return cam_pos;
	}

	Vec3f Target(void) const
	{
		return tgt_pos;
	}

	CamMatrixf Matrix(void) const
	{
		return CamMatrixf::LookingAt(cam_pos, tgt_pos);
	}
};

class JellyPhysProgram
 : public Program
{
private:
	static Program make(void)
	{
		Shader tfbs(se::Vertex(), ObjectDesc("Jelly physics"));
		tfbs.Source(
			"#version 150\n"

			"uniform vec3 ImpulseCenter;"
			"uniform float ImpulseStrength, Interval, Mass;"
			"uniform float SpringALength, SpringBLength, SpringCLength;"
			"uniform float SpringAStrength, SpringBStrength, SpringCStrength;"

			"layout(std140) uniform PositionBlock {"
			"	vec3 Positions[512];"
			"};"

			"struct VertexSpringIndices {"
			"	ivec4 SpringAX;"
			"	ivec4 SpringAY;"
			"	ivec4 SpringAZ;"

			"	ivec4 SpringBX;"
			"	ivec4 SpringBY;"
			"	ivec4 SpringBZ;"

			"	ivec4 SpringCUp;"
			"	ivec4 SpringCDn;"
			"};"

			"layout(std140) uniform SpringIndexBlock {"
			"	VertexSpringIndices VertexSprings[512];"
			"};"

			"in vec3 Position, Velocity;"

			"out vec4 tfbPosition, tfbVelocity;"

			"vec3 gravity(void)"
			"{"
			"	float Acting = -max(sign(Position.y), 0.0);"
			"	vec3 GravAccel = vec3(0.0, Acting * 9.81, 0.0);"
			"	return Mass * GravAccel;"
			"}"

			"vec3 floor(void)"
			"{"
			"	float IsUnder = -min(sign(Position.y), 0.0);"
			"	float GoingDown = -min(sign(Velocity.y), 0.0);"
			"	float Acting = IsUnder * GoingDown;"
			"	vec3 BounceVelocity = vec3(0.0, -Velocity.y, 0.0);"
			"	vec3 FrictionVelocity = vec3(-Velocity.x, 0.0, -Velocity.z);"
			"	vec3 Bounce = Acting * Mass * BounceVelocity / Interval;"
			"	vec3 Friction = IsUnder * Mass * FrictionVelocity / Interval;"
			"	return 1.95 * Bounce + 0.3 * Friction;"
			"}"

			"vec3 spring_force(vec3 P, vec3 Ps, float k, float l)"
			"{"
			"	vec3 v = Ps - P;"
			"	float ds = (length(v) - l);"
			"	return k * sign(ds) * min(abs(ds), l) * normalize(v);"
			"}"

			"vec3 spring_X(int springVertexID, float k, float l)"
			"{"
			"	if(springVertexID < 0) return vec3(0.0, 0.0, 0.0);"
			"	else return spring_force("
			"		Position,"
			"		Positions[springVertexID],"
			"		k, l"
			"	);"
			"}"

			"vec3 spring_X1(ivec4 indices, float k, float l)"
			"{"
			"	return	spring_X(indices.x, k, l*1)+"
			"		spring_X(indices.y, k, l*1)+"
			"		spring_X(indices.z, k, l*2)+"
			"		spring_X(indices.w, k, l*2);"
			"}"

			"vec3 spring_X2(ivec4 indices, float k, float l)"
			"{"
			"	return	spring_X(indices.x, k, l)+"
			"		spring_X(indices.y, k, l)+"
			"		spring_X(indices.z, k, l)+"
			"		spring_X(indices.w, k, l);"
			"}"

			"vec3 spring_A(ivec4 indices)"
			"{"
			"	return spring_X1("
			"		indices, "
			"		SpringAStrength, "
			"		SpringALength"
			"	);"
			"}"

			"vec3 spring_B(ivec4 indices)"
			"{"
			"	return spring_X2("
			"		indices, "
			"		SpringBStrength, "
			"		SpringBLength"
			"	);"
			"}"

			"vec3 spring_C(ivec4 indices)"
			"{"
			"	return spring_X2("
			"		indices, "
			"		SpringCStrength, "
			"		SpringCLength"
			"	);"
			"}"

			"vec3 springs(void)"
			"{"
			"	vec3 Result = vec3(0.0, 0.0, 0.0);"
			"	VertexSpringIndices vsi = VertexSprings[gl_VertexID];"

			"	Result += spring_A(vsi.SpringAX);"
			"	Result += spring_A(vsi.SpringAY);"
			"	Result += spring_A(vsi.SpringAZ);"

			"	Result += spring_B(vsi.SpringBX);"
			"	Result += spring_B(vsi.SpringBY);"
			"	Result += spring_B(vsi.SpringBZ);"

			"	Result += spring_C(vsi.SpringCUp);"
			"	Result += spring_C(vsi.SpringCDn);"

			"	return Result;"
			"}"

			"vec3 drag(void)"
			"{"
			"	return -0.002 * SpringALength * Velocity;"
			"}"

			"vec3 impulse(void)"
			"{"
			"	vec3 v = Position - ImpulseCenter;"
			"	return ImpulseStrength * normalize(v)/length(v);"
			"}"

			"void main(void)"
			"{"
			"	vec3 Force = drag() + gravity() + springs() + floor() + impulse();"
			"	tfbVelocity = vec4(Velocity + (Force * Interval) / Mass, 1.0);"
			"	tfbPosition = vec4(Position + tfbVelocity.xyz * Interval,1.0);"
			"}"
		);
		tfbs.Compile();

		Program prog;
		prog.AttachShader(tfbs);

		const GLchar* var_names[2] = {"tfbPosition", "tfbVelocity"};
		prog.TransformFeedbackVaryings(2, var_names, se::SeparateAttribs());

		prog.Link();
		return prog;
	}

	const Program& self(void) { return *this; }
public:
	ProgramUniform<Vec3f> impulse_center;
	ProgramUniform<GLfloat> impulse_strength, interval, mass;

	JellyPhysProgram(void)
	 : Program(make())
	 , impulse_center(self(), "ImpulseCenter")
	 , impulse_strength(self(), "ImpulseStrength")
	 , interval(self(), "Interval")
	 , mass(self(), "Mass")
	{ }
};

class JellyDrawProgram
 : public Program
{
private:
	static Program make(void)
	{
		Shader vs(se::Vertex(), ObjectDesc("Draw vertex"));
		vs.Source(
			"#version 150\n"

			"uniform vec3 LightPosition;"

			"in vec3 Position;"

			"out vec3 vertLightDir;"

			"void main(void)"
			"{"
			"	gl_Position = vec4(Position, 1.0);"
			"	vertLightDir = normalize(LightPosition - Position);"
			"}"
		);
		vs.Compile();

		Shader gs(se::Geometry(), ObjectDesc("Draw geometry"));
		gs.Source(
			"#version 150\n"
			"layout(triangles_adjacency) in;"
			"layout(triangle_strip, max_vertices = 3) out;"

			"uniform mat4 CameraMatrix, ProjectionMatrix;"

			"in vec3 vertLightDir[6];"

			"out vec3 geomLightDir, geomNormal;"

			"void make_vertex(int index)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix*"
			"		CameraMatrix*"
			"		gl_in[index].gl_Position;"
			"	geomLightDir = vertLightDir[index];"
			"	EmitVertex();"
			"}"

			"vec3 face_normal(int a, int b, int c)"
			"{"
			"	return normalize(cross("
			"		gl_in[c].gl_Position.xyz-"
			"		gl_in[a].gl_Position.xyz,"
			"		gl_in[b].gl_Position.xyz-"
			"		gl_in[a].gl_Position.xyz"
			"	));"
			"}"

			"void main(void)"
			"{"
			"	vec3 fn  = face_normal(0, 2, 4);"
			"	vec3 fn1 = face_normal(1, 2, 0);"
			"	vec3 fn3 = face_normal(3, 4, 2);"
			"	vec3 fn5 = face_normal(5, 0, 4);"

			"	const float a = 0.400, b = (1.0 - a) * 0.5;"
			"	geomNormal = fn*a + fn5*b + fn1*b;"
			"	make_vertex(0);"

			"	geomNormal = fn*a + fn1*b + fn3*b;"
			"	make_vertex(2);"

			"	geomNormal = fn*a + fn3*b + fn5*b;"
			"	make_vertex(4);"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();

		Shader fs(se::Fragment(), ObjectDesc("Draw fragment"));
		fs.Source(
			"#version 150\n"

			"uniform vec3 AmbientColor, DiffuseColor;"
			"uniform float LightMultiplier;"

			"in vec3 geomLightDir, geomNormal;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float Ambient = 0.8;"
			"	float Diffuse = LightMultiplier * sqrt(max(dot("
			"		geomLightDir,"
			"		geomNormal"
			"	) - 0.1, 0.0));"
			"	fragColor = "
			"		Ambient * AmbientColor+"
			"		Diffuse * DiffuseColor;"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(gs);
		prog.AttachShader(fs);
		prog.Link();
		return prog;
	}

	const Program& self(void) { return *this; }

public:
	ProgramUniform<Mat4f> camera_matrix, projection_matrix;
	ProgramUniform<Vec3f> ambient_color, diffuse_color;
	ProgramUniform<GLfloat> light_multiplier;

	JellyDrawProgram(void)
	 : Program(make())
	 , camera_matrix(self(), "CameraMatrix")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , ambient_color(self(), "AmbientColor")
	 , diffuse_color(self(), "DiffuseColor")
	 , light_multiplier(self(), "LightMultiplier")
	{ }
};

class ShadowProgram
 : public Program
{
private:
	static Program make(void)
	{
		Shader vs(se::Vertex(), ObjectDesc("Shadow vertex"));
		vs.Source(
			"#version 150\n"

			"uniform vec3 LightPosition;"

			"in vec3 Position;"

			"out vec3 vertLightDir;"

			"void main(void)"
			"{"
			"	gl_Position = vec4(Position, 1.0);"
			"	vertLightDir = LightPosition - Position;"
			"}"
		);
		vs.Compile();

		Shader gs(se::Geometry(), ObjectDesc("Shadow geometry"));
		gs.Source(
			"#version 150\n"
			"layout(triangles_adjacency) in;"
			"layout(triangle_strip, max_vertices = 12) out;"

			"uniform mat4 CameraMatrix, ProjectionMatrix;"

			"in vec3 vertLightDir[6];"

			"void make_near_vertex(int index)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix*"
			"		CameraMatrix*"
			"		gl_in[index].gl_Position;"
			"	EmitVertex();"
			"}"

			"void make_far_vertex(int index)"
			"{"
			"	vec3 Pos = gl_in[index].gl_Position.xyz;"
			"	Pos -= vertLightDir[index];"
			"	gl_Position = "
			"		ProjectionMatrix*"
			"		CameraMatrix*"
			"		vec4(Pos, 1.0);"
			"	EmitVertex();"
			"}"

			"void make_plane(int a, int b)"
			"{"
			"	make_near_vertex(a);"
			"	make_near_vertex(b);"
			"	make_far_vertex(a);"
			"	make_far_vertex(b);"
			"	EndPrimitive();"
			"}"

			"vec3 face_normal(int a, int b, int c)"
			"{"
			"	return cross("
			"		gl_in[c].gl_Position.xyz-"
			"		gl_in[a].gl_Position.xyz,"
			"		gl_in[b].gl_Position.xyz-"
			"		gl_in[a].gl_Position.xyz"
			"	);"
			"}"

			"vec3 face_light_dir(int a, int b, int c)"
			"{"
			"	return ("
			"		vertLightDir[a]+"
			"		vertLightDir[b]+"
			"		vertLightDir[c] "
			"	);"
			"}"

			"void main(void)"
			"{"
			"	vec3 ld = face_light_dir(0, 2, 4);"
			"	vec3 fn  = face_normal(0, 2, 4);"

			"	if(dot(fn, ld) >= 0.0)"
			"	{"
			"		make_plane(2, 0);"
			"		make_plane(4, 2);"
			"		make_plane(0, 4);"
			"	}"
			"}"
		);
		gs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(gs);

		prog.Link();

		return prog;
	}

	const Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix, projection_matrix;

	ShadowProgram(void)
	 : Program(make())
	 , camera_matrix(self(), "CameraMatrix")
	 , projection_matrix(self(), "ProjectionMatrix")
	{ }
};

// Wrapper around the data for rendering of the jelly cube
class JellyCube
{
private:
	const GLsizei vertex_count, face_index_count, shadow_index_count;

	Context gl;

	VertexArray phys_vao, draw_vao, shadow_vao;

	Buffer positions, velocities, indices, shadow_indices;
	Buffer tfb_positions, tfb_velocities;

	void CopyFeedback(Buffer& src, Buffer& dst)
	{
		src.Bind(se::CopyRead());
		dst.Bind(se::CopyWrite());

		Buffer::CopySubData(
			se::CopyRead(),
			se::CopyWrite(),
			0, 0,
			vertex_count * 4 * sizeof(GLfloat)
		);
	}

	Buffer springs;

	void InitPositions(
		JellyPhysProgram& phys_prog,
		JellyDrawProgram& draw_prog,
		ShadowProgram& shadow_prog,
		CameraDriveProgram& cam_prog,
		const GLfloat size,
		const GLfloat mass,
		const GLuint n,
		const Mat4f& transform
	)
	{
		const GLfloat s_2 = size * 0.5f;
		const GLfloat s_n = size / (n - 1);

		std::vector<GLfloat> pos_data(vertex_count * 4);
		auto pi = pos_data.begin(), pe = pos_data.end();

		for(GLuint z=0; z!=n; ++z)
		{
			const GLfloat z_offs = -s_2 + s_n*z;
			for(GLuint y=0; y!=n; ++y)
			{
				const GLfloat y_offs = -s_2 + s_n*y;
				for(GLuint x=0; x!=n; ++x)
				{
					const GLfloat x_offs = -s_2 + s_n*x;

					Vec4f initial(x_offs, y_offs, z_offs, 1.0f);
					Vec4f pos = transform * initial;

					assert(pi != pe);
					*pi = pos.x() ; ++pi;
					assert(pi != pe);
					*pi = pos.y() ; ++pi;
					assert(pi != pe);
					*pi = pos.z(); ++pi;
					assert(pi != pe);
					*pi = 1.0f; ++pi; // uniform buffer padding
				}
			}
		}
		assert(pi == pe);

		positions.Bind(se::Array());
		tfb_positions.Bind(se::TransformFeedback());

		Buffer::Data(se::Array(), pos_data);
		Buffer::Resize(se::TransformFeedback(), pos_data);

		positions.BindBase(se::Uniform(), 0);
		tfb_positions.BindBase(se::TransformFeedback(), 0);

		draw_vao.Bind();
		VertexArrayAttrib draw_vert_attr(draw_prog, "Position");
		draw_vert_attr.Setup<Vec4f>();
		draw_vert_attr.Enable();

		phys_vao.Bind();
		VertexArrayAttrib phys_vert_attr(phys_prog, "Position");
		phys_vert_attr.Setup<Vec4f>();
		phys_vert_attr.Enable();

		shadow_vao.Bind();
		VertexArrayAttrib shadow_vert_attr(shadow_prog, "Position");
		shadow_vert_attr.Setup<Vec4f>();
		shadow_vert_attr.Enable();

		phys_prog.Use();

		phys_prog.mass.Set(mass / vertex_count);

		UniformBlock(phys_prog, "PositionBlock").Binding(0);
		Uniform<GLfloat>(phys_prog, "SpringALength").Set(s_n);
		Uniform<GLfloat>(phys_prog, "SpringBLength").Set(GLfloat(sqrt(2.0f)*s_n));
		Uniform<GLfloat>(phys_prog, "SpringCLength").Set(GLfloat(sqrt(3.0f)*s_n));

		GLfloat k = 1.1f;
		Uniform<GLfloat>(phys_prog, "SpringAStrength").Set(mass * 4.2f * k);
		Uniform<GLfloat>(phys_prog, "SpringBStrength").Set(mass * 4.0f * k);
		Uniform<GLfloat>(phys_prog, "SpringCStrength").Set(mass * 3.8f * k);

		cam_prog.Use();
		UniformBlock(cam_prog, "CubePositionBlock").Binding(0);

		Uniform<GLuint>(cam_prog, "CubeCenterIndex").Set(vertex_count/2);
	}

	void InitVelocities(const JellyPhysProgram& phys_prog)
	{
		const std::vector<GLfloat> vel_data(vertex_count * 4, 0.0f);

		velocities.Bind(se::Array());

		tfb_velocities.Bind(se::TransformFeedback());

		Buffer::Data(se::Array(), vel_data);
		Buffer::Resize(se::TransformFeedback(), vel_data);

		tfb_velocities.BindBase(se::TransformFeedback(), 1);

		phys_vao.Bind();
		VertexArrayAttrib vert_attr(phys_prog, "Velocity");
		vert_attr.Setup<Vec4f>();
		vert_attr.Enable();
	}

	int FaceIndex(
		const GLuint n,
		const GLuint n_1,
		const GLuint f,
		int i,
		int j
	) const
	{
		if(i<0)
		{
			i += n_1;
			assert(i <=int(n_1));
			assert(j >=  0);
			assert(j <=int(n_1));
			switch(f)
			{
				// +x -> +z
				case 0: return FaceIndex(n, n_1, 4,     i,     j);
				// -x -> -z
				case 1: return FaceIndex(n, n_1, 5,     i,     j);
				// +y -> -x
				case 2: return FaceIndex(n, n_1, 1, n_1-j,     i);
				// -y -> -x
				case 3: return FaceIndex(n, n_1, 1,     j, n_1-i);
				// +z -> -x
				case 4: return FaceIndex(n, n_1, 1,     i,     j);
				// -z -> +x
				case 5: return FaceIndex(n, n_1, 0,     i,     j);
			}
		}
		else if(i>int(n_1))
		{
			i -= n_1;
			assert(i <= int(n_1));
			assert(j >=  0);
			assert(j <= int(n_1));
			switch(f)
			{
				// +x -> -z
				case 0: return FaceIndex(n, n_1, 5,     i,     j);
				// -x -> +z
				case 1: return FaceIndex(n, n_1, 4,     i,     j);
				// +y -> +x
				case 2: return FaceIndex(n, n_1, 0,     j, n_1-i);
				// -y -> +x
				case 3: return FaceIndex(n, n_1, 0, n_1-j,     i);
				// +z -> +x
				case 4: return FaceIndex(n, n_1, 0,     i,     j);
				// -z -> -x
				case 5: return FaceIndex(n, n_1, 1,     i,     j);
			}
		}
		else if(j<0)
		{
			j += n_1;
			assert(j <= int(n_1));
			assert(i >=  0);
			assert(i <= int(n_1));
			switch(f)
			{
				// +x -> -y
				case 0: return FaceIndex(n, n_1, 3,     j, n_1-i);
				// -x -> -y
				case 1: return FaceIndex(n, n_1, 3, n_1-j,     i);
				// +y -> +z
				case 2: return FaceIndex(n, n_1, 4,     i,     j);
				// -y -> -z
				case 3: return FaceIndex(n, n_1, 5, n_1-i, n_1-j);
				// +z -> -y
				case 4: return FaceIndex(n, n_1, 3,     i,     j);
				// -z -> -y
				case 5: return FaceIndex(n, n_1, 3, n_1-i, n_1-j);
			}
		}
		else if(j>int(n_1))
		{
			j -= n_1;
			assert(j <= int(n_1));
			assert(i >=  0);
			assert(i <= int(n_1));
			switch(f)
			{
				// +x -> +y
				case 0: return FaceIndex(n, n_1, 2, n_1-j,     i);
				// -x -> +y
				case 1: return FaceIndex(n, n_1, 2,     j, n_1-i);
				// +y -> -z
				case 2: return FaceIndex(n, n_1, 5, n_1-i, n_1-j);
				// -y -> +z
				case 3: return FaceIndex(n, n_1, 4,     i,     j);
				// +z -> +y
				case 4: return FaceIndex(n, n_1, 2,     i,     j);
				// -z -> +y
				case 5: return FaceIndex(n, n_1, 2, n_1-i, n_1-j);
			}
		}

		assert(f < 6);
		assert(i >= 0);
		assert(j >= 0);
		assert(i <= int(n_1));
		assert(j <= int(n_1));

		int get_x[6] = { int(n_1), 0, i, i, i, int(n_1)-i };
		int get_y[6] = { j, j, int(n_1), 0, j, j };
		int get_z[6] = { int(n_1)-i, i, int(n_1)-j, j, int(n_1), 0 };
		int x = get_x[f];
		int y = get_y[f];
		int z = get_z[f];


		return z*n*n + y*n + x;
	}

	void InitIndexData(
		std::vector<GLuint>& index_data,
		const GLuint n,
		const GLuint n_1,
		const GLuint k
	)
	{
		assert(k != 0);
		assert(k < n);
		assert(n_1 % k == 0);

		auto ii = index_data.begin(), ie = index_data.end();

		for(GLuint f=0; f!=6; ++f)
		{
			for(GLuint j=0; j!=n_1; j+=k)
			{
				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f,  0, j+0);
				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f, -k, j+k);
				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f,  0, j+k);
				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f, +k, j-k);

				for(GLuint i=k; i!=n_1; i+=k)
				{
					assert(ii != ie);
					*ii++ = FaceIndex(n, n_1, f, i+0, j+0);
					assert(ii != ie);
					*ii++ = FaceIndex(n, n_1, f, i-k, j+2*k);
					assert(ii != ie);
					*ii++ = FaceIndex(n, n_1, f, i+0, j+k);
					assert(ii != ie);
					*ii++ = FaceIndex(n, n_1, f, i+k, j-k);
				}

				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f, n_1+0, j+0);
				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f, n_1-k, j+2*k);
				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f, n_1+0, j+k);
				assert(ii != ie);
				*ii++ = FaceIndex(n, n_1, f, n_1+k, j+0);

				// primitive restart index
				assert(ii != ie);
				*ii++ = vertex_count;
			}
		}

		assert(ii == ie);
	}

	void InitIndices(const GLuint n)
	{
		const GLuint n_1 = n-1;

		std::vector<GLuint> index_data(face_index_count);
		InitIndexData(index_data, n, n_1, 1);

		draw_vao.Bind();
		indices.Bind(se::ElementArray());
		Buffer::Data(se::ElementArray(), index_data);


		shadow_vao.Bind();
		shadow_indices.Bind(se::ElementArray());
		Buffer::Data(se::ElementArray(), index_data);
	}

	GLint SpringIndex(int x, int y, int z, int n) const
	{
		if(x <  0) return -1;
		if(x >= n) return -1;
		if(y <  0) return -1;
		if(y >= n) return -1;
		if(z <  0) return -1;
		if(z >= n) return -1;
		GLint index = z*n*n + y*n + x;
		assert(index >= 0);
		assert(index < GLint(vertex_count));
		return index;
	}

	void InitSprings(const JellyPhysProgram& phys_prog, const GLuint n)
	{
		std::vector<GLint> idx_data(vertex_count * (4*8));

		auto ii = idx_data.begin(), ie = idx_data.end();

		for(GLuint z=0; z!=n; ++z)
		{
			for(GLuint y=0; y!=n; ++y)
			{
				for(GLuint x=0; x!=n; ++x)
				{
					// SpringAX
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y+0, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y+0, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x-2, y+0, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+2, y+0, z+0, n);

					// SpringAY
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y-1, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+1, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y-2, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+2, z+0, n);

					// SpringAZ
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+0, z-1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+0, z+1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+0, z-2, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+0, z+2, n);

					// SpringBX
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+1, z-1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y+1, z+1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y-1, z+1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+0, y-1, z-1, n);

					// SpringBY
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y+0, z+1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y+0, z+1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y+0, z-1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y+0, z-1, n);

					// SpringBZ
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y-1, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y+1, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y+1, z+0, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y-1, z+0, n);

					// SpringCUp
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y+1, z-1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y+1, z-1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y+1, z+1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y+1, z+1, n);

					// SpringCDn
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y-1, z-1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y-1, z-1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x+1, y-1, z+1, n);
					assert(ii != ie);
					*ii++ = SpringIndex(x-1, y-1, z+1, n);
				}
			}
		}
		assert(ii == ie);

		springs.Bind(se::Uniform());

		Buffer::Data(se::Uniform(), idx_data);

		springs.BindBase(se::Uniform(), 1);

		phys_prog.Use();
		UniformBlock(phys_prog, "SpringIndexBlock").Binding(1);
	}
public:

	JellyCube(
		JellyPhysProgram& phys_prog,
		JellyDrawProgram& draw_prog,
		ShadowProgram& shadow_prog,
		CameraDriveProgram& cam_prog,
		const GLfloat size,
		const GLfloat mass,
		const GLuint n,
		const Mat4f& transform
	): vertex_count(n*n*n)
	 , face_index_count(6*(n-1)*(n*4+1))
	 , shadow_index_count(face_index_count)
	{
		assert(size > 0.0f);
		assert(n > 1);

		InitPositions(
			phys_prog,
			draw_prog,
			shadow_prog,
			cam_prog,
			size,
			mass,
			n,
			transform
		);
		InitIndices(n);
		InitVelocities(phys_prog);
		InitSprings(phys_prog, n);
	}

	void UpdatePhysics(JellyPhysProgram& phys_prog, double interval)
	{
		phys_vao.Bind();
		phys_prog.Use();

		GLuint n = 20;
		phys_prog.interval.Set(GLfloat(interval / n));

		tfb_positions.BindBase(se::TransformFeedback(), 0);
		tfb_velocities.BindBase(se::TransformFeedback(), 1);

		for(GLuint i=0; i!=n; ++i)
		{
			{
				TransformFeedbackPrimitiveType tfbmode = se::Points();
				TransformFeedback::Activator tfb(tfbmode);
				gl.DrawArrays(se::Points(), 0, vertex_count);
			}
			CopyFeedback(tfb_positions, positions);
			CopyFeedback(tfb_velocities, velocities);
		}
	}

	void Shadow(ShadowProgram& shadow_prog)
	{
		shadow_vao.Bind();
		shadow_prog.Use();

		gl.PrimitiveRestartIndex(vertex_count);
		gl.Enable(se::PrimitiveRestart());

		se::TriangleStripAdjacency tswa;
		gl.DrawElements(tswa, shadow_index_count, se::UnsignedInt());

		gl.Disable(se::PrimitiveRestart());
	}

	void Draw(JellyDrawProgram& draw_prog)
	{
		draw_vao.Bind();
		draw_prog.Use();

		gl.PrimitiveRestartIndex(vertex_count);
		gl.Enable(se::PrimitiveRestart());

		draw_prog.ambient_color.Set(Vec3f(0.3f, 0.4f, 0.2f));
		draw_prog.diffuse_color.Set(Vec3f(0.6f, 1.0f, 0.4f));

		se::TriangleStripAdjacency tswa;
		gl.DrawElements(tswa, face_index_count, se::UnsignedInt());

		draw_prog.ambient_color.Set(Vec3f(0.1f, 0.2f, 0.0f));
		draw_prog.diffuse_color.Set(Vec3f(0.3f, 0.4f, 0.2f));

		gl.Enable(se::PolygonOffsetLine());
		gl.PolygonMode(se::Line());
		gl.DrawElements(tswa, face_index_count, se::UnsignedInt());
		gl.PolygonMode(se::Fill());
		gl.Disable(se::PolygonOffsetLine());


		gl.Disable(se::PrimitiveRestart());
	}
};

class BulletTimeTrigger
{
private:
	double _remaining;
	bool _status;

	bool _started(void) const
	{
		return _remaining > 0.0;
	}

	bool _can_restart(void) const
	{
		return _remaining < -2.0;
	}
public:
	BulletTimeTrigger(void)
	 : _remaining(0.0)
	 , _status(false)
	{ }

	void UpdateAndStartIf(double interval, bool can_start, double duration)
	{
		_remaining -= interval;
		if(can_start && _can_restart())
			_remaining = duration;
	}

	bool On(void)
	{
		if(_started() && !_status)
		{
			_status = true;
			return true;
		}
		return false;
	}

	bool Off(void)
	{
		if(!_started() && _status)
		{
			_status = false;
			return true;
		}
		return false;
	}
};

class JellyExample : public Example
{
private:
	double prev_impulse_strength;

	// wrapper around the current OpenGL context
	Context gl;

	MetalProgram metal_prog;
	MetalFloor floor;

	CameraDriveProgram cam_prog;
	JellyPhysProgram phys_prog;
	JellyDrawProgram draw_prog;
	ShadowProgram shadow_prog;

	ChasingCamera camera;
	JellyCube jelly_cube;

	BulletTimeTrigger bt_trg;
public:
	JellyExample(const ExampleParams&)
	 : prev_impulse_strength(0.0)
	 , floor(metal_prog)
	 , camera(cam_prog, 0.05f, 3.5f, 10.0f)
	 , jelly_cube(
		phys_prog,
		draw_prog,
		shadow_prog,
		cam_prog,
		2.0f, 0.5f,
		8,
		ModelMatrixf::Translation(0.0f, 9.0f, 0.0f)*
		ModelMatrixf::RotationA(Vec3f(1.0f, 1.0f, 1.0f), Degrees(44.0f))
	)
	{
		phys_prog.impulse_strength.Set(0.0f);

		Vec3f light_pos(20.0f, 30.0f, 25.0f);
		ProgramUniform<Vec3f>(metal_prog, "LightPosition").Set(light_pos);
		ProgramUniform<Vec3f>(draw_prog, "LightPosition").Set(light_pos);
		ProgramUniform<Vec3f>(shadow_prog, "LightPosition").Set(light_pos);

		gl.ClearColor(0.6f, 0.6f, 0.6f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.ClearStencil(1);

		gl.Enable(se::DepthTest());
		gl.DepthFunc(se::LEqual());

		gl.Enable(se::CullFace());
		gl.FrontFace(se::CW());
		gl.CullFace(se::Back());

		gl.PolygonOffset(-0.01f, -1.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		auto perspective = CamMatrixf::PerspectiveX(
			Degrees(70),
			float(width)/height,
			1, 200
		);

		metal_prog.projection_matrix.Set(perspective);
		draw_prog.projection_matrix.Set(perspective);
		shadow_prog.projection_matrix.Set(perspective);
	}

	void UpdateImpulse(double time, Vec3f pos)
	{
		float impulse_strength = float(100.0 * (-SineWave(time / 5.0) - 0.99));
		if(impulse_strength < 0) impulse_strength = 0;
		impulse_strength = float(pow(impulse_strength, 12.0) * 2.0);

		if(impulse_strength != 0.0)
		{
			if(prev_impulse_strength == 0.0)
			{
				phys_prog.impulse_center.Set(Vec3f(
					+4.5f-float(std::rand())/float(RAND_MAX)*9.0f+pos.x(),
					-4.5f-float(std::rand())/float(RAND_MAX),
					+4.5f-float(std::rand())/float(RAND_MAX)*9.0f+pos.z()
				));
			}
			phys_prog.impulse_strength.Set(impulse_strength);
		}
		prev_impulse_strength = impulse_strength;
	}

	void Render(ExampleClock& clock)
	{
		gl.Clear().ColorBuffer().DepthBuffer().StencilBuffer();

		double interval = clock.Interval().Seconds();
		jelly_cube.UpdatePhysics(phys_prog, interval);
		camera.UpdatePhysics(cam_prog, interval);

		// update the 'bullet-time' trigger
		bt_trg.UpdateAndStartIf(
			clock.Interval().Seconds(),
			camera.Target().y() < 1.0f &&
			clock.RealTime().Second() < 20,
			1.5
		);

		// slow down the time if necessary
		if(bt_trg.On()) clock.Pace(0.1);
		// restore normal time flow if necessary
		if(bt_trg.Off())clock.Pace(1.0);


		auto cam_matrix = camera.Matrix();

		shadow_prog.camera_matrix.Set(cam_matrix);
		metal_prog.camera_matrix.Set(cam_matrix);
		draw_prog.camera_matrix.Set(cam_matrix);

		metal_prog.camera_position.Set(camera.Position());

		// Draw objects only with ambient light and the depth
		metal_prog.light_multiplier.Set(0.2f);
		draw_prog.light_multiplier.Set(0.2f);

		gl.CullFace(se::Back());
		gl.ColorMask(true, true, true, true);
		gl.DepthMask(true);
		gl.Disable(se::StencilTest());

		floor.Draw(metal_prog);
		jelly_cube.Draw(draw_prog);

		// Draw the shadow "volume" into the stencil buffer
		gl.ColorMask(false, false, false, false);
		gl.DepthMask(false);

		gl.Enable(se::StencilTest());
		gl.StencilFunc(se::Always(), 0);
		gl.StencilOpSeparate(se::Front(), se::Keep(), se::Keep(), se::Incr());
		gl.StencilOpSeparate(se::Back(),  se::Keep(), se::Keep(), se::Decr());

		gl.Disable(se::CullFace());
		jelly_cube.Shadow(shadow_prog);
		gl.Enable(se::CullFace());


		// Draw stencilled parts of objects only with full light
		metal_prog.light_multiplier.Set(1.0);
		draw_prog.light_multiplier.Set(1.0);

		gl.CullFace(se::Back());
		gl.ColorMask(true, true, true, true);
		gl.DepthMask(true);

		gl.StencilFunc(se::Equal(), 1);
		gl.StencilOp(se::Keep(), se::Keep(), se::Keep());

		gl.Clear().DepthBuffer();

		floor.Draw(metal_prog);
		jelly_cube.Draw(draw_prog);

		// update the force pulse kicking the cube
		UpdateImpulse(clock.Now().Seconds(), camera.Target());
	}

	bool Continue(const ExampleClock& clock)
	{
		return clock.RealTime().Minutes() < 2.0;
	}

	double HeatUpTime(void) const
	{
		return 0.0;
	}

	double ScreenshotTime(void) const
	{
		return 2.0;
	}

	double FrameTime(void) const
	{
		return 1.0/60.0;
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
	return std::unique_ptr<Example>(new JellyExample(params));
}

} // namespace oglplus
