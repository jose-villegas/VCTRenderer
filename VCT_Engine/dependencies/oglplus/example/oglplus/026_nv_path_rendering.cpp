/**
 *  @example oglplus/026_nv_path_rendering.cpp
 *  @brief Shows how to use the NV_path_rendering wrapper
 *
 *  @oglplus_screenshot{026_nv_path_rendering}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{THREADS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_2}
 *  @oglplus_example_uses_gl{GL_ARB_compatibility}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 *  @oglplus_example_uses_gl{GL_NV_path_rendering}
 */
#define GL_MODELVIEW  0x1700
#define GL_PROJECTION 0x1701

#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/image_spec.hpp>

#include <oglplus/opt/list_init.hpp>

#include <oglplus/ext/EXT_direct_state_access.hpp>
#include <oglplus/ext/NV_path_rendering.hpp>

#include <vector>
#include <sstream>
#include <iomanip>

#include "example.hpp"
#include "example_mt.hpp"

namespace oglplus {

class FBTexThread;

class FBTexExample : public Example
{
private:
	friend class FBTexThread;

	Context gl;

	VertexShader vertex_shader;

	Program prog;

	Program make_prog(void)
	{
		Program result(ObjectDesc("Main"));

		vertex_shader.Source(
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
		).Compile();
		result.AttachShader(vertex_shader);

		FragmentShader fs(ObjectDesc("Main fragment"));
		fs.Source(
			"#version 140\n"
			"uniform sampler2D TexUnit;"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = dot(vertNormal, normalize(vertLight));"
			"	float i = 0.6 + max(d, 0.0);"
			"	fragColor = texture(TexUnit, vertTexCoord)*i;"
			"}"
		);
		fs.Compile();
		result.AttachShader(fs);

		result.Link().Use();

		return std::move(result);
	}

	// Handle for matrix uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	shapes::ShapeWrapper cube;

	GLuint width, height;

	Texture tex;

	DefaultFramebuffer dfb;

	ExampleSyncQueue  parent_ready;
	ExampleSyncQueue* thread_ready;
public:
	FBTexExample(void)
	 : gl()
	 , vertex_shader(ObjectDesc("Vertex"))
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , cube(List("Position")("Normal")("TexCoord").Get(), shapes::Cube(), prog)
	 , width(800)
	 , height(600)
	{
		UniformSampler(prog, "TexUnit").Set(0);
		Uniform<Vec3f>(prog, "LightPos").Set(40.0f, 40.0f, -80.0f);

		Texture::Active(0);
		tex.Bind(Texture::Target::_2D);

		Use();
	}

	void SetProjection(void)
	{
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(54),
				width, height,
				1, 100
			)
		);
	}

	void Use(void)
	{
		gl.ClearDepth(1.0f);
		gl.ClearColor(0.8f, 0.8f, 0.8f, 0.0f);

		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);

		dfb.Bind(Framebuffer::Target::Draw);
		gl.Viewport(width, height);

		prog.Use();
		cube.Use();

		SetProjection();
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		gl.Viewport(width, height);
		SetProjection();
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.0,
				Degrees(time * 35),
				Degrees(SineWave(time / 20.0) * 60)
			)
		);

		model_matrix.Set(ModelMatrixf::RotationX(FullCircles(time * 0.25)));

		assert(thread_ready);
		thread_ready->Wait();

		cube.Draw();

		parent_ready.Signal();
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

class FBTexThread
 : public ExampleThread
{
private:
	Context gl;

	EXT_direct_state_access dsa;
	NV_path_rendering npr;

	PathNV shape;
	PathArrayNV text_glyphs;
	std::vector<GLfloat> glyph_spacings;

	GLfloat font_y_min, font_y_max, font_height;

	Renderbuffer rbo;
	Framebuffer fbo;

	GLuint tex_side;
	unsigned frame_no;

	ExampleSyncQueue  thread_ready;
	ExampleSyncQueue& parent_ready;
public:
	FBTexThread(FBTexExample& example)
	 : gl()
	 , dsa()
	 , npr()
	 , text_glyphs(128)
	 , tex_side(512)
	 , frame_no(0)
	 , parent_ready(example.parent_ready)
	{
		example.thread_ready = &thread_ready;

		Texture::Active(0);
		example.tex
			<< TextureTarget::_2D
			<< TextureFilter::Linear
			<< TextureWrap::Repeat
			<< images::ImageSpec(
				tex_side, tex_side,
				PixelDataFormat::RGBA,
				PixelDataType::UnsignedByte
			);

		rbo	<< RenderbufferTarget::Renderbuffer
			<< images::ImageSpec(
				tex_side, tex_side,
				PixelDataInternalFormat::StencilIndex8
			);

		fbo	<< FramebufferTarget::Draw
			<< FramebufferAttachment::Color << example.tex
			<< FramebufferAttachment::Stencil << rbo;

		shape.SVGString(
		"m 43.371378,37.539671"
		"c 13.611668,15.8582"
		"  19.209264,30.90734"
		"  23.672964,45.82925"
		"  3.661218,8.4554"
		"  12.165653,10.28682"
		"  20.035286,12.9651"
		"l 23.240262,-25.01766"
		"c -1.46397,-11.60113"
		"  -7.97625,-19.26946"
		"  -17.458566,-24.59093"
		"  -16.102248,-6.17383"
		"  -32.841826,-7.28036"
		"  -49.489946,-9.18576"
		"z"
		"m 69.224042,34.9532"
		"  -24.470964,26.1184"
		"c -0.424168,2.860869"
		"  1.771705,3.638839"
		"  4.947536,3.651769"
		"L 117.01841,76.122411"
		"c -0.6978,-1.73312"
		"  -1.57354,-3.37403"
		"  -4.42299,-3.62954"
		"z"
		"m 96.74348,13.86489"
		"  -3.83844,0.0475"
		"  -30.37174,32.119769"
		"  -1.09321,3.59966"
		"  21.04835,15.49073"
		"  3.9258,-0.73053"
		"  30.37173,-32.11976"
		"  0.34543,-2.70113"
		"z"
		"m -90.1373,-8.06177"
		"  -23.478345,25.779639"
		"  34.731575,28.08879"
		"  19.92164,-19.41255"
		"  6.63604,-2.40327"
		"  15.2551,11.02897"
		"  1.86981,-4.99851"
		"z"
		"m 121.25631,22.884729"
		"  -5.63826,0.64963"
		"  -34.50826,36.69755"
		"  0.17514,2.75441"
		"  17.44637,12.03959"
		"  1.34218,3.25131"
		"  -11.42007,14.24354"
		"  3.90638,3.5452"
		"  3.07023,-2.01334"
		"  47.96771,-55.8908"
		"c -5.2881,-6.90958"
		"  -13.00669,-13.42756"
		"  -22.34142,-15.27709"
		"z"
		"m -82.97086,12.33038"
		"  -4.96607,1.44186"
		"c -16.74585,15.3575"
		"  -31.13455,31.43379"
		"  -42.31266,48.51177"
		"  14.32975,15.18082"
		"  29.75001,29.95699"
		"  50.10018,42.90383"
		"l -12.86997,89.36422"
		"  76.0707,7.53421"
		"  4.08806,-49.56176"
		"c 2.63515,-16.68351"
		"  11.16889,-28.57327"
		"  20.4365,-35.81021"
		"l -43.78946,-30.11675"
		"c -7.37799,-6.14328"
		"  -6.76172,-11.95465"
		"  -30.59946,-14.4867"
		"  -7.3681,-8.26173"
		"  -16.91998,-15.58704"
		"  -17.77179,-25.44079"
		"l 3.19111,-1.30899"
		"c 5.66966,8.17456"
		"  5.04135,19.14781"
		"  19.90453,26.63415"
		"  16.79016,-0.42901"
		"  33.48024,-8.77684"
		"  37.51318,-17.5806"
		"z"
		"m 100.47604,12.17096"
		"  -41.13754,47.8457"
		"  -1.28556,3.45416"
		"  19.69984,15.03817"
		"  3.00685,-2.61745"
		"  39.98919,-47.11398"
		"  0.20368,-2.96727"
		"c -5.20109,-4.99"
		"  -10.11474,-10.17304"
		"  -17.87754,-13.34188"
		"z"
		"m 17.21754,25.10098"
		"  -2.34127,0.98707"
		"  -32.55714,39.03611"
		"  -2.52598,2.98121"
		"  15.36908,11.51245"
		"  4.68518,0.47953"
		"  34.77739,-39.62667"
		"  -0.55381,-3.64243"
		"c -3.06075,-4.58721"
		"  -10.50061,-10.2548"
		"  -16.85345,-11.72727"
		"z"
		"m -72.86065,19.8761"
		"  -15.2679,3.0925"
		"c 26.57114,13.9239"
		"  43.79716,29.59996"
		"  65.69469,44.40557"
		"  31.09815,22.86624"
		"  63.72592,40.27978"
		"  100.34072,52.54386"
		"  -19.7004,-24.16603"
		"  -45.48586,-53.7133"
		"  -80.19406,-78.6645"
		"l -13.63751,16.18691"
		"  -7.41411,-0.0354"
		"z"
		"m -60.76927,125.44609"
		"  -8.79676,44.89633"
		"  92.36105,9.4532"
		"  3.89632,-45.39793"
		"z"
		);

		GLfloat font_scale = 48;

		text_glyphs.GlyphRange(
			PathNVFontTarget::Standard,
			"Sans",
			PathNVFontStyle::Bold,
			0, 128,
			PathNVMissingGlyph::Use,
			~0,
			font_scale
		);

		gl.ClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		gl.ClearStencil(0);
		gl.StencilMask(~0);
		gl.StencilFunc(CompareFunction::NotEqual, 0, 0xFF);
		gl.StencilOp(
			StencilOperation::Keep,
			StencilOperation::Keep,
			StencilOperation::Zero
		);

		gl.Enable(Capability::StencilTest);

		gl.Viewport(tex_side, tex_side);
		dsa.ProjectionMatrix()
			.LoadIdentity()
			.Ortho(0, tex_side, 0, tex_side, -1, 1);

		GLfloat font_min_max[2];
		text_glyphs.GetMetricRange(
			PathNVMetricQuery::FontYMinBounds|
			PathNVMetricQuery::FontYMaxBounds,
			1, 0,
			font_min_max
		);
		font_y_min = font_min_max[0];
		font_y_max = font_min_max[1];
		font_height = font_y_max - font_y_min;
	}

	void RenderText(const String& text)
	{
		glyph_spacings.resize(text.size()+1);
		text_glyphs.GetSpacing(
			PathNVListMode::AccumAdjacentPairs,
			text,
			1.0f, 1.0f,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
		glyph_spacings.insert(glyph_spacings.begin(), 0);

		text_glyphs.StencilFillInstanced(
			text,
			PathNVFillMode::CountUp,
			0xFF,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
		text_glyphs.CoverFillInstanced(
			text,
			PathNVFillCoverMode::BoundingBoxOfBoundingBoxes,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
	}

	void Cancel(void)
	{
		parent_ready.Cancel();
		thread_ready.Cancel();
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().StencilBuffer();

		int border = 20;
		std::stringstream text;

		// shape colors
		GLfloat shape_color_gen_coeffs[9] = {
			 0.0f, 0.0f, 0.2f,
			 0.2f, 0.0f, 0.2f,
			 0.0f, 0.0f, 0.2f
		};
		npr.ColorGen(
			PathNVColor::Primary,
			PathNVGenMode::ObjectBoundingBox,
			PathNVColorFormat::RGB,
			shape_color_gen_coeffs
		);

		// Shape
		dsa.ModelviewMatrix()
			.LoadIdentity()
			.Translate(64, 64, 0)
			.Translate(192, 192, 0)
			.Rotate(RightAngles(time), Vec3f::Unit(2))
			.Translate(-192, -192, 0);
		shape.StencilFill(PathNVFillMode::CountUp, 0xFF);
		shape.CoverFill(PathNVFillCoverMode::BoundingBox);

		// text colors
		GLfloat text_color_gen_coeffs[9] = {
			-0.3f, 0.0f, 0.3f,
			 0.0f, 0.2f, 0.6f,
			 0.3f, 0.0f, 0.0f
		};
		npr.ColorGen(
			PathNVColor::Primary,
			PathNVGenMode::ObjectBoundingBox,
			PathNVColorFormat::RGB,
			text_color_gen_coeffs
		);

		// Time
		dsa.ModelviewMatrix()
			.LoadIdentity()
			.Translate(border, border, 0);
		text	<< "Time: "
			<< std::setw(7)
			<< std::setprecision(2)
			<< std::fixed
			<< time << "[s]";
		RenderText(text.str());
		text.str(std::string());

		// Frame no
		dsa.ModelviewMatrix()
			.LoadIdentity()
			.Translate(tex_side-border, border, 0)
			.Rotate(RightAngle(), Vec3f::Unit(2));
		text	<< "Frame: "
			<< std::setw(7)
			<< std::fixed
			<< frame_no;
		RenderText(text.str());
		text.str(std::string());

		// FPS
		dsa.ModelviewMatrix()
			.LoadIdentity()
			.Translate(tex_side-border, tex_side-border, 0)
			.Rotate(RightAngles(2), Vec3f::Unit(2));
		text	<< "FPS: ";
		if(time < 1) text << "<N/A>";
		else text << std::setw(7) << std::fixed << frame_no / time;
		RenderText(text.str());
		text.str(std::string());

		// title
		dsa.ModelviewMatrix()
			.LoadIdentity()
			.Translate(border, tex_side-border, 0)
			.Rotate(RightAngles(3), Vec3f::Unit(2));
		RenderText("OpenGL");

		// sync
		thread_ready.Signal();
		parent_ready.Wait();

		++frame_no;
	}
};

void setupExample(ExampleParams& params)
{
	if(params.max_threads > 0)
	{
		params.num_threads = 1;
		params.compat_context_threads.insert(0);
	}
	else throw std::runtime_error(
		"This example requires multi-threading"
	);
}

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& example,
	unsigned thread_id,
	const ExampleParams& /*params*/
)
{
	assert(thread_id == 0);

	return std::unique_ptr<ExampleThread>(
		new FBTexThread(
			dynamic_cast<FBTexExample&>(example)
		)
	);
}

std::unique_ptr<Example> makeExample(const ExampleParams& /*params*/)
{
	return std::unique_ptr<Example>(new FBTexExample);
}

} // namespace oglplus
