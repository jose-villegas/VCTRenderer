/**
 *  @example oglplus/031_fog.cpp
 *  @brief Shows depth buffer fog
 *
 *  @oglplus_screenshot{031_fog}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/twisted_torus.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/image_spec.hpp>
#include <oglplus/images/checker.hpp>
#include <oglplus/images/random.hpp>

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
		pos_data.push_back(Vec3f( 60.0f,  0.0f,-45.0f));
		pos_data.push_back(Vec3f( 10.0f,  0.0f,-60.0f));
		pos_data.push_back(Vec3f(-25.0f, 20.0f,-60.0f));
		pos_data.push_back(Vec3f(-60.0f, 40.0f,-60.0f));
		pos_data.push_back(Vec3f(-80.0f, 40.0f,-30.0f));
		pos_data.push_back(Vec3f(-25.0f, 40.0f, 10.0f));
		pos_data.push_back(Vec3f( 35.0f, 40.0f, 10.0f));
		pos_data.push_back(Vec3f( 80.0f,  0.0f,  0.0f));
		pos_data.push_back(Vec3f( 45.0f,-40.0f,-10.0f));
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

	static GLfloat nrand(void)
	{
		return std::rand()/GLfloat(RAND_MAX);
	}
public:
	Buffer matrix_buffer;

	Instances(GLuint n = 64)
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
				const GLfloat s[2] = {-1.0f, 1.0f};
				Vec3f offs =
					pos+
					btg*s[j]*(3+4*nrand())+
					tgt*2*nrand()+
					nml*6*nrand();
				Mat4f matrix = Transposed(
					ModelMatrixf::Translation(offs)*
					ModelMatrixf::RotationX(FullCircles(nrand()))*
					ModelMatrixf::RotationY(FullCircles(nrand()))*
					ModelMatrixf::RotationZ(FullCircles(nrand()))
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

		matrix_buffer
			<< BufferTarget::Uniform
			<< BufferUsage::StaticDraw
			<< matrix_data;
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

class SkyProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Sky"));

		VertexShader vs(ObjectDesc("Sky"));
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"

			"in vec4 Position;"
			"in vec2 TexCoord;"

			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = ProjectionMatrix * CameraMatrix * Position;"
			"	vertTexCoord = TexCoord;"
			"}"
		).Compile();

		FragmentShader fs(ObjectDesc("Sky"));
		fs.Source(
			"#version 140\n"

			"uniform sampler2D Noise;"

			"in vec2 vertTexCoord;"

			"out float fragValue;"

			"void main(void)"
			"{"
			"	float n = 1.0;"
			"	for(int l=0; l!=7; ++l)"
			"	{"
			"		float t = textureLod(Noise, vertTexCoord, l).r;"
			"		n *= (0.05*(18-l) + t*0.05*(2+l));"
			"	}"
			"	n = pow(min(2.1*n, 1.0), 4.0);"
			"	fragValue = n;"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix, camera_matrix;
	UniformSampler noise;

	SkyProgram(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , noise(self(), "Noise")
	{ }
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
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"layout (std140) uniform ModelBlock {"
			"	mat4 ModelMatrices[128];"
			"};"

			"const vec3 LightPos = vec3(0.0, 0.0, 0.0);"

			"in vec4 Position;"
			"in vec3 Normal;"

			"out vec3 vertLightDir;"
			"out vec3 vertNormal;"
			"out vec3 vertColor;"

			"void main(void)"
			"{"
			"	mat4 ModelMatrix = ModelMatrices[gl_InstanceID];"

			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = normalize(LightPos - gl_Position.xyz);"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertColor = abs(Normal-normalize((ModelMatrix*Position).yxz));"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();

		FragmentShader fs(ObjectDesc("Draw"));
		fs.Source(
			"#version 140\n"

			"in vec3 vertLightDir;"
			"in vec3 vertNormal;"
			"in vec3 vertColor;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float l = 0.5 + sqrt(max(dot(vertLightDir,vertNormal)+0.1,0.0));"
			"	fragColor = vertColor * min(l, 0.8);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix, camera_matrix;
	UniformBlock model_block;

	DrawProgram(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_block(self(), "ModelBlock")
	{ }
};

class FogProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Fog"));

		VertexShader vs(ObjectDesc("Fog"));
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

		FragmentShader fs(ObjectDesc("Fog"));
		fs.Source(
			"#version 140\n"

			"uniform sampler2DRect Noise;"
			"uniform sampler2DRect Color;"
			"uniform sampler2DRect Depth;"
			"const int Samples = 64;"
			"uniform vec2 SampleOffs[64];"
			"const float InvSamples = 1.0 / Samples;"

			"in vec2 vertTexCoord;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float n = texture(Noise, vertTexCoord).r;"
			"	n -= 0.5;"
			"	float z = pow(texture(Depth, vertTexCoord).r, 4);"
			"	z = pow(clamp((z-0.35-0.1*n)*(2.5+0.5*n), 0.0, 1.0), 8);"
			"	int nsam = int(1+z*(Samples-1));"
			"	float wsum = 0.0;"

			"	for(int i=0; i!=nsam; ++i)"
			"	{"
			"		vec2 so = SampleOffs[i]*8.0*z;"
			"		vec2 samTexCoord = vertTexCoord+vec2(so.x, so.y);"

			"		float sz = pow(texture(Depth, samTexCoord).r, 2);"

			"		float n = texture(Noise, samTexCoord).r;"
			"		float d = mix(0.9, 1.0, n);"
			"		vec3 t = texture(Color, samTexCoord).rgb;"
			"		vec3 fd = vec3(d, d, d);"
			"		float s = (0.3*t.r + 0.59*t.g + 0.11*t.b);"
			"		vec3 fs = vec3(s, s, s);"
			"		float w = 1.0-abs(z-sz);"
			"		fragColor += w*mix(t, mix(fd, fs, sz*(1.0-sz)), z*sz);"
			"		wsum += w;"
			"	}"
			"	fragColor /= wsum;"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		Uniform<Vec2f> shadow_offs(prog, "SampleOffs");
		for(GLuint i=0; i!=64; ++i)
		{
			float u = std::rand() / float(RAND_MAX);
			float v = std::rand() / float(RAND_MAX);
			shadow_offs[i].Set(
				GLfloat(std::sqrt(v) * std::cos(2*3.1415*u)),
				GLfloat(std::sqrt(v) * std::sin(2*3.1415*u))
			);
		}

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Vec2f> screen_size;
	UniformSampler noise, color, depth;

	FogProgram(void)
	 : Program(make())
	 , screen_size(self(), "ScreenSize")
	 , noise(self(), "Noise")
	 , color(self(), "Color")
	 , depth(self(), "Depth")
	{ }
};

class NoiseTex : public Texture
{
public:
	NoiseTex(void)
	{
		Texture::Target tex_tgt = Texture::Target::_2D;

		Texture::Active(0);
		this->Bind(tex_tgt);

		GLuint tex_side = 64;
		for(GLuint level=0; level!=7; ++level)
		{
			(tex_tgt|level) << images::RandomRedUByte(tex_side, tex_side);
			tex_side /= 2.0;
		}
		Texture::MaxLOD(tex_tgt, 6);
		Texture::MinFilter(tex_tgt, TextureMinFilter::LinearMipmapLinear);
		Texture::MagFilter(tex_tgt, TextureMagFilter::Linear);
		Texture::WrapS(tex_tgt, TextureWrap::Repeat);
		Texture::WrapT(tex_tgt, TextureWrap::Repeat);
	}
};

class FogBuffers
{
private:
	Texture noise;
	Framebuffer sky_fbo;

	Texture color, depth;
	Framebuffer fog_fbo;

	GLuint width, height;
public:
	FogBuffers(void)
	{
		Texture::Active(1);
		noise	<< Texture::Target::Rectangle
			<< TextureMinFilter::Linear
			<< TextureMagFilter::Linear
			<< TextureWrap::ClampToEdge;

		Texture::Active(2);
		color	<< Texture::Target::Rectangle
			<< TextureMinFilter::Linear
			<< TextureMagFilter::Linear
			<< TextureWrap::ClampToEdge;

		Texture::Active(3);
		depth	<< Texture::Target::Rectangle
			<< TextureMinFilter::Linear
			<< TextureMagFilter::Linear
			<< TextureWrap::ClampToEdge;


		sky_fbo	<< Framebuffer::Target::Draw
			<< FramebufferAttachment::Color << noise;

		fog_fbo	<< Framebuffer::Target::Draw
			<< FramebufferAttachment::Color << color
			<< FramebufferAttachment::Depth << depth;
	}

	void Resize(GLuint new_width, GLuint new_height)
	{
		width = new_width;
		height = new_height;

		Texture::Active(1);
		noise	<< Texture::Target::Rectangle
			<< images::ImageSpec(
				width, height,
				PixelDataFormat::Red,
				PixelDataType::UnsignedByte
			);

		Texture::Active(2);
		color	<< Texture::Target::Rectangle
			<< images::ImageSpec(
				width, height,
				PixelDataFormat::RGB,
				PixelDataType::UnsignedByte
			);

		Texture::Active(3);
		depth	<< Texture::Target::Rectangle
			<< images::ImageSpec(
				width, height,
				PixelDataFormat::DepthComponent,
				PixelDataType::Float
			);
	}

	void BindSkyFBO(void)
	{
		sky_fbo.Bind(Framebuffer::Target::Draw);
		assert(Framebuffer::IsComplete(Framebuffer::Target::Draw));
	}

	void BindFogFBO(void)
	{
		fog_fbo.Bind(Framebuffer::Target::Draw);
		assert(Framebuffer::IsComplete(Framebuffer::Target::Draw));
	}
};

class FogExample : public Example
{
private:
	Context gl;
	Instances instances;

	SkyProgram sky_prog;
	shapes::ShapeWrapper sky_box;
	NoiseTex noise;

	DrawProgram draw_prog;
	shapes::ShapeWrapper torus;

	FogProgram fog_prog;
	shapes::ShapeWrapper screen;

	FogBuffers fog_buffers;

	GLuint screen_width;

public:
	FogExample(void)
	 : gl()
	 , instances()
	 , sky_box(
		List("Position")("TexCoord").Get(),
		shapes::Cube(1000, 1000, 1000),
		sky_prog
	), torus(
		List("Position")("Normal").Get(),
		shapes::TwistedTorus(1.0, 0.2, 0.01, 6, 36, 4),
		draw_prog
	), screen(
		List("Position")("TexCoord").Get(),
		shapes::Screen(),
		fog_prog
	)
	{
		sky_prog.Use();
		sky_prog.noise.Set(0);

		draw_prog.Use();
		draw_prog.model_block.Binding(0);
		instances.matrix_buffer.BindBaseUniform(0);

		fog_prog.Use();
		fog_prog.noise.Set(1);
		fog_prog.color.Set(2);
		fog_prog.depth.Set(3);

		gl.ClearColor(0.95f, 0.95f, 0.95f, 0.0f);
		gl.ClearDepth(1.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		screen_width = width;
		gl.Viewport(width, height);

		fog_buffers.Resize(width, height);

		sky_prog.Use();
		sky_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(75),
				width, height,
				1, 2000
			)
		);

		draw_prog.Use();
		draw_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(75),
				width, height,
				1, 200
			)
		);

		fog_prog.Use();
		fog_prog.screen_size.Set(width, height);
	}

	void Render(double time)
	{
		double pos = time/90.0;

		auto camera = CamMatrixf::LookingAt(
			instances.Position(pos-0.03+SineWave(time/7.0)*0.01),
			instances.Position(pos+0.02+SineWave(time/11.0)*0.01),
			instances.Normal(pos-0.02+SineWave(time/9.0)*0.02)
		);

		// render the noise sky box
		fog_buffers.BindSkyFBO();

		gl.DepthMask(false);
		gl.Disable(Capability::DepthTest);
		gl.Disable(Capability::CullFace);

		sky_prog.Use();
		sky_prog.camera_matrix.Set(camera);

		sky_box.Use();
		sky_box.Draw();
		gl.DepthMask(true);

		// draw objects off-screen
		fog_buffers.BindFogFBO();

		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);

		gl.Clear().ColorBuffer().DepthBuffer();

		draw_prog.Use();
		draw_prog.camera_matrix.Set(camera);

		torus.Use();
		torus.Draw(instances.Count());


		// draw fog
		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);

		gl.Disable(Capability::DepthTest);

		fog_prog.Use();

		gl.DepthMask(false);
		screen.Use();
		screen.Draw();
		gl.DepthMask(true);
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
	return std::unique_ptr<Example>(new FogExample);
}

} // namespace oglplus
