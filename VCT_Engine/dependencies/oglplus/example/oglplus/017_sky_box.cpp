/**
 *  @example oglplus/017_sky_box.cpp
 *  @brief Shows how to render a textured sky box
 *
 *  @oglplus_screenshot{017_sky_box}
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_texture{cloudy_day-cm_0}
 *  @oglplus_example_uses_texture{cloudy_day-cm_1}
 *  @oglplus_example_uses_texture{cloudy_day-cm_2}
 *  @oglplus_example_uses_texture{cloudy_day-cm_3}
 *  @oglplus_example_uses_texture{cloudy_day-cm_4}
 *  @oglplus_example_uses_texture{cloudy_day-cm_5}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/images/load.hpp>

#include "example.hpp"

namespace oglplus {

class SkyBoxExample : public Example
{
private:
	Context gl;

	Program prog;

	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix;

	VertexArray sky_box;

	Buffer corners, indices;

	Texture env_map;
public:
	SkyBoxExample(void)
	 : prog()
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	{
		VertexShader vs;
		vs.Source(StrCRef(
			"#version 120\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"mat4 Matrix = ProjectionMatrix*CameraMatrix;"

			"attribute vec3 Corner;"
			"varying vec3 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = Matrix * vec4(Corner * 10.0, 1.0);"
			"	vertTexCoord = Corner;"
			"}"
		));
		vs.Compile();
		prog.AttachShader(vs);

		FragmentShader fs;
		fs.Source(StrCRef(
			"#version 120\n"
			"uniform samplerCube EnvMap;"
			"varying vec3 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_FragColor = vec4(textureCube(EnvMap, normalize(vertTexCoord)).rgb, 1.0);"
			"}"
		));
		fs.Compile();
		prog.AttachShader(fs);

		prog.Link();
		prog.Use();

		sky_box.Bind();

		GLfloat sky_box_corners[8*3] = {
			-1.0f,-1.0f,-1.0f,
			+1.0f,-1.0f,-1.0f,
			-1.0f,+1.0f,-1.0f,
			+1.0f,+1.0f,-1.0f,
			-1.0f,-1.0f,+1.0f,
			+1.0f,-1.0f,+1.0f,
			-1.0f,+1.0f,+1.0f,
			+1.0f,+1.0f,+1.0f
		};
		corners.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, sky_box_corners);
		VertexArrayAttrib vert_attr(prog, "Corner");
		vert_attr.Setup<Vec3f>().Enable();

		GLuint sky_box_indices[6*5] = {
			1, 3, 5, 7, 9,
			4, 6, 0, 2, 9,
			2, 6, 3, 7, 9,
			4, 0, 5, 1, 9,
			5, 7, 4, 6, 9,
			0, 2, 1, 3, 9
		};
		indices.Bind(Buffer::Target::ElementArray);
		Buffer::Data(Buffer::Target::ElementArray, 6*5, sky_box_indices);

		gl.Enable(Capability::PrimitiveRestart);
		gl.PrimitiveRestartIndex(9);

		{
			UniformSampler(prog, "EnvMap").Set(0);
			Texture::Active(0);

			gl.Bound(Texture::Target::CubeMap, env_map)
				.MinFilter(TextureMinFilter::Linear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::ClampToEdge)
				.WrapT(TextureWrap::ClampToEdge)
				.WrapR(TextureWrap::ClampToEdge);

			Texture::Image2D(
				Texture::CubeMapFace(0),
				images::LoadTexture("cloudy_day-cm_0", false, false)
			);
			Texture::Image2D(
				Texture::CubeMapFace(1),
				images::LoadTexture("cloudy_day-cm_1", false, false)
			);
			Texture::Image2D(
				Texture::CubeMapFace(2),
				images::LoadTexture("cloudy_day-cm_2", false, false)
			);
			Texture::Image2D(
				Texture::CubeMapFace(3),
				images::LoadTexture("cloudy_day-cm_3", false, false)
			);
			Texture::Image2D(
				Texture::CubeMapFace(4),
				images::LoadTexture("cloudy_day-cm_4", false, false)
			);
			Texture::Image2D(
				Texture::CubeMapFace(5),
				images::LoadTexture("cloudy_day-cm_5", false, false)
			);
		}

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.ClearDepth(1.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(48),
				width, height,
				1, 100
			)
		);
	}

	void Render(double time)
	{
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			3.0,
			FullCircles(time / 13.0),
			Degrees(-35 - SineWave(time / 19.0) * 30)
		);
		camera_matrix.Set(camera);

		gl.Clear().ColorBuffer().DepthBuffer();
		gl.DrawElements(PrimitiveType::TriangleStrip, 6*5, DataType::UnsignedInt);
	}

	bool Continue(double time)
	{
		return time < 45.0;
	}

	double ScreenshotTime(void) const
	{
		return 1.0;
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
	return std::unique_ptr<Example>(new SkyBoxExample);
}

} // namespace oglplus
