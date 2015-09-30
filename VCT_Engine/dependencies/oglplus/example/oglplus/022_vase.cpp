/**
 *  @example oglplus/022_vase.cpp
 *  @brief Renders an object built by revolving a spline around the y-axis.
 *
 *  @oglplus_screenshot{022_vase}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/opt/list_init.hpp>
#include <oglplus/shapes/revolve.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/images/newton.hpp>

#include <oglplus/bound/texture.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class VaseVertShader
 : public VertexShader
{
public:
	VaseVertShader(void)
	 : VertexShader(
		ObjectDesc("Vase vertex"),
		StrCRef("#version 140\n"
		"uniform vec3 LightPosition;"
		"uniform vec3 CameraPosition;"
		"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
		"in vec4 Position;"
		"in vec3 Normal;"
		"in vec3 TexCoord;"
		"out vec3 vertNormal;"
		"out vec3 vertLightDir;"
		"out vec3 vertLightRefl;"
		"out vec3 vertViewDir;"
		"out vec2 vertTexCoord;"
		"out float vertShadow;"
		"void main(void)"
		"{"
		"	gl_Position = ModelMatrix* Position;"
		"	vertNormal = mat3(ModelMatrix)*Normal;"
		"	vertLightDir = normalize(LightPosition - gl_Position.xyz);"
		"	vertLightRefl = reflect(-vertLightDir, vertNormal);"
		"	vertViewDir = normalize(CameraPosition - gl_Position.xyz);"
		"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
		"	vertTexCoord = TexCoord.xy;"
		"	vertShadow = TexCoord.z;"
		"}")
	)
	{ }
};

class VaseFragShader
 : public FragmentShader
{
public:
	VaseFragShader(void)
	 : FragmentShader(
		ObjectDesc("Vase fragment"),
		StrCRef("#version 140\n"
		"uniform sampler2D VaseTex;"
		"in vec3 vertNormal;"
		"in vec3 vertLightDir;"
		"in vec3 vertLightRefl;"
		"in vec3 vertViewDir;"
		"in vec2 vertTexCoord;"
		"in float vertShadow;"
		"out vec3 fragColor;"
		"void main(void)"
		"{"
		"	float Ambient = 0.2 + 0.3 * vertShadow;"

		"	float Diffuse = max(dot("
		"		normalize(vertNormal),"
		"		normalize(vertLightDir)"
		"	)+0.1, 0.0) * vertShadow;"

		"	float Specular = pow(clamp(dot("
		"		normalize(vertViewDir),"
		"		normalize(vertLightRefl)"
		"	), 0.0, 1.0), 32.0) * vertShadow;"

		"	const vec3 LightColor = vec3(1.0, 1.0, 1.0);"
		"	vec3 Texel = texture(VaseTex, vertTexCoord).rgb;"

		"	fragColor = "
		"		(Ambient + Diffuse)*Texel +"
		"		Specular * LightColor;"
		"}")
	)
	{ }
};

class VaseProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Vase program"));
		prog << VaseVertShader() << VaseFragShader();
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniformSampler vase_tex;

	VaseProgram(void)
	 : Program(make())
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	 , vase_tex(prog(), "VaseTex")
	{ }
};

class VaseExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// the vase shading program
	VaseProgram vase_prog;

	// texture applied to the vase
	Texture vase_tex;

	// wraper for the vase builder, VAO and VBOs
	shapes::ShapeWrapper vase;
public:
	VaseExample(void)
	 : vase(
		List("Position")("Normal")("TexCoord").Get(),
		shapes::RevolveY<GLfloat>(
			36,
			BezierCurves<Vec3f, GLfloat, 3>(
				ListOf<Vec3f>
					(Vec3f( 0.00f, 0.00f, 0.00f))

					(Vec3f( 0.20f, 0.00f, 0.00f))
					(Vec3f( 0.40f, 0.00f, 0.00f))
					(Vec3f( 0.80f, 0.00f, 0.00f))

					(Vec3f( 1.45f, 0.00f, 0.00f))
					(Vec3f( 1.45f, 0.40f, 0.00f))
					(Vec3f( 1.40f, 0.80f, 0.00f))

					(Vec3f( 1.35f, 1.20f, 0.00f))
					(Vec3f( 1.20f, 1.30f, 0.00f))
					(Vec3f( 1.00f, 1.50f, 0.00f))

					(Vec3f( 0.80f, 1.70f, 0.00f))
					(Vec3f( 0.80f, 2.20f, 0.00f))
					(Vec3f( 0.90f, 2.50f, 0.00f))

					(Vec3f( 1.00f, 2.80f, 0.00f))
					(Vec3f( 0.98f, 2.88f, 0.00f))
					(Vec3f( 1.05f, 2.90f, 0.00f))

					(Vec3f( 1.12f, 2.92f, 0.00f))
					(Vec3f( 1.09f, 2.96f, 0.00f))
					(Vec3f( 1.05f, 2.99f, 0.00f))

					(Vec3f( 1.01f, 3.02f, 0.00f))
					(Vec3f( 0.96f, 2.99f, 0.00f))
					(Vec3f( 0.93f, 2.90f, 0.00f))

					(Vec3f( 0.90f, 2.81f, 0.00f))
					(Vec3f( 0.88f, 2.70f, 0.00f))
					(Vec3f( 0.82f, 2.50f, 0.00f))

					(Vec3f( 0.74f, 2.30f, 0.00f))
					(Vec3f( 0.70f, 1.70f, 0.00f))
					(Vec3f( 0.88f, 1.50f, 0.00f))

					(Vec3f( 1.06f, 1.30f, 0.00f))
					(Vec3f( 1.25f, 1.20f, 0.00f))
					(Vec3f( 1.30f, 0.80f, 0.00f))

					(Vec3f( 1.35f, 0.40f, 0.00f))
					(Vec3f( 1.35f, 0.10f, 0.00f))
					(Vec3f( 0.80f, 0.10f, 0.00f))

					(Vec3f( 0.40f, 0.10f, 0.00f))
					(Vec3f( 0.25f, 0.10f, 0.00f))
					(Vec3f( 0.00f, 0.10f, 0.00f))
				.Get()
			).Approximate(8),
			std::vector<Vec3f>(),
			BezierCurves<Vec3f, GLfloat, 3>(
				ListOf<Vec3f>
					(Vec3f( 5.00f,-2.00f, 1.00f))

					(Vec3f( 5.00f,-1.67f, 1.00f))
					(Vec3f( 5.00f,-1.33f, 1.00f))
					(Vec3f( 5.00f,-1.00f, 1.00f))

					(Vec3f( 5.00f,-0.67f, 1.00f))
					(Vec3f( 5.00f,-0.33f, 1.00f))
					(Vec3f( 5.00f, 0.00f, 1.00f))

					(Vec3f( 5.00f, 0.20f, 1.00f))
					(Vec3f( 5.00f, 0.40f, 1.00f))
					(Vec3f( 5.00f, 0.60f, 1.00f))

					(Vec3f( 5.00f, 0.80f, 1.00f))
					(Vec3f( 5.00f, 1.00f, 1.00f))
					(Vec3f( 5.00f, 1.15f, 1.00f))

					(Vec3f( 5.00f, 1.30f, 1.00f))
					(Vec3f( 5.00f, 1.45f, 0.40f))
					(Vec3f( 5.00f, 1.60f, 0.60f))

					(Vec3f( 5.00f, 1.75f, 1.00f))
					(Vec3f( 5.00f, 1.90f, 1.00f))
					(Vec3f( 5.00f, 2.10f, 1.00f))

					(Vec3f( 5.00f, 2.20f, 1.00f))
					(Vec3f( 5.00f, 2.35f, 1.00f))
					(Vec3f( 5.00f, 2.50f, 1.00f))

					(Vec3f( 5.00f, 2.65f, 1.00f))
					(Vec3f( 5.00f, 2.80f, 1.00f))
					(Vec3f( 5.00f, 2.95f, 1.00f))

					(Vec3f( 5.00f, 3.10f, 0.90f))
					(Vec3f( 5.00f, 3.25f, 0.50f))
					(Vec3f( 5.00f, 3.40f, 0.10f))

					(Vec3f( 5.00f, 3.55f, 0.00f))
					(Vec3f( 5.00f, 3.70f, 0.00f))
					(Vec3f( 5.00f, 3.95f, 0.00f))

					(Vec3f( 5.00f, 4.10f, 0.00f))
					(Vec3f( 5.00f, 4.30f, 0.00f))
					(Vec3f( 5.00f, 4.50f, 0.10f))

					(Vec3f( 5.00f, 4.70f, 0.15f))
					(Vec3f( 5.00f, 4.90f, 0.20f))
					(Vec3f( 5.00f, 5.00f, 0.30f))
				.Get()
			).Approximate(8)
		),
		vase_prog
	)
	{
		// setup the texture
		{
			GLuint tex_side = 512;
			auto image = images::NewtonFractal(
				tex_side, tex_side,
				Vec3f(0.8f, 0.8f, 1.0f),
				Vec3f(0.1f, 0.0f, 0.2f),
				Vec2f(-1.0f, -1.0f),
				Vec2f( 1.0f,  1.0f),
				images::NewtonFractal::X4Minus1(),
				[](double x) -> double
				{
					return pow(SineWave(pow(x,0.5)), 4.0);
				}
			);
			gl.Bound(Texture::Target::_2D, vase_tex)
				.Image2D(image)
				.GenerateMipmap()
				.BorderColor(Vec4f(0.8f, 0.8f, 1.0f, 1.0f))
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::ClampToBorder);
		}

		vase_prog.vase_tex = 0;
		vase_prog.light_position.Set(4.0f, 4.0f, -8.0f);

		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		vase_prog.projection_matrix = CamMatrixf::PerspectiveX(
			Degrees(60),
			width, height,
			1, 30
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto camera = CamMatrixf::Orbiting(
			Vec3f(0.0, 1.5, 0.0),
			GLfloat(7.0 + SineWave(time / 11.0)*2.5),
			FullCircles(time / 19.0),
			Degrees(SineWave(time / 20.0) * 30 + 35)
		);
		vase_prog.camera_matrix = camera;
		vase_prog.camera_position = camera.Position();

		vase_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::RotationX(Degrees(25))*
			ModelMatrixf::Translation(0.0, 0.0,-1.0);

		vase.Draw();
	}

	bool Continue(double time)
	{
		return time < 30.0;
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
	return std::unique_ptr<Example>(new VaseExample);
}

} // namespace oglplus
