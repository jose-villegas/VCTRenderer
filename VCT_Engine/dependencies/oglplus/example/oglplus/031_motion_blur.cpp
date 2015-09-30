/**
 *  @example oglplus/031_motion_blur.cpp
 *  @brief Shows accumulation-based motion blur effect
 *
 *  @oglplus_screenshot{031_motion_blur}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_model{arrow_z}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/checker.hpp>

#include <oglplus/opt/list_init.hpp>
#include <oglplus/opt/resources.hpp>

#include <algorithm>
#include <cassert>

#include "example.hpp"

namespace oglplus {

class Instances
{
private:
	static std::vector<Vec3f> make_positions(void)
	{
		std::vector<Vec3f> pos_data;
		pos_data.reserve(17);

		pos_data.push_back(Vec3f( 10.0f,  0.0f, 70.0f));
		pos_data.push_back(Vec3f( 60.0f,  0.0f, 60.0f));
		pos_data.push_back(Vec3f( 95.0f,  0.0f,  0.0f));
		pos_data.push_back(Vec3f( 60.0f,  0.0f,-60.0f));
		pos_data.push_back(Vec3f( 10.0f,  0.0f,-70.0f));
		pos_data.push_back(Vec3f(-30.0f, 20.0f,-70.0f));
		pos_data.push_back(Vec3f(-70.0f, 40.0f,-70.0f));
		pos_data.push_back(Vec3f(-90.0f, 40.0f,-30.0f));
		pos_data.push_back(Vec3f(-30.0f, 40.0f, 10.0f));
		pos_data.push_back(Vec3f( 40.0f, 40.0f, 10.0f));
		pos_data.push_back(Vec3f( 90.0f,  0.0f,  0.0f));
		pos_data.push_back(Vec3f( 50.0f,-40.0f,-10.0f));
		pos_data.push_back(Vec3f(  0.0f,-40.0f,-10.0f));
		pos_data.push_back(Vec3f(-50.0f,-40.0f,-10.0f));
		pos_data.push_back(Vec3f(-90.0f,-40.0f, 30.0f));
		pos_data.push_back(Vec3f(-70.0f,-40.0f, 70.0f));
		pos_data.push_back(Vec3f(-30.0f,-20.0f, 70.0f));

		return pos_data;
	}
	CubicBezierLoop<Vec3f, double> path_pos;

	static std::vector<Vec3f> make_normals(void)
	{
		std::vector<Vec3f> nml_data;
		nml_data.reserve(17);

		nml_data.push_back(Vec3f(  0.0f,  1.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f, -1.0f));
		nml_data.push_back(Vec3f( -1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  2.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  1.0f));
		nml_data.push_back(Vec3f(  1.0f,  1.0f,  0.5f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  1.0f));
		nml_data.push_back(Vec3f(  1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f, -1.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  0.0f));
		nml_data.push_back(Vec3f(  1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f, -1.0f,  1.0f));
		nml_data.push_back(Vec3f(  0.0f,  0.0f,  1.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  1.0f));
		nml_data.push_back(Vec3f(  1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f, -1.0f));
		nml_data.push_back(Vec3f( -1.0f,  1.0f,  0.0f));

		for(auto i=nml_data.begin(),e=nml_data.end(); i!=e; ++i)
			*i = Normalized(*i);

		return nml_data;
	}
	CubicBezierLoop<Vec3f, double> path_nml;

	GLuint count;
public:
	Buffer matrix_buffer;

	Instances(GLuint n = 256)
	 : path_pos(make_positions(), 0.25)
	 , path_nml(make_normals(), 0.25)
	{
		count = 2*n;
		// inst_count x 4x4 matrices
		std::vector<GLfloat> matrix_data(count*16);
		auto p = matrix_data.begin();

		double step = 1.0/n;

		for(GLuint i=0; i!=n; ++i)
		{
			Vec3f pos = path_pos.Position(i*step);
			Vec3f tgt = Normalized(
				path_pos.Position((i+1)*step)-
				path_pos.Position((i-1)*step)
			);
			Vec3f tmp = path_nml.Position(i*step);
			Vec3f nml = Normalized(
				Dot(tmp, tgt) != 0.0?
				tmp-tgt*Dot(tmp, tgt):
				tmp
			);
			Vec3f btg = Cross(nml, tgt);

			for(GLuint j=0; j!=2; ++j)
			{
				const GLfloat s[2] = {-3.0f, 3.0f};
				Mat4f matrix =
					Mat4f(
						Vec4f(btg, 0),
						Vec4f(nml, 0),
						Vec4f(tgt, 0),
						Vec4f(pos+btg*s[j], 1)
					);

				p = std::copy(
					Data(matrix),
					Data(matrix)+
					Size(matrix),
					p
				);
			}
		}
		assert(p == matrix_data.end());

		matrix_buffer.Bind(Buffer::Target::Uniform);
		Buffer::Data(
			Buffer::Target::Uniform,
			matrix_data,
			BufferUsage::StaticDraw
		);
	}

	Vec3f Position(double t) const
	{
		return path_pos.Position(t);
	}

	Vec3f Normal(double t) const
	{
		return path_nml.Position(t);
	}

	Mat4f MakeMatrix(double t, double dt) const
	{
		Vec3f pos = Position(t);
		Vec3f tgt = Normalized(Position(t+dt)-Position(t-dt));
		Vec3f tmp = Normal(t+dt);
		float dtt = Dot(tmp, tgt);
		Vec3f nml = Normalized(dtt != 0.0 ? tmp-tgt*dtt : tmp);
		Vec3f btg = Cross(nml, tgt);

		return Transposed(Mat4f(
			Vec4f(btg, 0),
			Vec4f(nml, 0),
			Vec4f(tgt, 0),
			Vec4f(pos, 1)
		));
	}

	GLuint Count(void) const
	{
		return count;
	}
};

class DrawProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Draw"));

		VertexShader vs(ObjectDesc("Draw"));
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, SingleModelMatrix;"
			"uniform uint SingleModel;"
			"layout (std140) uniform ModelBlock {"
			"	mat4 ModelMatrices[512];"
			"};"

			"const vec3 LightPos = vec3(0.0, 0.0, 0.0);"

			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"

			"out vec3 vertLightDir;"
			"out vec3 vertNormal;"
			"out vec3 vertColor;"
			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	mat4 ModelMatrix = "
			"		SingleModel!=0u?"
			"		SingleModelMatrix:"
			"		ModelMatrices[gl_InstanceID];"

			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = normalize(LightPos - gl_Position.xyz);"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertTexCoord = SingleModel!=0u ? Position.xz : TexCoord;"
			"	vertColor = abs(normalize((ModelMatrix*Position).yxz));"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();

		FragmentShader fs(ObjectDesc("Draw"));
		fs.Source(
			"#version 140\n"

			"uniform sampler2D CheckerTex;"

			"in vec3 vertLightDir;"
			"in vec3 vertNormal;"
			"in vec3 vertColor;"
			"in vec2 vertTexCoord;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float c = texture(CheckerTex, vertTexCoord).r;"
			"	float l = pow(max(dot(vertLightDir, vertNormal)+0.1,0.0)*1.6,2.0);"
			"	fragColor = mix("
			"		vec3(0, 0, 0),"
			"		vertColor,"
			"		mix(c, 1.0, 0.2)*(l+0.3)"
			"	);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	Uniform<GLuint> single_model;
	UniformSampler checker_tex;
	UniformBlock model_block;

	DrawProgram(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_matrix(self(), "SingleModelMatrix")
	 , single_model(self(), "SingleModel")
	 , checker_tex(self(), "CheckerTex")
	 , model_block(self(), "ModelBlock")
	{ }
};

class ArrowShape
 : public ResourceFile
 , public shapes::ObjMesh
{
private:
public:
	ArrowShape(void)
	 : ResourceFile("models", "arrow_z", ".obj")
	 , shapes::ObjMesh(stream(), LoadingOptions(false).Normals())
	{ }
};

class BlurProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Blur"));

		VertexShader vs(ObjectDesc("Blur"));
		vs.Source(
			"#version 140\n"

			"uniform vec2 ScreenSize;"

			"in vec4 Position;"
			"in vec2 TexCoord;"

			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertTexCoord = ScreenSize*TexCoord;"
			"}"
		).Compile();

		FragmentShader fs(ObjectDesc("Blur"));
		fs.Source(
			"#version 140\n"

			"uniform sampler2DRect CurrentFrame;"
			"uniform sampler2DRect PreviousFrames;"
			"uniform float Splitter;"

			"in vec2 vertTexCoord;"

			"out vec3 fragColor;"

			"vec3 sharp(void)"
			"{"
			"	return texture(CurrentFrame, vertTexCoord).rgb;"
			"}"

			"vec3 blurred(void)"
			"{"
			"	vec3 prev = vec3(0, 0, 0);"
			"	const vec2 otc[9] = vec2[9]("
			"		vec2(-1,-1),"
			"		vec2( 0,-1),"
			"		vec2( 1,-1),"
			"		vec2(-1, 0),"
			"		vec2( 0, 0),"
			"		vec2( 1, 0),"
			"		vec2(-1, 1),"
			"		vec2( 0, 1),"
			"		vec2( 1, 1) "
			"	);"
			"	const float is = 1.0/9.0;"
			"	for(int s=0; s!=9; ++s)"
			"	{"
			"		vec2 tc = vertTexCoord+otc[s]*2;"
			"		prev += texture(PreviousFrames, tc).rgb * is;"
			"	}"
			"	vec3 curr = texture(CurrentFrame, vertTexCoord).rgb;"
			"	float a = curr.x+curr.y+curr.z;"
			"	return mix(curr, prev, 0.95-0.4*a);"
			"}"

			"void main(void)"
			"{"
			"	if(gl_FragCoord.x < Splitter-1)"
			"		fragColor = sharp();"
			"	else if(gl_FragCoord.x > Splitter+1)"
			"		fragColor = blurred();"
			"	else fragColor = vec3(1.0, 1.0, 1.0);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Vec2f> screen_size;
	UniformSampler current_frame, previous_frames;
	Uniform<GLfloat> splitter;

	BlurProgram(void)
	 : Program(make())
	 , screen_size(self(), "ScreenSize")
	 , current_frame(self(), "CurrentFrame")
	 , previous_frames(self(), "PreviousFrames")
	 , splitter(self(), "Splitter")
	{ }
};

class MotionBlurBuffers
{
private:
	Array<Texture> tex;
	Framebuffer fbo;
	Renderbuffer rbo;

	GLuint width, height;
public:
	MotionBlurBuffers(void)
	 : tex(2)
	{
		for(GLuint t=0; t!=2; ++t)
		{
			Texture::Active(t);

			Texture::Target tex_tgt = Texture::Target::Rectangle;
			tex[t].Bind(tex_tgt);

			Texture::MinFilter(tex_tgt, TextureMinFilter::Nearest);
			Texture::MagFilter(tex_tgt, TextureMagFilter::Nearest);
			Texture::WrapS(tex_tgt, TextureWrap::ClampToEdge);
			Texture::WrapT(tex_tgt, TextureWrap::ClampToEdge);
		}
		Framebuffer::Target fbo_tgt = Framebuffer::Target::Draw;
		fbo.Bind(fbo_tgt);
		Framebuffer::AttachTexture(
			fbo_tgt,
			FramebufferAttachment::Color,
			tex[0],
			0
		);

		Renderbuffer::Target rbo_tgt = Renderbuffer::Target::Renderbuffer;
		rbo.Bind(rbo_tgt);
		Framebuffer::AttachRenderbuffer(
			fbo_tgt,
			FramebufferAttachment::Depth,
			rbo
		);
	}

	void Resize(GLuint new_width, GLuint new_height)
	{
		width = new_width;
		height = new_height;
		for(GLuint t=0; t!=2; ++t)
		{
			Texture::Active(t);

			Texture::Target tex_tgt = Texture::Target::Rectangle;
			tex[t].Bind(tex_tgt);
			Texture::Image2D(
				tex_tgt,
				0,
				PixelDataInternalFormat::RGB,
				width, height,
				0,
				PixelDataFormat::RGB,
				PixelDataType::UnsignedByte,
				nullptr
			);
		}

		Renderbuffer::Target rbo_tgt = Renderbuffer::Target::Renderbuffer;
		rbo.Bind(rbo_tgt);

		Renderbuffer::Storage(
			rbo_tgt,
			PixelDataInternalFormat::DepthComponent,
			width,
			height
		);
	}

	void BindFBO(void)
	{
		fbo.Bind(Framebuffer::Target::Draw);
		assert(Framebuffer::IsComplete(Framebuffer::Target::Draw));
	}

	void Accumulate(void)
	{
		Texture::Active(1);
		tex[1].Bind(Texture::Target::Rectangle);
		Texture::CopyImage2D(
			Texture::Target::Rectangle,
			0,
			PixelDataInternalFormat::RGB,
			0, 0,
			width, height,
			0
		);
	}

};

class CheckerTex : public Texture
{
public:
	CheckerTex(TextureUnitSelector tex_unit)
	{
		Active(tex_unit);

		Target tex_tgt = Target::_2D;
		Bind(tex_tgt);

		Image2D(tex_tgt, images::CheckerRedBlack(256, 256, 8, 8));
		GenerateMipmap(tex_tgt);
		MinFilter(tex_tgt, TextureMinFilter::LinearMipmapLinear);
		MagFilter(tex_tgt, TextureMagFilter::Linear);
		WrapS(tex_tgt, TextureWrap::Repeat);
		WrapT(tex_tgt, TextureWrap::Repeat);

	}
};

class MotionBlurExample : public Example
{
private:
	Context gl;

	DrawProgram draw_prog;
	Instances instances;
	shapes::ShapeWrapper cube;

	shapes::ShapeWrapper arrow;

	BlurProgram blur_prog;
	shapes::ShapeWrapper screen;

	CheckerTex checker_tex;

	MotionBlurBuffers blur_buffers;

	GLuint screen_width;
public:
	MotionBlurExample(void)
	 : gl()
	 , instances()
	 , cube(List("Position")("Normal")("TexCoord").Get(), shapes::Cube(), draw_prog)
	 , arrow(List("Position")("Normal").Get(), ArrowShape(), draw_prog)
	 , screen(List("Position")("TexCoord").Get(), shapes::Screen(), blur_prog)
	 , checker_tex(2)
	{
		draw_prog.Use();
		draw_prog.checker_tex.Set(2);
		draw_prog.model_block.Binding(0);
		instances.matrix_buffer.BindBaseUniform(0);

		blur_prog.Use();
		blur_prog.current_frame.Set(0);
		blur_prog.previous_frames.Set(1);

		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		screen_width = width;
		gl.Viewport(width, height);

		blur_buffers.Resize(width, height);

		auto projection =
			CamMatrixf::PerspectiveX(
				Degrees(60),
				float(width)/height,
				1, 300
			);

		draw_prog.Use();
		draw_prog.projection_matrix.Set(projection);

		blur_prog.Use();
		blur_prog.screen_size.Set(width, height);
	}

	void Render(ExampleClock& clock)
	{
		int samples = 8;
		double frame_time = clock.Now().Seconds();
		double interval = clock.Interval().Seconds();
		double step = interval/samples;

		for(int s=0; s!=samples; ++s)
		{
			double time = frame_time + s*step;

			// draw objects off-screen
			blur_buffers.BindFBO();

			gl.Enable(Capability::DepthTest);

			gl.Clear().ColorBuffer().DepthBuffer();

			double pos = time/20.0;

			draw_prog.Use();
			draw_prog.camera_matrix.Set(
				CamMatrixf::LookingAt(
					instances.Position(pos-0.03+SineWave(time/7.0)*0.01),
					instances.Position(pos+0.02+SineWave(time/11.0)*0.01),
					instances.Normal(pos-0.02+SineWave(time/9.0)*0.02)
				)
			);
			draw_prog.single_model.Set(0);

			cube.Use();
			cube.Draw(instances.Count());

			draw_prog.single_model.Set(1);

			arrow.Use();

			draw_prog.model_matrix.Set(
				instances.MakeMatrix(
					pos-0.007+SineWave(time/13.0)*0.007,
					0.001
				)
			);
			arrow.Draw();

			draw_prog.model_matrix.Set(
				instances.MakeMatrix(
					pos+0.013+SineWave(time / 7.0)*0.007,
					0.001
				)
			);
			arrow.Draw();

			// motion blur
			DefaultFramebuffer().Bind(Framebuffer::Target::Draw);

			gl.Disable(Capability::DepthTest);

			blur_prog.Use();

			screen.Use();
			screen.Draw();

			blur_buffers.Accumulate();
		}
		blur_prog.splitter.Set(
			GLfloat((SineWave(frame_time / 20.0)*0.5+0.5)*screen_width)
		);
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
	return std::unique_ptr<Example>(new MotionBlurExample);
}

} // namespace oglplus
