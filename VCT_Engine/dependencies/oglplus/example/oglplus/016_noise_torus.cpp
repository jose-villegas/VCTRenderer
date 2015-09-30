/**
 *  @example oglplus/016_noise_torus.cpp
 *  @brief Shows how to draw a torus with a grayscale noise texture
 *
 *  @oglplus_screenshot{016_noise_torus}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/torus.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/buffer.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class TorusExample : public Example
{
private:
	// helper object building torus vertex attributes
	shapes::Torus make_torus;
	// helper object encapsulating torus drawing instructions
	shapes::DrawingInstructions torus_instr;
	// indices pointing to torus primitive elements
	shapes::Torus::IndexArray torus_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// Uniforms
	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered torus
	VertexArray torus;

	// VBOs for the torus's vertex attributes
	Buffer verts, normals, texcoords;

	// texture for the torus
	DefaultTexture tex;
public:
	TorusExample(void)
	 : make_torus(1.0, 0.5, 72, 48)
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	{
		// Set the vertex shader source and compile it
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
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertLight = LightPos - gl_Position.xyz;"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();

		// set the fragment shader source and compile it
		fs.Source(
			"#version 120\n"
			"uniform sampler2D TexUnit;"
			"varying vec3 vertNormal;"
			"varying vec3 vertLight;"
			"varying vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	float l = sqrt(length(vertLight));"
			"	float d = l > 0? dot("
			"		vertNormal, "
			"		normalize(vertLight)"
			"	) / l : 0.0;"
			"	float i = 0.2 + 3.2*max(d, 0.0);"
			"	gl_FragColor = texture2D(TexUnit, vertTexCoord)*i;"
			"}"
		).Compile();

		// attach the shaders to the program
		prog.AttachShader(vs).AttachShader(fs);
		// link and use it
		prog.Link();
		gl.Use(prog);

		// bind the VAO for the torus
		gl.Bind(torus);

		// bind the VBO for the torus vertices
		gl.Bind(Buffer::Target::Array, verts);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Positions(data);
			// upload the data
			gl.Current(Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the torus normals
		gl.Bind(Buffer::Target::Array, normals);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Normals(data);
			// upload the data
			gl.Current(Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the torus texture coordinates
		gl.Bind(Buffer::Target::Array, texcoords);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.TexCoordinates(data);
			// upload the data
			gl.Current(Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// setup the texture
		gl.Bind(Texture::Target::_2D, tex);
		{
			GLuint s = 256;
			std::vector<GLubyte> tex_data(s*s);
			for(GLuint v=0;v!=s;++v)
			{
				for(GLuint u=0;u!=s;++u)
				{
					tex_data[v*s+u] = GLubyte(rand() % 0x100);
				}
			}
			gl.Current(Texture::Target::_2D)
				.MinFilter(TextureMinFilter::Linear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::Repeat)
				.SwizzleG(TextureSwizzle::Red)
				.SwizzleB(TextureSwizzle::Red)
				.Image2D(
					0,
					PixelDataInternalFormat::Red,
					s, s,
					0,
					PixelDataFormat::Red,
					PixelDataType::UnsignedByte,
					tex_data.data()
				);
		}
		// typechecked uniform with exact data type
		Typechecked<Uniform<SLtoCpp<SLDataType::Sampler2D>>>(prog, "TexUnit").Set(0);
		//
		Uniform<Vec3f>(prog, "LightPos").Set(4.0f, 4.0f, -8.0f);

		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_torus.FaceWinding());
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				float(width)/height,
				1, 20
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
				4.5,
				Degrees(time * 35),
				Degrees(SineWave(time / 20.0) * 60)
			)
		);
		// set the model matrix
		model_matrix.Set(
			ModelMatrixf::RotationX(FullCircles(time * 0.25))
		);

		gl.PolygonMode(PolygonMode::Line);
		gl.CullFace(Face::Front);
		torus_instr.Draw(torus_indices);
		//
		gl.PolygonMode(PolygonMode::Fill);
		gl.CullFace(Face::Back);
		torus_instr.Draw(torus_indices);
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
	return std::unique_ptr<Example>(new TorusExample);
}

} // namespace oglplus
