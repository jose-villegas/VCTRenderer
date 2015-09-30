/**
 *  @example oglplus/033_steam.cpp
 *  @brief Shows how to simulate volumetric smoke with a patricle system
 *
 *  @oglplus_screenshot{033_steam}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/dsa/ext/framebuffer.hpp>
#include <oglplus/dsa/ext/renderbuffer.hpp>
#include <oglplus/dsa/ext/buffer.hpp>
#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/ext/vertex_array.hpp>
#include <oglplus/dsa/ext/vertex_attrib.hpp>

#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/random.hpp>
#include <oglplus/images/cloud.hpp>
#include <oglplus/images/image_spec.hpp>

#include <oglplus/opt/list_init.hpp>
#include <oglplus/opt/resources.hpp>

#include <vector>

#include "example.hpp"

namespace oglplus {

class NoiseTexture
 : public DSATextureEXT
{
public:
	const GLuint tex_unit;

	NoiseTexture(GLuint unit)
	 : tex_unit(unit)
	{
		this->target = TextureTarget::_2D;
		this->BindMulti(tex_unit);
		this->Image2D(images::RandomRGBUByte(256, 256));
		this->MinFilter(TextureMinFilter::Linear);
		this->MagFilter(TextureMagFilter::Linear);
		this->WrapS(TextureWrap::Repeat);
		this->WrapT(TextureWrap::Repeat);
	}
};

class CloudTexture
 : public DSATextureEXT
{
public:
	const GLuint tex_unit;

	CloudTexture(GLuint unit)
	 : tex_unit(unit)
	{
		this->target = TextureTarget::_2D;
		this->BindMulti(tex_unit);
		this->Image2D(
			images::Cloud2D(
				images::Cloud(
					128, 128, 128,
					Vec3f(),
					0.5f
				)
			)
		);
		this->GenerateMipmap();
		this->MinFilter(TextureMinFilter::LinearMipmapLinear);
		this->MagFilter(TextureMagFilter::Linear);
		this->BorderColor(Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
		this->WrapS(TextureWrap::ClampToBorder);
		this->WrapT(TextureWrap::ClampToBorder);
	}
};

class MeshProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Depth"));

		Shader vs(ShaderType::Vertex);
		vs.Source(
		"#version 150\n"

		"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"

		"const vec3 BackLight = vec3(0.0, 0.0,-20.0);"
		"const vec3 AmbiLight = vec3(0.0, 0.0, 7.0);"

		"in vec4 Position;"
		"in vec3 Normal;"
		"out vec3 vertNormal;"
		"out vec3 vertBackLtDir;"
		"out vec3 vertAmbiLtDir;"

		"void main(void)"
		"{"
		"	gl_Position = ModelMatrix * Position;"
		"	vertNormal = mat3(ModelMatrix) * Normal;"
		"	vertBackLtDir = BackLight - gl_Position.xyz;"
		"	vertAmbiLtDir = AmbiLight - gl_Position.xyz;"
		"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
		"}"
		).Compile();

		Shader fs(ShaderType::Fragment);
		fs.Source(
		"#version 150\n"

		"in vec3 vertNormal;"
		"in vec3 vertBackLtDir;"
		"in vec3 vertAmbiLtDir;"
		"out vec3 fragColor;"
		"void main(void)"
		"{"
		"	float bl = dot(normalize(vertNormal),normalize(vertBackLtDir));"
		"	float al = dot(normalize(vertNormal),normalize(vertAmbiLtDir));"
		"	bl = max(bl+0.1, 0.0);"
		"	al = max(al+0.1, 0.0)/length(vertAmbiLtDir);"
		"	fragColor = vec3(0.7, 0.6, 1.0)*(bl*0.2+al);"
		"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return std::move(prog);
	}

	Program& self(void){ return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	MeshProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_matrix(self(), "ModelMatrix")
	{ }
};

class ParticlePhysicsProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("ParticlePhysics"));

		Shader vs(ShaderType::Vertex);
		vs.Source(
		"#version 150\n"

		"uniform vec4 EmitPosAndCoef;"
		"uniform vec4 EmitDirAndCoef;"
		"uniform float AgeMult;"
		"uniform float deltaT;"
		"uniform sampler2D NoiseTex;"

		"in vec4 PositionAndId;"
		"in vec4 VelocityAndAge;"

		"out vec4 xfbPositionAndId;"
		"out vec4 xfbVelocityAndAge;"

		"void main(void)"
		"{"
		"	int id1 = int(gl_VertexID)%256;"
		"	int id2 = int(PositionAndId.w)%256;"
		"	int id3 = id1 ^ id2;"
		"	float age = VelocityAndAge.w;"
		"	age += deltaT * AgeMult;"

		"	if(age > 1.0)"
		"	{"
		"		vec3 rand1 = texelFetch(NoiseTex, ivec2(id1, id2), 0).rgb;"
		"		vec3 rand2 = texelFetch(NoiseTex, ivec2(id1, id3), 0).rgb;"

		"		xfbPositionAndId = vec4("
		"			EmitPosAndCoef.xyz+"
		"			EmitPosAndCoef.w*(rand1-0.5),"
		"			id2+1"
		"		);"
		"		xfbVelocityAndAge = vec4("
		"			EmitDirAndCoef.xyz+"
		"			EmitDirAndCoef.w*(rand2-0.5),"
		"			0.0"
		"		);"
		"	}"
		"	else"
		"	{"
		"		float drag = mix(0.0, 0.4, pow(age, 2.0))*deltaT;"

		"		xfbPositionAndId = vec4("
		"			VelocityAndAge.xyz*deltaT+"
		"			PositionAndId.xyz,"
		"			PositionAndId.w"
		"		);"
		"		xfbVelocityAndAge = vec4("
		"			VelocityAndAge.xyz*(1.0-drag),"
		"			age"
		"		);"
		"	}"
		"}"
		).Compile();

		prog.AttachShader(vs);

		const GLchar* variable_names[2] = {
			"xfbPositionAndId",
			"xfbVelocityAndAge"
		};
		prog.TransformFeedbackVaryings(
			variable_names,
			TransformFeedbackMode::SeparateAttribs
		);

		prog.Link().Use();

		return std::move(prog);
	}

	Program& self(void){ return *this; }
public:
	ProgramUniform<Vec4f> emit_pos_and_coef, emit_dir_and_coef;
	ProgramUniform<GLfloat> age_mult;
	ProgramUniform<GLfloat> delta_t;

	ParticlePhysicsProg(GLuint noise_tex)
	 : Program(make())
	 , emit_pos_and_coef(self(), "EmitPosAndCoef")
	 , emit_dir_and_coef(self(), "EmitDirAndCoef")
	 , age_mult(self(), "AgeMult")
	 , delta_t(self(), "deltaT")
	{
		UniformSampler(self(), "NoiseTex").Set(noise_tex);
	}
};

class PatricleVolumeProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("PatricleVolume"));

		Shader vs(ShaderType::Vertex);
		vs.Source(
		"#version 150\n"
		"in vec4 PositionAndId;"
		"in vec4 VelocityAndAge;"
		"out int vertID1, vertID2;"
		"out float vertAge;"
		"void main(void)"
		"{"
		"	gl_Position = vec4(PositionAndId.xyz, 1.0);"
		"	vertID1 = int(gl_VertexID)%256;"
		"	vertID2 = int(PositionAndId.w)%256;"
		"	vertAge = VelocityAndAge.w;"
		"}"
		).Compile();

		Shader gs(ShaderType::Geometry);
		gs.Source(
		"#version 150\n"
		"layout(points) in;"
		"layout(triangle_strip, max_vertices = 16) out;"

		"uniform mat4 ProjectionMatrix;"
		"uniform mat4 CameraMatrix;"
		"mat4 InvCameraMatrix = inverse(CameraMatrix);"
		"uniform mat4 OcclMapMatrix;"
		"uniform sampler2D NoiseTex;"
		"uniform int FrameID;"

		"in int vertID1[1];"
		"in int vertID2[1];"
		"in float vertAge[1];"

		"out vec4 geomGFTCoord;"
		"out vec2 geomTexCoord;"
		"out float geomAge;"

		"void main(void)"
		"{"
		"	float s0 = 4;"
		"	float s1 = 20;"
		"	vec3 rand = texelFetch("
		"		NoiseTex, "
		"		ivec2(vertID1[0],vertID2[0]),"
		"		0"
		"	).rgb;"

		"	vec2 oc = vec2(-1.0, 1.0);"

		"	float angle = rand.r*4.0*3.1415+(rand.g-0.5)*vertAge[0]*11;"
		"	float cx = cos(angle), sx = sin(angle);"
		"	mat2 rot = mat2(cx, sx, -sx, cx);"

		"	for(int j=0;j!=2;++j)"
		"	for(int i=0;i!=2;++i)"
		"	{"
		"		vec2 offs = vec2(oc[i], oc[j]);"
		"		offs *= mix(s0, s1, pow(vertAge[0], 2.0));"
		"		offs = rot * offs;"

		"		gl_Position = CameraMatrix * gl_in[0].gl_Position;"
		"		gl_Position += vec4(offs, 0, 0);"
		"		geomGFTCoord = OcclMapMatrix * InvCameraMatrix * gl_Position;"
		"		gl_Position = ProjectionMatrix * gl_Position;"

		"		geomTexCoord = vec2(float(i), float(j));"

		"		geomAge = vertAge[0];"
		"		EmitVertex();"
		"	}"
		"	EndPrimitive();"
		"}"
		).Compile();

		Shader fs(ShaderType::Fragment);
		fs.Source(
		"#version 150\n"
		"uniform mat4 CameraMatrix;"
		"uniform sampler2D OccludeTex;"
		"uniform sampler2D CloudTex;"

		"in vec4 geomGFTCoord;"
		"in vec2 geomTexCoord;"
		"in float geomAge;"
		"out vec2 fragColor;"

		"void main(void)"
		"{"
		"	vec3 GFTCoord = (geomGFTCoord.xyz/geomGFTCoord.w)*0.5 + 0.5;"
		"	float gf = texture(OccludeTex, GFTCoord.xy).r - GFTCoord.z;"
		"	float gf1 = (gf>0.0)?1.0:min(-gf*5, 1.0);"
		"	float gf2 = (gf>0.0)?1.0:min(-gf*4, 1.0);"
		"	gf = max(gf, 0);"
		"	float a0 = 1.0-pow(geomAge, 0.125);"
		"	float a1 = max((geomAge-0.3)*pow(0.99-geomAge, 0.25), 0.0);"
		"	float d = texture(CloudTex, geomTexCoord).r*0.5;"
		"	fragColor.r = d*(a0*(gf1*0.3+gf2*0.1)+a1*0.05);"
		"	fragColor.g = d*a0*a1*gf*2;"
		"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(gs).AttachShader(fs).Link().Use();

		return std::move(prog);
	}

	Program& self(void){ return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, occl_map_matrix;

	PatricleVolumeProg(GLuint occlude_tex, GLuint noise_tex, GLuint cloud_tex)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , occl_map_matrix(self(), "OcclMapMatrix")
	{
		UniformSampler(self(), "OccludeTex").Set(occlude_tex);
		UniformSampler(self(), "NoiseTex").Set(noise_tex);
		UniformSampler(self(), "CloudTex").Set(cloud_tex);
	}
};

class CompositeProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Composite"));

		Shader vs(ShaderType::Vertex);
		vs.Source(
		"#version 150\n"
		"uniform vec2 ScreenSize;"
		"in vec4 Position;"
		"in vec2 TexCoord;"
		"out vec2 vertTexCoord;"

		"void main(void)"
		"{"
		"	gl_Position = Position;"
		"	vertTexCoord = TexCoord*ScreenSize;"
		"}"
		).Compile();

		Shader fs(ShaderType::Fragment);
		fs.Source(
		"#version 150\n"

		"uniform sampler2DRect GeomTex, VolmTex;"

		"in vec2 vertTexCoord;"
		"out vec3 fragColor;"

		"void main(void)"
		"{"
		"	vec3 geom = texture(GeomTex, vertTexCoord).rgb;"
		"	vec2 volm = texture(VolmTex, vertTexCoord).rg;"
		"	float d = min(pow(volm.r*0.5, 2.00), 2.0);"
		"	float l = volm.g;"
		"	vec3 volc = vec3(1.0, 1.0, 1.0) * mix(0.5, 0.8, l);"
		"	fragColor = mix(geom, volc, d*0.7);"
		"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return std::move(prog);
	}

	Program& self(void) { return *this; }
public:
	ProgramUniform<Vec2f> screen_size;

	CompositeProg(GLuint geom_tex, GLuint volm_tex)
	 : Program(make())
	 , screen_size(self(), "ScreenSize")
	{
		ProgramUniformSampler(self(), "GeomTex").Set(geom_tex);
		ProgramUniformSampler(self(), "VolmTex").Set(volm_tex);
	}
};

class Geometry
{
private:
	MeshProg mesh_prog;
	ResourceFile mesh_input;
	shapes::ObjMesh mesh_loader;
	shapes::ShapeWrapper meshes;
	GLuint fan_index;
public:
	Geometry(void)
	 : mesh_prog()
	 , mesh_input("models", "large_fan", ".obj")
	 , mesh_loader(
		mesh_input,
		shapes::ObjMesh::LoadingOptions(false).Normals()
	), meshes(List("Position")("Normal").Get(), mesh_loader, mesh_prog)
	 , fan_index(mesh_loader.GetMeshIndex("Fan"))
	{ }

	void Use(void)
	{
		meshes.Use();
		mesh_prog.Use();
	}

	void Render(const Mat4f& projection, const Mat4f& camera, double time)
	{
		mesh_prog.projection_matrix.Set(projection);
		mesh_prog.camera_matrix.Set(camera);

		ProgramUniform<Mat4f>* pmodel_matrix = &mesh_prog.model_matrix;
		auto mm_nomotion = ModelMatrixf();
		auto mm_rotation = ModelMatrixf::RotationZ(FullCircles(time / 2.0));

		GLuint drawing_fan = fan_index;
		const auto drawing_driver =
			[
				&drawing_fan,
				&mm_nomotion,
				&mm_rotation,
				&pmodel_matrix
			](GLuint phase) -> bool
			{
				if(phase == drawing_fan)
					pmodel_matrix->Set(mm_rotation);
				else pmodel_matrix->Set(mm_nomotion);
				return true;
			};
		meshes.Draw(drawing_driver);
	}
};

class OcclusionMap
{
private:
	Context gl;

	DSATextureEXT tex;
	DSARenderbufferEXT rbo;
	DSAFramebufferEXT fbo;
	const GLuint size;
public:
	const GLuint tex_unit;

	const Mat4f projection;
	const Mat4f camera;

	OcclusionMap(GLuint unit, GLsizei side)
	 : size(side)
	 , tex_unit(unit)
	 , projection(CamMatrixf::Ortho(-7, +7, -7, +7, 5, 30))
	 , camera(CamMatrixf::LookingAt(Vec3f( 0, 0,-7), Vec3f()))
	{
		tex	<< (tex_unit|TextureTarget::_2D)
			<< TextureFilter::Nearest
			<< TextureWrap::ClampToEdge
			<< images::ImageSpec(
				size, size,
				Format::DepthComponent,
				InternalFormat::DepthComponent32,
				DataType::Float
			);

		rbo	<< RenderbufferTarget::Renderbuffer
			<< images::ImageSpec(size, size, InternalFormat::Red);

		fbo	<< FramebufferTarget::Draw
			<< FramebufferAttachment::Depth << tex
			<< FramebufferAttachment::Color << rbo
			<< FramebufferComplete();
	}

	void Update(Geometry& geometry, double time)
	{
		fbo.Bind(FramebufferTarget::Draw);

		gl.Viewport(size, size);

		gl.DepthMask(true);
		gl.Clear().DepthBuffer();

		geometry.Render(projection, camera, time);
	}
};

class ParticleSystem
{
private:
	Context gl;

	NoiseTexture noise_tex;
	CloudTexture cloud_tex;

	ParticlePhysicsProg pp_prog;
	PatricleVolumeProg pv_prog;

	Array<DSAVertexArrayEXT> pp_vao, pv_vao;
	Array<DSABufferEXT> pos_and_id_buf, vel_and_age_buf;

	GLuint particle_count;
	const GLfloat age_mult;

public:
	ParticleSystem(OcclusionMap& occl_map)
	 : gl()
	 , noise_tex(occl_map.tex_unit+1)
	 , cloud_tex(occl_map.tex_unit+2)
	 , pp_prog(noise_tex.tex_unit)
	 , pv_prog(occl_map.tex_unit, noise_tex.tex_unit, cloud_tex.tex_unit)
	 , pp_vao(2)
	 , pv_vao(2)
	 , pos_and_id_buf(2)
	 , vel_and_age_buf(2)
	 , particle_count(0)
	 , age_mult(0.15f)
	{
		pp_prog.emit_pos_and_coef.Set(0, 0, -9.0f, 1.5f);
		pp_prog.emit_dir_and_coef.Set(0, 0, 4.5f, 1.5f);
		pp_prog.age_mult = age_mult;

		const GLuint max_particle_count = 1024;
		double prev_spawn = 0.0;

		std::vector<GLfloat> v(max_particle_count*4, 2);
		// pos/id
		pos_and_id_buf[0].Data(v);
		pos_and_id_buf[1].Data(v);

		// dir/age
		vel_and_age_buf[0].Data(v);
		vel_and_age_buf[1].Data(v);

		v.clear();

		for(GLint i=0; i!=2; ++i)
		{
			DSAVertexArrayAttribEXT(pp_vao[i], pp_prog, "PositionAndId")
				.Setup<Vec4f>(pos_and_id_buf[i])
				.Enable();
			DSAVertexArrayAttribEXT(pv_vao[i], pv_prog, "PositionAndId")
				.Setup<Vec4f>(pos_and_id_buf[i])
				.Enable();

			DSAVertexArrayAttribEXT(pp_vao[i], pp_prog, "VelocityAndAge")
				.Setup<Vec4f>(vel_and_age_buf[i])
				.Enable();
			DSAVertexArrayAttribEXT(pv_vao[i], pv_prog, "VelocityAndAge")
				.Setup<Vec4f>(vel_and_age_buf[i])
				.Enable();
		}

		double time = 0.0;
		const double time_diff = 1.0/25.0;
		double spawn_interval = 1.0/(age_mult*max_particle_count);
		GLuint frame_no = 0;
		while(particle_count < max_particle_count)
		{

			if(prev_spawn + spawn_interval < time)
			{
				++particle_count;
				prev_spawn = time;
			}
			Update(time_diff, frame_no);
			time += time_diff;
			frame_no++;
		}
	}

	void Update(double time_diff, GLuint frame_no)
	{
		int vao_i = (frame_no+0)%2;
		int xfb_i = (frame_no+1)%2;

		pp_vao[vao_i].Bind();
		pos_and_id_buf[xfb_i].BindBase(Buffer::IndexedTarget::TransformFeedback, 0);
		vel_and_age_buf[xfb_i].BindBase(Buffer::IndexedTarget::TransformFeedback, 1);

		pp_prog.Use();
		pp_prog.delta_t = GLfloat(time_diff);

		TransformFeedback::Activator xfb_act(TransformFeedbackPrimitiveType::Points);
		gl.DrawArrays(PrimitiveType::Points, 0, particle_count);
		xfb_act.Finish();
	}

	void Render(
		const Mat4f& projection,
		const Mat4f& camera,
		const Mat4f& occl_mat,
		GLuint frame_no
	)
	{
		pv_vao[(frame_no+1)%2].Bind();
		pv_prog.Use();
		pv_prog.occl_map_matrix.Set(occl_mat);
		pv_prog.projection_matrix.Set(projection);
		pv_prog.camera_matrix.Set(camera);

		gl.DrawArrays(PrimitiveType::Points, 0, particle_count);
	}
};

class ParticlesExample : public Example
{
private:
	Context gl;

	GLuint geom_tex_unit;
	GLuint volm_tex_unit;

	Geometry geometry;
	OcclusionMap occl_map;
	ParticleSystem steam;

	DSATextureEXT geom_tex;
	DSATextureEXT volm_tex;

	DSARenderbufferEXT depth_rbo;

	DSAFramebufferEXT geom_fbo;
	DSAFramebufferEXT volm_fbo;

	CompositeProg screen_prog;
	shapes::ShapeWrapper screen;

	GLdouble prev_time;
	GLuint frame_no;

	GLuint width, height;

	Mat4f projection;
public:
	ParticlesExample(void)
	 : gl()
	 , geom_tex_unit(0)
	 , volm_tex_unit(1)
	 , geometry()
	 , occl_map(volm_tex_unit+1, 256)
	 , steam(occl_map)
	 , screen_prog(geom_tex_unit, volm_tex_unit)
	 , screen(List("Position")("TexCoord").Get(), shapes::Screen(), screen_prog)
	 , prev_time(0.0)
	 , frame_no(0)
	 , width(400)
	 , height(300)
	{
		geom_tex<< (geom_tex_unit|TextureTarget::Rectangle)
			<< TextureFilter::Nearest
			<< TextureWrap::ClampToEdge;

		volm_tex<< (volm_tex_unit|TextureTarget::Rectangle)
			<< TextureFilter::Nearest
			<< TextureWrap::ClampToEdge;

		depth_rbo
			<< RenderbufferTarget::Renderbuffer;

		Reshape(width, height);

		geom_fbo<< FramebufferTarget::Draw
			<< FramebufferAttachment::Color << geom_tex
			<< FramebufferAttachment::Depth << depth_rbo
			<< FramebufferComplete();

		volm_fbo<< FramebufferTarget::Draw
			<< FramebufferAttachment::Color << volm_tex
			<< FramebufferAttachment::Depth << depth_rbo
			<< FramebufferComplete();

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.BlendFunc(BlendFn::One, BlendFn::One);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		screen_prog.screen_size.Set(width, height);

		projection =
			CamMatrixf::PerspectiveX(
				Degrees(60),
				width, height,
				1, 100
			);

		geom_tex
			<< images::ImageSpec(
				width, height,
				Format::RGB,
				InternalFormat::RGB,
				DataType::UnsignedByte
			);

		volm_tex
			<< images::ImageSpec(
				width, height,
				Format::RG,
				InternalFormat::RG32F,
				DataType::Float
			);

		depth_rbo
			<< images::ImageSpec(
				width, height,
				InternalFormat::DepthComponent32
			);
	}

	void Render(double time)
	{
		double time_diff = (time - prev_time);
		auto camera =
			CamMatrixf::Roll(
				Degrees(
					SineWave(time/11.0)*9+
					SineWave(time/13.0)*7
				)
			)*
			CamMatrixf::Orbiting(
				Vec3f(),
				40.0f,
				Degrees(
					SineWave(time/23.0)*35+
					CosineWave(time/31.0)*45-90
				),
				Degrees(
					SineWave(time/19.0)*35+
					SineWave(time/26.0)*45
				)
			);


		gl.Enable(Capability::DepthTest);
		geometry.Use();

		occl_map.Update(geometry, time);

		gl.Viewport(width, height);

		geom_fbo.Bind(FramebufferTarget::Draw);

		gl.DepthMask(true);
		gl.Clear().ColorBuffer().DepthBuffer();

		gl.Disable(Capability::Blend);
		geometry.Render(projection, camera, time);

		steam.Update(time_diff, frame_no);

		volm_fbo.Bind(FramebufferTarget::Draw);

		gl.DepthMask(false);
		gl.Clear().ColorBuffer();

		gl.Enable(Capability::Blend);

		steam.Render(
			projection,
			camera,
			occl_map.projection*
			occl_map.camera,
			frame_no
		);

		DefaultFramebuffer().Bind(FramebufferTarget::Draw);
		gl.Disable(Capability::DepthTest);
		gl.Disable(Capability::Blend);

		screen_prog.Use();
		screen.Use();
		screen.Draw();

		prev_time = time;
		frame_no++;
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}

	double ScreenshotTime(void) const
	{
		return 10.0;
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
	return std::unique_ptr<Example>(new ParticlesExample);
}

} // namespace oglplus
