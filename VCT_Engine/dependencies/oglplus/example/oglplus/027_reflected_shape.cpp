/**
 *  @example oglplus/027_reflected_shape.cpp
 *  @brief Show a how to do texture-based blurry reflection
 *
 *  @oglplus_screenshot{027_reflected_shape}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/twisted_torus.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class ReflectionExample : public Example
{
private:
	// helper object building and storing plane drawing instructions
	shapes::Plane make_plane;
	shapes::DrawingInstructions plane_instr;
	shapes::Plane::IndexArray plane_indices;


	// helper object building and storing shape drawing instructions
	typedef shapes::TwistedTorus Shape;
	Shape make_shape;
	shapes::DrawingInstructions shape_instr;
	Shape::IndexArray shape_indices;

	Context gl;

	VertexShader plane_vs, shape_vs;

	FragmentShader plane_fs, shape_fs;

	Program plane_prog, shape_prog;

	ProgramUniform<Mat4f>
		plane_projection_matrix, plane_camera_matrix, plane_model_matrix,
		shape_projection_matrix, shape_camera_matrix, shape_model_matrix;

	VertexArray plane, shape;

	Buffer plane_verts, plane_texcoords;
	Buffer shape_verts, shape_normals;

	Texture reflect_tex, depth_tex;

	Framebuffer fbo;
	DefaultFramebuffer dfb;

	GLuint width, height;
	const GLuint tex_size_div;
public:
	ReflectionExample(void)
	 : make_plane(
		Vec3f(),
		Vec3f(3.0f, 0.0f, 0.0f),
		Vec3f(0.0f, 0.0f, -3.0f),
		15,
		15
	), plane_instr(make_plane.Instructions())
	 , plane_indices(make_plane.Indices())
	 , make_shape()
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , plane_vs(ObjectDesc("Plane vertex"))
	 , shape_vs(ObjectDesc("Shape vertex"))
	 , plane_fs(ObjectDesc("Plane fragment"))
	 , shape_fs(ObjectDesc("Shape fragment"))
	 , plane_projection_matrix(plane_prog)
	 , plane_camera_matrix(plane_prog)
	 , plane_model_matrix(plane_prog)
	 , shape_projection_matrix(shape_prog)
	 , shape_camera_matrix(shape_prog)
	 , shape_model_matrix(shape_prog)
	 , width(800)
	 , height(600)
	 , tex_size_div(2)
	{
		plane_vs.Source(
			"#version 140\n"
			"uniform vec3 LightPosition;"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"out vec3 vertLightDir;"
			"out vec4 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix*Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"	vertTexCoord = gl_Position;"
			"}"
		);
		plane_vs.Compile();

		plane_fs.Source(
			"#version 140\n"
			"uniform sampler2DRect ReflectTex;"
			"uniform vec3 Normal;"
			"in vec3 vertLightDir;"
			"in vec4 vertTexCoord;"
			"out vec3 fragColor;"
			"const int n = 5;"
			"const int ns = (n*n);"
			"const float blur = 0.15/n;"
			"void main(void)"
			"{"
			"	float d = dot(Normal, normalize(vertLightDir));"
			"	float intensity = 0.5 + pow(1.4*d, 2.0);"
			"	vec3 color = vec3(0.0, 0.0, 0.0);"
			"	int n = 2;"
			"	float pct = 0.5/vertTexCoord.w;"
			"	for(int y=-n; y!=(n+1); ++y)"
			"	for(int x=-n; x!=(n+1); ++x)"
			"	{"
			"		vec2 coord = vertTexCoord.xy;"
			"		coord += vec2(blur*x, blur*y);"
			"		coord *= pct;"
			"		coord += vec2(0.5, 0.5);"
			"		coord *= textureSize(ReflectTex);"
			"		color += texture(ReflectTex, coord).rgb/ns;"
			"	}"
			"	fragColor = color*intensity;"
			"}"
		);
		plane_fs.Compile();

		plane_prog.AttachShader(plane_vs);
		plane_prog.AttachShader(plane_fs);
		plane_prog.Link();
		plane_prog.Use();

		plane_projection_matrix.BindTo("ProjectionMatrix");
		plane_camera_matrix.BindTo("CameraMatrix");
		plane_model_matrix.BindTo("ModelMatrix");

		Vec3f lightPos(3.0f, 0.5f, 2.0f);
		Uniform<Vec3f>(plane_prog, "LightPosition").Set(lightPos);
		Uniform<Vec3f>(plane_prog, "Normal").Set(make_plane.Normal());

		plane.Bind();

		plane_verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_plane.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(plane_prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}
		//
		Texture::Active(1);
		gl.Bound(Texture::Target::Rectangle, depth_tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge);

		Texture::Active(0);
		ProgramUniformSampler(plane_prog, "ReflectTex").Set(0);
		gl.Bound(Texture::Target::Rectangle, reflect_tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge);

		gl.Bound(Framebuffer::Target::Draw, fbo)
			.AttachTexture(
				FramebufferAttachment::Color,
				reflect_tex,
				0
			).AttachTexture(
				FramebufferAttachment::Depth,
				depth_tex,
				0
			);

		shape_vs.Source(
			"#version 140\n"
			"uniform vec3 LightPosition;"
			"uniform mat4 ProjectionMatrix, ModelMatrix, CameraMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"out vec3 vertNormal;"
			"out vec3 vertLightDir;"
			"out vec3 vertLightRefl;"
			"out vec3 vertViewDir;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertLightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		normalize(vertNormal)"
			"	);"
			"	vertViewDir = (vec4(0.0, 0.0, 1.0, 1.0)*CameraMatrix).xyz;"
			"	vertColor = vec3(1, 1, 1) - vertNormal;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		shape_vs.Compile();

		shape_fs.Source(
			"#version 140\n"
			"in vec3 vertNormal;"
			"in vec3 vertLightDir;"
			"in vec3 vertLightRefl;"
			"in vec3 vertViewDir;"
			"in vec3 vertColor;"
			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float l = length(vertLightDir);"
			"	float d = dot("
			"		normalize(vertNormal), "
			"		normalize(vertLightDir)"
			"	) / l;"
			"	float s = dot("
			"		normalize(vertLightRefl),"
			"		normalize(vertViewDir)"
			"	);"
			"	vec3 lt = vec3(1.0, 1.0, 1.0);"
			"	fragColor = "
			"		vertColor * 0.4 + "
			"		(lt + vertColor)*pow(max(2.5*d, 0.0), 3) + "
			"		lt * pow(max(s, 0.0), 64);"
			"}"
		);
		shape_fs.Compile();

		shape_prog.AttachShader(shape_vs);
		shape_prog.AttachShader(shape_fs);
		shape_prog.Link();
		shape_prog.Use();

		shape_projection_matrix.BindTo("ProjectionMatrix");
		shape_camera_matrix.BindTo("CameraMatrix");
		shape_model_matrix.BindTo("ModelMatrix");

		Uniform<Vec3f>(shape_prog, "LightPosition").Set(lightPos);

		shape.Bind();

		shape_verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_shape.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(shape_prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		shape_normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_shape.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(shape_prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}
		//
		gl.ClearColor(0.5f, 0.5f, 0.4f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		float aspect = float(width)/height;

		auto projection =
			CamMatrixf::PerspectiveX(Degrees(60), aspect, 1, 20);

		plane_projection_matrix.Set(projection);
		shape_projection_matrix.Set(projection);

		gl.Bound(Texture::Target::Rectangle, depth_tex).Image2D(
			0,
			PixelDataInternalFormat::DepthComponent,
			width/tex_size_div, height/tex_size_div,
			0,
			PixelDataFormat::DepthComponent,
			PixelDataType::Float,
			nullptr
		);
		gl.Bound(Texture::Target::Rectangle, reflect_tex).Image2D(
			0,
			PixelDataInternalFormat::RGB,
			width/tex_size_div, height/tex_size_div,
			0,
			PixelDataFormat::RGB,
			PixelDataType::UnsignedByte,
			nullptr
		);
	}

	void Render(double time)
	{
		static const ModelMatrixf reflection =
			ModelMatrixf::Translation(0.0f, -1.0f, 0.0f) *
			ModelMatrixf::Reflection(false, true, false);

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			4.5,
			FullCircles(time / 10.0),
			Degrees(45.0 - SineWave(time / 7.0)*35.0)
		);

		shape_prog.Use();
		shape.Bind();
		gl.FrontFace(make_shape.FaceWinding());

		shape_model_matrix =
			ModelMatrixf::Translation(0.0f, 1.1f, 0.0f) *
			ModelMatrixf::RotationX(FullCircles(time / 12.0));


		// render into the off-screen framebuffer
		fbo.Bind(Framebuffer::Target::Draw);
		gl.Viewport(width/tex_size_div, height/tex_size_div);
		gl.Clear().ColorBuffer().DepthBuffer();

		shape_camera_matrix = camera * reflection;

		gl.FrontFace(Inverted(make_shape.FaceWinding()));
		shape_instr.Draw(shape_indices);

		// render into the on-screen framebuffer
		dfb.Bind(Framebuffer::Target::Draw);
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();

		shape_camera_matrix = camera;

		gl.FrontFace(make_shape.FaceWinding());
		shape_instr.Draw(shape_indices);

		// Render the plane
		plane_prog.Use();
		plane.Bind();
		gl.FrontFace(make_plane.FaceWinding());

		plane_camera_matrix = camera;
		plane_model_matrix = ModelMatrixf::Translation(0.0f, -0.5f, 0.0f);

		plane_instr.Draw(plane_indices);
	}

	bool Continue(double time)
	{
		return time < 60.0;
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
	return std::unique_ptr<Example>(new ReflectionExample);
}

} // namespace oglplus
