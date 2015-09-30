/**
 *  @example oglplus/021_wooden_crate.cpp
 *  @brief Shows how to draw a simple textured and bump mapped cube
 *
 *  @oglplus_screenshot{021_wooden_crate}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_texture{wooden_crate}
 *  @oglplus_example_uses_texture{wooden_crate-hmap}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/images/load.hpp>
#include <oglplus/images/normal_map.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/buffer.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class CubeExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions cube_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray cube_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;
	static Program make_prog(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec3 Tangent;"
			"in vec2 TexCoord;"
			"out vec3 vertLight;"
			"out vec2 vertTexCoord;"
			"out mat3 NormalMatrix;"
			"uniform vec3 LightPos;"
			"void main(void)"
			"{"
			"	vec3 fragNormal = mat3(ModelMatrix) * Normal;"
			"	vec3 fragTangent = mat3(ModelMatrix) * Tangent;"
			"	NormalMatrix[0] = fragTangent;"
			"	NormalMatrix[1] = cross(fragNormal, fragTangent);"
			"	NormalMatrix[2] = fragNormal;"

			"	gl_Position = ModelMatrix * Position;"
			"	vertLight = LightPos - gl_Position.xyz;"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		vs.Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"uniform sampler2D ColorTex, NormalTex;"
			"in mat3 NormalMatrix;"
			"in vec3 vertLight;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float l = dot(vertLight, vertLight);"
			"	vec3 n = texture(NormalTex, vertTexCoord).xyz;"
			"	vec3 finalNormal = NormalMatrix * n;"
			"	float d = (l > 0.0) ? dot("
			"		normalize(vertLight), "
			"		finalNormal"
			"	) / l : 0.0;"
			"	float i = 0.2 + 4.5*max(d, 0.0);"
			"	vec4 t  = texture(ColorTex, vertTexCoord);"
			"	fragColor = vec4(t.rgb*i, 1.0);"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		prog.Link();
		prog.Use();

		return prog;
	}

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	Uniform<Vec3f> light_pos;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertex attributes
	Buffer verts, normals, tangents, texcoords;

	// The color and normalmap texture
	Texture colorTex, normalTex;
public:
	CubeExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , light_pos(prog, "LightPos")
	{
		gl.Bind(cube);

		gl.Bind(Buffer::Target::Array, verts);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			gl.Current(Buffer::Target::Array).Data(data);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		gl.Bind(Buffer::Target::Array, normals);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			gl.Current(Buffer::Target::Array).Data(data);
			VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		gl.Bind(Buffer::Target::Array, tangents);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Tangents(data);
			gl.Current(Buffer::Target::Array).Data(data);
			VertexArrayAttrib attr(prog, "Tangent");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		gl.Bind(Buffer::Target::Array, texcoords);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.TexCoordinates(data);
			gl.Current(Buffer::Target::Array).Data(data);
			VertexArrayAttrib attr(prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// setup the textures
		{
			Texture::Active(0);
			UniformSampler(prog, "ColorTex").Set(0);

			gl.Bind(Texture::Target::_2D, colorTex);
			gl.Current(Texture::Target::_2D)
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::Repeat)
				.Image2D(images::LoadTexture("wooden_crate"))
				.GenerateMipmap();
		}
		{
			Texture::Active(1);
			UniformSampler(prog, "NormalTex").Set(1);

			gl.Bind(Texture::Target::_2D, normalTex);
			gl.Current(Texture::Target::_2D)
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::Repeat)
				.Image2D(
					images::NormalMap(
						images::LoadTexture("wooden_crate-hmap")
					)
				).GenerateMipmap();
		}
		//
		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_cube.FaceWinding());
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				float(width)/height,
				1, 15
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto lightAzimuth = FullCircles(time * -0.5);
		light_pos.Set(
			Vec3f(
				Cos(lightAzimuth),
				1.0f,
				Sin(lightAzimuth)
			) * 2.0f
		);
		//
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.0f,
				Degrees(-45),
				Degrees(SineWave(time / 30.0) * 70)
			)
		);

		// set the model matrix
		model_matrix.Set(
			ModelMatrixf::RotationY(FullCircles(time * 0.05))
		);

		cube.Bind();
		gl.CullFace(Face::Back);
		cube_instr.Draw(cube_indices);
	}

	bool Continue(double time)
	{
		return time < 30.0;
	}

	double ScreenshotTime(void) const
	{
		return 2.0;
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
	return std::unique_ptr<Example>(new CubeExample);
}

} // namespace oglplus
