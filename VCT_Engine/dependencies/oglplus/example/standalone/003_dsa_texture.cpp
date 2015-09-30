/**
 *  @example standalone/003_dsa_texture.cpp
 *  @brief Shows usage of the direct state access texture wrapper
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "glut_glew_example.hpp"

#include <oglplus/all.hpp>
#include <oglplus/images/checker.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/opt/list_init.hpp>
#include <oglplus/dsa/ext/texture.hpp>

#include <vector>

class DSATextureExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::Context gl;

	oglplus::Program prog;

	static oglplus::Program make_prog(void)
	{
		using namespace oglplus;

		Program prog;

		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"uniform vec3 LightPos;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"
			"out vec3 vertNormal;"
			"out vec3 vertLight;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLight = LightPos - gl_Position.xyz;"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"uniform sampler2D Checker;"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = dot(vertNormal, normalize(vertLight));"
			"	float i = 0.4 + 1.4*max(d, 0.0);"
			"	vec4 t  = texture(Checker, vertTexCoord);"
			"	fragColor = vec4(t.rrr*i, 1.0);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return prog;
	}

	oglplus::Uniform<oglplus::Mat4f>
		projection_matrix,
		camera_matrix,
		model_matrix;

	oglplus::shapes::ShapeWrapper textured_cube;

	oglplus::DSATextureEXT checker;
public:
	DSATextureExample(int /*argc*/, const char** /*argv*/)
	 : gl()
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , textured_cube(
		oglplus::List("Position")("Normal")("TexCoord").Get(),
		oglplus::shapes::Cube(),
		prog
	)
	{
		using namespace oglplus;

		checker.target = Texture::Target::_2D;
		checker.Image2D(images::CheckerRedBlack(256, 256, 8, 8));
		checker.GenerateMipmap();
		checker.MinFilter(TextureMinFilter::LinearMipmapLinear);
		checker.MagFilter(TextureMagFilter::Linear);
		checker.Anisotropy(2.0);
		checker.WrapS(TextureWrap::Repeat);
		checker.WrapT(TextureWrap::Repeat);
		checker.Bind();
		(prog/"Checker") = 0;
		(prog/"LightPos") = Vec3f(10.0f, 20.0f, 30.0f);

		gl.ClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
	}

	void Render(void)
	{
		using namespace oglplus;

		gl.Clear().ColorBuffer().DepthBuffer();

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				9.5 - SineWave(FrameTime() / 6.0) * 7.0,
				FullCircles(FrameTime() / 9.0),
				Degrees(SineWave(FrameTime() / 30.0) * 90)
			)
		);

		model_matrix.Set(ModelMatrixf::RotationX(FullCircles(FrameTime() * 0.1)));

		textured_cube.Draw();
	}

	void Reshape(void)
	{
		using namespace oglplus;

		gl.Viewport(Width(), Height());

		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				Aspect(),
				1, 50
			)
		);
	}
};

int main(int argc, char* argv[])
{
	return oglplus::GlutGlewMain<DSATextureExample>(
		"Example of EXT_direct_state_access texture usage",
		argc, argv
	);
}

