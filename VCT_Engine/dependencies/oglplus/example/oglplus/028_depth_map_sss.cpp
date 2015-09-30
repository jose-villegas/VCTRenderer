/**
 *  @example oglplus/028_depth_map_sss.cpp
 *  @brief Shows depth-map-based simulation of subsurface scattering
 *
 *  @oglplus_screenshot{028_depth_map_sss}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 *  @oglplus_example_uses_model{stanford_dragon}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/image_spec.hpp>

#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class DepthProgram
 : public Program
{
private:
	const Program& prog(void) const { return *this; }

	static Program make(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4  CameraMatrix,ModelMatrix;"

			"in vec4 Position;"

			"void main(void)"
			"{"
			"	gl_Position = CameraMatrix * ModelMatrix * Position;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 150\n"
			"void main(void)"
			"{"
			"}"
		).Compile();

		Program prog;
		prog << vs << fs;
		prog.Link().Use();

		return std::move(prog);
	}
public:
	Uniform<Mat4f> camera_matrix, model_matrix;

	DepthProgram(void)
	 : Program(make())
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	{ }
};

class ShapeProgram
 : public Program
{
private:
	const Program& prog(void) const { return *this; }

	static Program make(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4  LightMatrix, ModelMatrix;"
			"uniform vec3 LightPosition;"
			"uniform vec3 CameraPosition;"

			"in vec4 Position;"

			"out vec4 vertDepthCoord;"
			"out vec3 vertLightDir;"
			"out vec3 vertViewDir;"

			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertDepthCoord = LightMatrix * gl_Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertViewDir = CameraPosition - gl_Position.xyz;"
			"}"
		).Compile();

		GeometryShader gs;
		gs.Source(
			"#version 150\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices=3) out;"

			"uniform mat4 CameraMatrix;"

			"in vec4 vertDepthCoord[3];"
			"in vec3 vertLightDir[3];"
			"in vec3 vertViewDir[3];"

			"out vec4 geomDepthCoord;"
			"out vec3 geomLightDir;"
			"out vec3 geomViewDir;"
			"out vec3 geomNormal;"

			"void main(void)"
			"{"
			"	geomNormal = normalize("
			"		cross("
			"			gl_in[1].gl_Position.xyz-"
			"			gl_in[0].gl_Position.xyz,"
			"			gl_in[2].gl_Position.xyz-"
			"			gl_in[0].gl_Position.xyz "
			"		)"
			"	);"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		gl_Position = CameraMatrix * gl_in[v].gl_Position;"
			"		geomDepthCoord = vertDepthCoord[v];"
			"		geomLightDir = vertLightDir[v];"
			"		geomViewDir = vertViewDir[v];"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 150\n"

			"uniform sampler2D DepthMap;"

			"uniform vec2 DepthOffs[32];"
			"const int DepthSamples = 32;"
			"const float InvDepthSamples = 1.0 / DepthSamples;"

			"in vec4 gl_FragCoord;"

			"in vec4 geomDepthCoord;"
			"in vec3 geomLightDir;"
			"in vec3 geomViewDir;"
			"in vec3 geomNormal;"

			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	float LightDist = geomDepthCoord.z/geomDepthCoord.w;"
			"	vec3 Normal = normalize(geomNormal);"
			"	vec3 LightDir = normalize(geomLightDir);"
			"	vec3 LightRefl  = reflect(-LightDir, Normal);"
			"	vec3 ViewDir = normalize(geomViewDir);"

			"	float inv_w = 1.0/geomDepthCoord.w;"

			"	float Depth = 0.0;"
			"	for(int s=0; s!=DepthSamples; ++s)"
			"	{"
			"		vec2 SampleCoord = DepthOffs[s]+geomDepthCoord.xy;"
			"		SampleCoord *= inv_w;"
			"		SampleCoord *= 0.5;"
			"		SampleCoord += 0.5;"
			"		float Sample = texture(DepthMap, SampleCoord).r;"
			"		if(Sample < 0.95)"
			"			Depth += Sample;"
			"		else Depth += 0.5;"
			"	}"
			"	Depth *= InvDepthSamples;"

			"	float Ambi = 0.15;"
			"	float BkLt = (dot(-LightDir, ViewDir)+3.0)*0.25;"
			"	float SuSS = pow(abs(Depth-LightDist), 2.0)*BkLt*1.2;"
			"	float Shdw = min(pow(abs(Depth-LightDist)*2.0, 8.0), 1.0);"
			"	float Diff  = sqrt(max(dot(LightDir, Normal)+0.1, 0.0))*0.4;"
			"	float Spec  = pow(max(dot(LightRefl, ViewDir), 0.0), 64.0);"
			"	vec3 Color = vec3(0.2, 0.9, 0.7);"
			"	fragColor = (Ambi + Shdw*Diff + SuSS) * Color;"
			"	fragColor += Shdw*Spec * vec3(1.0, 1.0, 1.0);"
			"}"
		).Compile();

		Program prog;
		prog << vs << gs << fs;
		prog.Link().Use();

		ProgramUniform<Vec2f> depth_offs(prog, "DepthOffs");
		for(GLuint i=0; i!=32; ++i)
		{
			float u = std::rand() / float(RAND_MAX);
			float v = std::rand() / float(RAND_MAX);
			float x = float(std::sqrt(v) * std::cos(2*3.1415*u));
			float y = float(std::sqrt(v) * std::sin(2*3.1415*u));
			depth_offs[i].Set(x, y);
		}

		return std::move(prog);
	}
public:
	Uniform<Mat4f> camera_matrix, light_matrix, model_matrix;
	Uniform<Vec3f> camera_position, light_position;

	ShapeProgram(void)
	 : Program(make())
	 , camera_matrix(prog(), "CameraMatrix")
	 , light_matrix(prog(), "LightMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	{ }
};

class Shape
 : public shapes::ShapeWrapper
{
private:
	static shapes::ShapeWrapper load(const char* name)
	{
		std::ifstream input;
		OpenResourceFile(input, "models", name, ".obj");
		if(!input.good())
			throw std::runtime_error("Error opening file for reading");

		return shapes::ShapeWrapper(
			List("Position").Get(),
			shapes::ObjMesh(
				input,
				shapes::ObjMesh::LoadingOptions(false)
			)
		);
	}
public:
	Shape(const char* name)
	 : shapes::ShapeWrapper(load(name))
	{ }
};

class DepthBuffer
{
private:
	Context gl;
	Texture tex;
	Framebuffer fbo;
	Renderbuffer rbo;
	GLuint side;
public:
	DepthBuffer(const Program& prog, GLuint tex_unit, GLuint tex_side)
	 : side(tex_side)
	{
		Texture::Active(tex_unit);
		ProgramUniformSampler(prog, "DepthMap").Set(tex_unit);

		tex	<< Texture::Target::_2D
			<< TextureFilter::Nearest
			<< TextureWrap::ClampToEdge
			<< images::ImageSpec(
				side, side,
				Format::DepthComponent,
				InternalFormat::DepthComponent32,
				PixelDataType::Float
			);

		rbo	<< Renderbuffer::Target::Renderbuffer
			<< images::ImageSpec(side, side, InternalFormat::R8);

		fbo	<< Framebuffer::Target::Draw
			<< FramebufferAttachment::Color << rbo
			<< FramebufferAttachment::Depth << tex
			<< FramebufferComplete();
	}

	void SetupViewport(void)
	{
		gl.Viewport(side, side);
	}

	void Bind(void)
	{
		fbo.Bind(Framebuffer::Target::Draw);
	}
};

class SubsurfExample : public Example
{
private:
	Context gl;

	Shape shape;

	DepthProgram depth_prog;
	ShapeProgram draw_prog;


	VertexArray depth_vao, draw_vao;

	DepthBuffer depth_buffer;

	GLuint width, height;
public:
	SubsurfExample(void)
	 : gl()
	 , shape("stanford_dragon")
	 , depth_prog()
	 , draw_prog()
	 , depth_vao(shape.VAOForProgram(depth_prog))
	 , draw_vao(shape.VAOForProgram(draw_prog))
	 , depth_buffer(draw_prog, 0, 1024)
	{
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
		gl.PolygonOffset(1.0f, 1.0f);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
	}

	void Render(double time)
	{

		GLfloat bs_rad = shape.BoundingSphere().Radius()*1.2f;

		auto light =
			CamMatrixf::Orbiting(
				shape.BoundingSphere().Center(),
				shape.BoundingSphere().Radius()*10.0f,
				FullCircles(time / 23.0),
				Degrees(35 + SineWave(time / 31.0) * 50)
			);

		GLfloat lgt_tgt_dist = Distance(
			shape.BoundingSphere().Center(),
			light.Position()
		);

		auto lgt_proj =
			CamMatrixf::PerspectiveX(
				ArcSin(bs_rad/lgt_tgt_dist)*2,
				1.0f,
				lgt_tgt_dist-bs_rad,
				lgt_tgt_dist+bs_rad
			);

		auto camera =
			CamMatrixf::Orbiting(
				shape.BoundingSphere().Center(),
				shape.BoundingSphere().Radius()*2.8f,
				FullCircles(time / 17.0),
				Degrees(SineWave(time / 19.0) * 80)
			);

		GLfloat cam_tgt_dist = Distance(
			shape.BoundingSphere().Center(),
			camera.Position()
		);

		auto cam_proj =
			CamMatrixf::PerspectiveX(
				ArcSin(bs_rad/cam_tgt_dist)*2,
				width, height,
				cam_tgt_dist-bs_rad,
				cam_tgt_dist+bs_rad
			);

		auto model =
			ModelMatrixf::RotationZ(Degrees(SineWave(time / 21.0)*25))*
			ModelMatrixf::Translation(0.0f,-bs_rad*0.25f, 0.0f);

		depth_vao.Bind();
		depth_prog.Use();
		depth_prog.camera_matrix.Set(lgt_proj*light);
		depth_prog.model_matrix.Set(model);

		depth_buffer.SetupViewport();
		depth_buffer.Bind();

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Clear().ColorBuffer().DepthBuffer();
		gl.Enable(Capability::PolygonOffsetFill);

		shape.Draw();

		//
		draw_vao.Bind();
		draw_prog.Use();
		draw_prog.camera_matrix.Set(cam_proj*camera);
		draw_prog.light_matrix.Set(lgt_proj*light);
		draw_prog.model_matrix.Set(model);
		draw_prog.camera_position.Set(camera.Position());
		draw_prog.light_position.Set(light.Position());

		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);
		gl.Viewport(width, height);
		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Clear().ColorBuffer().DepthBuffer();
		gl.Disable(Capability::PolygonOffsetFill);

		shape.Draw();
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double ScreenshotTime(void) const
	{
		return 29.0;
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
	return std::unique_ptr<Example>(new SubsurfExample);
}

} // namespace oglplus
