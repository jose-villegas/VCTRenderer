/**
 *  @example oglplus/025_rendered_texture.cpp
 *  @brief Shows how to render into a texture
 *
 *  @oglplus_screenshot{025_rendered_texture}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_2}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/torus.hpp>
#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/ext/framebuffer.hpp>
#include <oglplus/dsa/ext/renderbuffer.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class FBTexExample : public Example
{
private:
	// The torus builder and rendering instructions
	shapes::Cube make_cube;
	shapes::DrawingInstructions cube_instr;
	shapes::Cube::IndexArray cube_indices;

	// The torus builder and rendering instructions
	shapes::Torus make_torus;
	shapes::DrawingInstructions torus_instr;
	shapes::Torus::IndexArray torus_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Cube and torus fragment shader
	FragmentShader cube_fs, torus_fs;

	// Program
	Program cube_prog, torus_prog;

	// Handle for matrix uniforms
	Uniform<Mat4f>
		torus_projection_matrix, torus_camera_matrix, torus_model_matrix,
		cube_projection_matrix, cube_camera_matrix, cube_model_matrix;

	// A vertex array objects for the rendered shapes
	VertexArray cube, torus;

	// VBOs for the shape vertex attributes
	Buffer cube_verts, cube_normals, cube_texcoords;
	Buffer torus_verts, torus_normals, torus_texcoords;

	// The default onscreen framebuffer
	DefaultFramebuffer dfb;

	// The FBO and RBO for offscreen rendering
	DSAFramebufferEXT fbo;
	DSARenderbufferEXT rbo;

	// The dynamically rendered texture
	DSATextureEXT tex;
	GLuint tex_side;

	GLuint width, height;
public:
	FBTexExample(void)
	 : make_cube()
	 , cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , make_torus(1.0, 0.5, 72, 48)
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , cube_fs(ObjectDesc("Cube fragment"))
	 , torus_fs(ObjectDesc("Torus fragment"))
	 , torus_projection_matrix(torus_prog)
	 , torus_camera_matrix(torus_prog)
	 , torus_model_matrix(torus_prog)
	 , cube_projection_matrix(cube_prog)
	 , cube_camera_matrix(cube_prog)
	 , cube_model_matrix(cube_prog)
	 , fbo()
	 , rbo()
	 , tex()
	 , tex_side(512)
	 , width(tex_side)
	 , height(tex_side)
	{
		vs.Source(
			"#version 140\n"
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
			"	vertLight = LightPos-gl_Position.xyz;"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		vs.Compile();

		cube_fs.Source(
			"#version 140\n"
			"uniform sampler2D TexUnit;"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float l = sqrt(length(vertLight));"
			"	float d = l > 0? dot(vertNormal, normalize(vertLight)) / l : 0.0;"
			"	float i = 0.6 + max(d, 0.0);"
			"	fragColor = texture(TexUnit, vertTexCoord)*i;"
			"}"
		);
		cube_fs.Compile();

		cube_prog.AttachShader(vs);
		cube_prog.AttachShader(cube_fs);
		cube_prog.Link();
		cube_prog.Use();
		cube_projection_matrix.BindTo("ProjectionMatrix");
		cube_camera_matrix.BindTo("CameraMatrix");
		cube_model_matrix.BindTo("ModelMatrix");

		cube.Bind();

		cube_verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(cube_prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		cube_normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(cube_prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		cube_texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.TexCoordinates(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(cube_prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		Uniform<GLint>(cube_prog, "TexUnit").Set(0);
		Uniform<Vec3f>(cube_prog, "LightPos").Set(Vec3f(4.0f, 4.0f, -8.0f));

		torus_fs.Source(
			"#version 140\n"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = dot("
			"		vertNormal, "
			"		normalize(vertLight)"
			"	);"
			"	float i = ("
			"		int(vertTexCoord.x*18) % 2+"
			"		int(vertTexCoord.y*14) % 2"
			"	) % 2;"
			"	float c = (0.4 + max(d, 0.0))*(1-i/2);"
			"	fragColor = vec4(c, c, c, 1.0);"
			"}"
		);
		torus_fs.Compile();

		torus_prog.AttachShader(vs);
		torus_prog.AttachShader(torus_fs);
		torus_prog.Link();
		torus_prog.Use();
		torus_projection_matrix.BindTo("ProjectionMatrix");
		torus_camera_matrix.BindTo("CameraMatrix");
		torus_model_matrix.BindTo("ModelMatrix");

		torus.Bind();

		torus_verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(torus_prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		torus_normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(torus_prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		torus_texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.TexCoordinates(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(torus_prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		Uniform<Vec3f>(torus_prog, "LightPos").Set(Vec3f(2.0f, 3.0f, 4.0f));

		tex.target = TextureTarget::_2D;
		tex.Image2D(
			0,
			PixelDataInternalFormat::RGBA,
			tex_side, tex_side,
			0,
			PixelDataFormat::RGBA,
			PixelDataType::UnsignedByte,
			nullptr
		);
		tex.MinFilter(TextureMinFilter::Linear);
		tex.MagFilter(TextureMagFilter::Linear);
		tex.WrapS(TextureWrap::Repeat);
		tex.WrapT(TextureWrap::Repeat);
		tex.Bind(TextureTarget::_2D);

		rbo.Storage(
			PixelDataInternalFormat::DepthComponent,
			tex_side,
			tex_side
		);

		fbo.AttachTexture(
			FramebufferAttachment::Color,
			tex,
			0
		);
		fbo.AttachRenderbuffer(
			FramebufferAttachment::Depth,
			rbo
		);

		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
	}

	void Render(double time)
	{
		// render into the texture
		fbo.Bind(Framebuffer::Target::Draw);
		gl.Viewport(tex_side, tex_side);
		gl.ClearDepth(1.0f);
		gl.ClearColor(0.4f, 0.9f, 0.4f, 1.0f);
		gl.Clear().ColorBuffer().DepthBuffer();
		torus_prog.Use();

		torus_projection_matrix.Set(
			CamMatrixf::PerspectiveX(Degrees(60), 1.0, 1, 30)
		);

		torus_camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.5,
				Degrees(time * 25),
				Degrees(SineWave(time / 30.0) * 90)
			)
		);

		torus_model_matrix.Set(
			ModelMatrixf::RotationA(
				Vec3f(1.0f, 1.0f, 1.0f),
				FullCircles(time * 0.5)
			)
		);

		torus.Bind();
		gl.FrontFace(make_torus.FaceWinding());
		torus_instr.Draw(torus_indices);

		// render the textured cube
		dfb.Bind(Framebuffer::Target::Draw);
		gl.Viewport(width, height);
		gl.ClearDepth(1.0f);
		gl.ClearColor(0.8f, 0.8f, 0.8f, 0.0f);
		gl.Clear().ColorBuffer().DepthBuffer();
		cube_prog.Use();

		cube_projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				float(width)/height,
				1, 30
			)
		);

		cube_camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.0,
				Degrees(time * 35),
				Degrees(SineWave(time / 20.0) * 60)
			)
		);

		cube_model_matrix.Set(
			ModelMatrixf::RotationX(FullCircles(time * 0.25))
		);

		cube.Bind();
		gl.FrontFace(make_cube.FaceWinding());
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
	return std::unique_ptr<Example>(new FBTexExample);
}

} // namespace oglplus
