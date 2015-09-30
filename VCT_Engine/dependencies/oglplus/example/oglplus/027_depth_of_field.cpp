/**
 *  @example oglplus/027_depth_of_field.cpp
 *  @brief Shows simple simulation of the Depth-of-field effect
 *
 *  @oglplus_screenshot{027_depth_of_field}
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
#include <oglplus/shapes/cube.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class DOFExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions face_instr, edge_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray face_indices, edge_indices;

	std::vector<Mat4f> cube_matrices;

	// Returns a vector of cube offsets
	static std::vector<Mat4f> MakeCubeMatrices(GLuint count, float max_dist)
	{
		std::srand(59039);
		std::vector<Mat4f> offsets(count);
		for(GLuint i=0; i!=count; ++i)
		{
			float x = float(std::rand())/RAND_MAX;
			float y = float(std::rand())/RAND_MAX;
			float z = float(std::rand())/RAND_MAX;
			float sx = std::rand()%2 ? 1.0f: -1.0f;
			float sy = std::rand()%2 ? 1.0f: -1.0f;
			float sz = std::rand()%2 ? 1.0f: -1.0f;
			offsets[i] =
				ModelMatrixf::Translation(
					sx*(1.0f + std::pow(x, 0.9f) * max_dist),
					sy*(1.0f + std::pow(y, 1.5f) * max_dist),
					sz*(1.0f + std::pow(z, 0.7f) * max_dist)
				) *
				ModelMatrixf::RotationZ(
					RightAngles(float(std::rand())/RAND_MAX)
				) *
				ModelMatrixf::RotationY(
					RightAngles(float(std::rand())/RAND_MAX)
				) *
				ModelMatrixf::RotationX(
					RightAngles(float(std::rand())/RAND_MAX)
				);
		}
		return offsets;
	}

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader main_vs, dof_vs;

	// Fragment shader
	FragmentShader main_fs, dof_fs;

	// Program
	Program main_prog, dof_prog;

	// A vertex array object for the rendered cube and the postproc screen
	VertexArray cube, screen;

	// VBOs for the cube's and screens vertex attribs
	Buffer positions, normals, corners;

	// Program uniform variables
	Lazy<ProgramUniform<GLuint>> viewport_width;
	Lazy<ProgramUniform<GLuint>> viewport_height;
	Lazy<ProgramUniform<Mat4f>> projection_matrix, camera_matrix, model_matrix;
	Lazy<ProgramUniform<Vec3f>> ambient_color, diffuse_color;
	Lazy<ProgramUniform<GLfloat>> focus_depth;

	// The framebuffer object of offscreen rendering
	Framebuffer fbo;

	DefaultFramebuffer dfb;

	// The color and depth textures
	Texture color_tex, depth_tex;

	GLuint width, height;
public:
	DOFExample(const ExampleParams& params)
	 : face_instr(make_cube.Instructions(shapes::DrawMode::Default()))
	 , edge_instr(make_cube.Instructions(shapes::DrawMode::Edges()))
	 , face_indices(make_cube.Indices(shapes::DrawMode::Default()))
	 , edge_indices(make_cube.Indices(shapes::DrawMode::Edges()))
	 , cube_matrices(MakeCubeMatrices(100, 10.0))
	 , viewport_width(dof_prog, "ViewportWidth")
	 , viewport_height(dof_prog, "ViewportHeight")
	 , projection_matrix(main_prog, "ProjectionMatrix")
	 , camera_matrix(main_prog, "CameraMatrix")
	 , model_matrix(main_prog, "ModelMatrix")
	 , ambient_color(main_prog, "AmbientColor")
	 , diffuse_color(main_prog, "DiffuseColor")
	 , focus_depth(dof_prog, "FocusDepth")
	 , width(800)
	 , height(600)
	{
		main_vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"uniform vec3 LightPos;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"out vec3 vertLightDir;"
			"out vec3 vertNormal;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = normalize(LightPos - gl_Position.xyz);"
			"	vertNormal = normalize(mat3(ModelMatrix)*Normal);"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		// compile it
		main_vs.Compile();

		// set the fragment shader source
		main_fs.Source(
			"#version 140\n"
			"uniform vec3 AmbientColor, DiffuseColor;"
			"in vec3 vertLightDir;"
			"in vec3 vertNormal;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = max(dot(vertLightDir,vertNormal),0.0);"
			"	float e = sin("
			"		10.0*vertLightDir.x + "
			"		20.0*vertLightDir.y + "
			"		25.0*vertLightDir.z   "
			"	)*0.9;"
			"	fragColor = vec4("
			"		mix(AmbientColor, DiffuseColor, d+e),"
			"		1.0"
			"	);"
			"}"
		);
		// compile it
		main_fs.Compile();

		// attach the shaders to the program
		main_prog.AttachShader(main_vs);
		main_prog.AttachShader(main_fs);
		// link and use it
		main_prog.Link();
		main_prog.Use();

		projection_matrix.Init();
		camera_matrix.Init();
		model_matrix.Init();
		ambient_color.Init();
		diffuse_color.Init();

		// bind the VAO for the cube
		gl.Bind(cube);

		// bind the VBO for the cube vertices
		gl.Bind(Buffer::Target::Array, positions);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(main_prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the cube normals
		gl.Bind(Buffer::Target::Array, normals);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(main_prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		Uniform<Vec3f>(main_prog, "LightPos").Set(30.0f, 50.0f, 20.0f);

		dof_vs.Source(
			"#version 140\n"
			"uniform uint ViewportWidth, ViewportHeight;"
			"in vec4 Position;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertTexCoord = vec2("
			"		(Position.x*0.5 + 0.5)*ViewportWidth,"
			"		(Position.y*0.5 + 0.5)*ViewportHeight"
			"	);"
			"}"
		);
		dof_vs.Compile();

		dof_fs.Source(
			"#version 140\n"
			"uniform sampler2DRect ColorTex;"
			"uniform sampler2DRect DepthTex;"
			"uniform float FocusDepth;"
			"uniform uint SampleMult;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"const float strength = 16.0;"
			"void main(void)"
			"{"
			"	float fragDepth = texture(DepthTex, vertTexCoord).r;"
			"	vec3 color = texture(ColorTex, vertTexCoord).rgb;"
			"	float of = abs(fragDepth - FocusDepth);"
			"	int nsam = int(of*SampleMult);"
			"	float inv_nsam = 1.0 / (1.0 + nsam);"
			"	float astep = (3.14151*4.0)/nsam;"
			"	for(int i=0; i!=nsam; ++i)"
			"	{"
			"		float a = i*astep;"
			"		float d = sqrt(i*inv_nsam);"
			"		float sx = cos(a)*of*strength*d;"
			"		float sy = sin(a)*of*strength*d;"
			"		vec2 samTexCoord = vertTexCoord + vec2(sx, sy) + noise2(vec2(sx, sy));"
			"		color += texture(ColorTex, samTexCoord).rgb;"
			"	}"
			"	fragColor = vec4(color * inv_nsam , 1.0);"
			"}"
		);
		dof_fs.Compile();

		dof_prog.AttachShader(dof_vs);
		dof_prog.AttachShader(dof_fs);
		dof_prog.Link();
		dof_prog.Use();

		viewport_width.Init();
		viewport_height.Init();
		focus_depth.Init();

		GLuint sample_mult = params.HighQuality()?512:128;
		Uniform<GLuint>(dof_prog, "SampleMult") = sample_mult;

		// bind the VAO for the screen
		gl.Bind(screen);

		gl.Bind(Buffer::Target::Array, corners);
		{
			GLfloat screen_verts[8] = {
				-1.0f, -1.0f,
				-1.0f,  1.0f,
				 1.0f, -1.0f,
				 1.0f,  1.0f
			};
			Buffer::Data(Buffer::Target::Array, 8, screen_verts);
			VertexArrayAttrib attr(dof_prog, "Position");
			attr.Setup<Vec2f>();
			attr.Enable();
		}

		Texture::Active(0);
		UniformSampler(dof_prog, "ColorTex").Set(0);
		gl.Bound(Texture::Target::Rectangle, color_tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.Image2D(
				0,
				PixelDataInternalFormat::RGB,
				width, height,
				0,
				PixelDataFormat::RGB,
				PixelDataType::UnsignedByte,
				nullptr
			);

		Texture::Active(1);
		UniformSampler(dof_prog, "DepthTex").Set(1);
		gl.Bound(Texture::Target::Rectangle, depth_tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.Image2D(
				0,
				PixelDataInternalFormat::DepthComponent,
				width, height,
				0,
				PixelDataFormat::DepthComponent,
				PixelDataType::Float,
				nullptr
			);

		gl.Bound(Framebuffer::Target::Draw, fbo)
			.AttachTexture(
				FramebufferAttachment::Color,
				color_tex,
				0
			).AttachTexture(
				FramebufferAttachment::Depth,
				depth_tex,
				0
			);

		//
		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.DepthFunc(CompareFn::LEqual);
		gl.Enable(Capability::LineSmooth);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		viewport_width.Set(width);
		viewport_height.Set(height);

		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(65),
				float(width)/height,
				4.0, 50.0
			)
		);

		gl.Bound(Texture::Target::Rectangle, color_tex).Image2D(
			0,
			PixelDataInternalFormat::RGB,
			width, height,
			0,
			PixelDataFormat::RGB,
			PixelDataType::UnsignedByte,
			nullptr
		);

		gl.Bound(Texture::Target::Rectangle, depth_tex).Image2D(
			0,
			PixelDataInternalFormat::DepthComponent,
			width, height,
			0,
			PixelDataFormat::DepthComponent,
			PixelDataType::Float,
			nullptr
		);
		gl.Viewport(width, height);
	}

	void Render(double time)
	{
		gl.Bind(Framebuffer::Target::Draw, fbo);

		gl.Clear().ColorBuffer().DepthBuffer();

		gl.Use(main_prog);
		gl.Bind(cube);

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				20.5,
				FullCircles(time / 20.0),
				Degrees(SineWave(time / 25.0) * 30)
			)
		);

		auto i = cube_matrices.begin(), e = cube_matrices.end();
		while(i != e)
		{
			model_matrix.Set(*i);
			ambient_color.Set(0.7f, 0.6f, 0.2f);
			diffuse_color.Set(1.0f, 0.8f, 0.3f);
			face_instr.Draw(face_indices);

			ambient_color.Set(0.1f, 0.1f, 0.1f);
			diffuse_color.Set(0.3f, 0.3f, 0.3f);
			edge_instr.Draw(edge_indices);
			++i;
		}

		gl.Bind(Framebuffer::Target::Draw, dfb);

		gl.Clear().ColorBuffer().DepthBuffer();

		dof_prog.Use();
		gl.Bind(screen);

		focus_depth.Set(GLfloat(0.6 + SineWave(time / 9.0)*0.3));

		gl.Enable(Capability::Blend);
		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
		gl.Disable(Capability::Blend);
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}

	double ScreenshotTime(void) const
	{
		return 5.0;
	}
};

void setupExample(ExampleParams& /*params*/){ }

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& /*example*/,
	unsigned /*thread_id*/,
	const ExampleParams& /*params*/
){ return std::unique_ptr<ExampleThread>(); }

std::unique_ptr<Example> makeExample(const ExampleParams& params)
{
	return std::unique_ptr<Example>(new DOFExample(params));
}

} // namespace oglplus
