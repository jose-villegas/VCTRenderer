/**
 *  @example oglplus/016_textured_cube.cpp
 *  @brief Shows how to draw a simple textured cube
 *
 *  @oglplus_screenshot{016_textured_cube}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_cpp_feat{FUNCTION_TEMPLATE_DEFAULT_ARGS}
 *  @oglplus_example_uses_texture{concrete_block}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/opt/smart_enums.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/images/load.hpp>
#include <oglplus/bound/texture.hpp>

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

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertex attributes
	Buffer verts, normals, texcoords;

	// The stained glass texture
	Texture tex;
public:
	CubeExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , projection_matrix(prog)
	 , camera_matrix(prog)
	 , model_matrix(prog)
	{
		namespace sv = oglplus::smart_values;
		try
		{
			vs.Source(
				"#version 330\n"
				"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
				"in vec4 Position;"
				"in vec3 Normal;"
				"in vec2 TexCoord;"
				"out vec3 vertNormal;"
				"out vec3 vertLight;"
				"out vec2 vertTexCoord;"
				"uniform vec3 LightPos;"
				"void main(void)"
				"{"
				"	vertNormal = mat3(ModelMatrix)*Normal;"
				"	gl_Position = ModelMatrix * Position;"
				"	vertLight = LightPos - gl_Position.xyz;"
				"	vertTexCoord = TexCoord;"
				"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
				"}"
			).Compile();
		}
		catch(CompileError&)
		{
			vs.Source(
				"#version 120\n"
				"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
				"attribute vec4 Position;"
				"attribute vec3 Normal;"
				"attribute vec2 TexCoord;"
				"varying vec3 vertNormal;"
				"varying vec3 vertLight;"
				"varying vec2 vertTexCoord;"
				"uniform vec3 LightPos;"
				"void main(void)"
				"{"
				"	vertNormal = mat3(ModelMatrix)*Normal;"
				"	gl_Position = ModelMatrix * Position;"
				"	vertLight = LightPos - gl_Position.xyz;"
				"	vertTexCoord = TexCoord;"
				"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
				"}"
			).Compile();
		}

		try
		{
			fs.Source(
				"#version 330\n"
				"uniform sampler2D TexUnit;"
				"in vec3 vertNormal;"
				"in vec3 vertLight;"
				"in vec2 vertTexCoord;"
				"out vec4 fragColor;"
				"void main(void)"
				"{"
				"	float l = length(vertLight);"
				"	float d = l > 0 ? dot(vertNormal, normalize(vertLight)) / l : 0.0;"
				"	float i = 0.3 + 2.0*max(d, 0.0);"
				"	vec4 t  = texture(TexUnit, vertTexCoord);"
				"	fragColor = vec4(t.rgb*i, 1.0);"
				"}"
			).Compile();
		}
		catch(CompileError&)
		{
			fs.Source(
				"#version 120\n"
				"uniform sampler2D TexUnit;"
				"varying vec3 vertNormal;"
				"varying vec3 vertLight;"
				"varying vec2 vertTexCoord;"
				"void main(void)"
				"{"
				"	float l = length(vertLight);"
				"	float d = l > 0 ? dot(vertNormal, normalize(vertLight)) / l : 0.0;"
				"	float i = 0.3 + 2.0*max(d, 0.0);"
				"	vec4 t = texture2D(TexUnit, vertTexCoord);"
				"	gl_FragColor = vec4(t.rgb*i, 1.0);"
				"}"
			).Compile();
		}

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		projection_matrix.BindTo("ProjectionMatrix");
		camera_matrix.BindTo("CameraMatrix");
		model_matrix.BindTo("ModelMatrix");

		// bind the VAO for the cube
		cube.Bind();

		verts.Bind(sv::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			Buffer::Data(sv::Array, data);
			(prog|"Position").Setup<GLfloat>(n_per_vertex).Enable();
		}

		normals.Bind(sv::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			Buffer::Data(sv::Array, data);
			(prog|"Normal").Setup<GLfloat>(n_per_vertex).Enable();
		}

		texcoords.Bind(sv::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.TexCoordinates(data);
			Buffer::Data(sv::Array, data);
			(prog|"TexCoord").Setup<GLfloat>(n_per_vertex).Enable();
		}

		// setup the texture
		gl.Bound(sv::_2D, tex)
			.Image2D(images::LoadTexture("concrete_block"))
			.Filter(sv::Linear)
			.Anisotropy(2.0f)
			.Wrap(sv::Repeat);
		// set the uniform values
		(prog/"TexUnit") = 0;
		(prog/"LightPos") = Vec3f(1.0f, 2.0f, 3.0f);
		//
		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(sv::DepthTest);

		gl.Enable(sv::CullFace);
		gl.FrontFace(make_cube.FaceWinding());
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				GLfloat(width)/height,
				1, 30
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		// set the matrix for camera orbiting the origin
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(5.5 - SineWave(time / 6.0) * 3.0),
				FullCircles(time * 0.7),
				Degrees(SineWave(time / 30.0) * 90)
			)
		);

		// set the model matrix
		model_matrix.Set(
			ModelMatrixf::RotationX(FullCircles(time * 0.1))
		);

		gl.CullFace(Face::Back);
		cube_instr.Draw(cube_indices);
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
	return std::unique_ptr<Example>(new CubeExample);
}

} // namespace oglplus
