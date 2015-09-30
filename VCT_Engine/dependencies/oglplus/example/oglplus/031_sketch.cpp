/**
 *  @example oglplus/031_sketch.cpp
 *  @brief Shows how to achieve a hand sketch effect
 *
 *  @oglplus_screenshot{031_sketch}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/wicker_torus.hpp>

#include <oglplus/images/brushed_metal.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>

#include <cmath>
#include <functional>

#include "example.hpp"

namespace oglplus {

class TransformProgram : public Program
{
private:
	static Program make(void)
	{
		const GLchar* shader_source =
		"#version 150\n"
		"uniform mat4 CameraMatrix, ModelMatrix;"
		"uniform mat4 LightProjMatrix;"
		"uniform mat2 TextureMatrix;"
		"uniform vec3 CameraPosition, LightPosition;"
		"in vec4 Position;"
		"in vec3 Normal;"
		"in vec2 TexCoord;"

		"out gl_PerVertex {"
		"	vec4 gl_Position;"
		"};"
		"out float vertNoise;"
		"out vec3 vertNormal;"
		"out vec3 vertLightDir, vertViewDir;"
		"out vec2 vertTexCoord;"
		"out vec4 vertLightTexCoord;"
		"void main(void)"
		"{"
		"	gl_Position = "
		"		ModelMatrix *"
		"		Position;"
		"	vertNoise = noise1(Position.x+Position.y+Position.z);"
		"	vertLightDir = LightPosition - gl_Position.xyz;"
		"	vertViewDir = CameraPosition - gl_Position.xyz;"
		"	vertNormal =  (ModelMatrix * vec4(Normal,  0.0)).xyz;"
		"	vertTexCoord = TextureMatrix * TexCoord;"
		"	vertLightTexCoord = LightProjMatrix* gl_Position;"
		"	gl_Position = CameraMatrix * gl_Position;"
		"}";

		return ShaderProgram(
			ShaderType::Vertex,
			shader_source,
			ObjectDesc("Transform")
		);
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix, model_matrix, light_proj_matrix;
	ProgramUniform<Mat2f> texture_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;

	TransformProgram(void)
	 : Program(make())
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , light_proj_matrix(prog(), "LightProjMatrix")
	 , texture_matrix(prog(), "TextureMatrix")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	{ }
};

class ShadowFragShader
 : public FragmentShader
{
public:
	ShadowFragShader(void)
	 : FragmentShader(
		ObjectDesc("Shadow fragment shader"),
		"#version 150\n"
		"in float vertNoise;"
		"in vec3 vertNormal;"
		"in vec3 vertLightDir, vertViewDir;"
		"in vec2 vertTexCoord;"
		"in vec4 vertLightTexCoord;"

		"void main(void){ }"
	)
	{ }
};

class ShadowProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Shadow"));
		prog.AttachShader(ShadowFragShader());
		prog.MakeSeparable().Link().Use();
		return prog;
	}
public:
	ShadowProgram(void)
	 : Program(make())
	{ }
};

class LineGeomShader
 : public GeometryShader
{
public:
	LineGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Line geometry shader"),
		"#version 150\n"
		"layout(lines) in;"
		"layout(line_strip, max_vertices=4) out;"

		"in gl_PerVertex {"
		"	vec4 gl_Position;"
		"} gl_in[];"
		"in float vertNoise[];"
		"in vec3 vertNormal[];"
		"in vec3 vertLightDir[], vertViewDir[];"
		"in vec2 vertTexCoord[];"
		"in vec4 vertLightTexCoord[];"

		"out gl_PerVertex {"
		"	vec4 gl_Position;"
		"};"
		"out float geomOpacity;"

		"void main(void)"
		"{"
		"	vec4 p0 = gl_in[0].gl_Position;"
		"	vec4 p1 = gl_in[1].gl_Position;"
		"	vec4 n0 = vec4(vertNormal[0], 0.0);"
		"	vec4 n1 = vec4(vertNormal[1], 0.0);"
		"	float dp = pow(length(p1 - p0), 0.25);"
		"	float l0 = cos(vertNoise[0]*1.618)*dp;"
		"	float l1 = sin(vertNoise[1]*1.618)*dp;"
		"	vec4 v0 = p0 + n0*l0*0.01;"
		"	vec4 v1 = p1 + n1*l1*0.01;"
		"	vec4 v = v1 - v0;"

		"	gl_Position = v0 - v*abs(l1*0.41);"
		"	geomOpacity = 0.0;"
		"	EmitVertex();"

		"	gl_Position = p0;"
		"	geomOpacity = 1.0;"
		"	EmitVertex();"

		"	gl_Position = p1;"
		"	geomOpacity = 1.0;"
		"	EmitVertex();"

		"	gl_Position = v1 + v*abs(l0*0.44);"
		"	geomOpacity = 0.0;"
		"	EmitVertex();"

		"	EndPrimitive();"
		"}"
	)
	{ }
};

class LineFragShader
 : public FragmentShader
{
public:
	LineFragShader(void)
	 : FragmentShader(
		ObjectDesc("Line fragment shader"),
		"#version 150\n"
		"in float geomOpacity;"

		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	fragColor = vec4(0.0, 0.0, 0.0, 0.1+0.4*geomOpacity);"
		"}"
	)
	{ }
};

class LineProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Line"));
		prog.AttachShader(LineGeomShader());
		prog.AttachShader(LineFragShader());
		prog.MakeSeparable().Link().Use();
		return prog;
	}
public:
	LineProgram(void)
	 : Program(make())
	{ }
};

class SketchFragShader
 : public FragmentShader
{
public:
	SketchFragShader(void)
	 : FragmentShader(
		ObjectDesc("Sketch fragment shader"),
		"#version 150\n"
		"uniform sampler3D SketchTex;"
		"uniform sampler2DShadow ShadowTex;"

		"in float vertNoise;"
		"in vec3 vertNormal;"
		"in vec3 vertLightDir, vertViewDir;"
		"in vec2 vertTexCoord;"
		"in vec4 vertLightTexCoord;"
		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	float LightMult = 1.0;"

		"	vec3 LightProjCoord = ("
		"		vertLightTexCoord.xyz /"
		"		vertLightTexCoord.w"
		"	) * 0.5 + 0.5;"
		"	if("
		"		LightProjCoord.x >= 0.0 && "
		"		LightProjCoord.x <= 1.0 && "
		"		LightProjCoord.y >= 0.0 && "
		"		LightProjCoord.y <= 1.0 && "
		"		LightProjCoord.z <= 1.0"
		"	)"
		"	{"
		"		float Shadow = 0.0;"
		"		const int sn = 12;"
		"		const float o = 1.0/32.0;"
		"		for(int s=0; s!=sn; ++s)"
		"		{"
		"			float r = float(s)/sn;"
		"			float a = 4.0*3.14151*r;"
		"			Shadow += texture("
		"				ShadowTex, "
		"				LightProjCoord+"
		"				vec3(cos(a)*o*r, sin(a)*o*r, 0.0)"
		"			);"
		"		}"
		"		LightMult *= (Shadow / sn);"
		"	}"

		"	vec3 LightRefl = reflect("
		"		-normalize(vertLightDir),"
		"		normalize(vertNormal)"
		"	);"

		"	float Specular = pow(max(dot("
		"		normalize(LightRefl),"
		"		normalize(vertViewDir)"
		"	)+0.02, 0.0)*1.1, 2.0);"

		"	float Diffuse = max(dot("
		"		normalize(vertNormal), "
		"		normalize(vertLightDir)"
		"	), 0.0)*3.5;"

		"	float Ambient = 0.1;"

		"	float Light = Ambient + (Diffuse + Specular)*LightMult;"
		"	float Shadow = clamp(2.0 - Light, 0.0, 1.0);"
		"	vec3 Sample = texture(SketchTex, vec3(vertTexCoord, Shadow)).rgb;"

		"	fragColor = vec4(0.0, 0.0, 0.0, pow(Sample.b*(0.5 + Shadow*0.5), 0.5));"
		"}"
	)
	{ }
};

class SketchProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Sketch"));
		prog.AttachShader(SketchFragShader());
		prog.MakeSeparable().Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniformSampler sketch_tex, shadow_tex;

	SketchProgram(void)
	 : Program(make())
	 , sketch_tex(prog(), "SketchTex")
	 , shadow_tex(prog(), "ShadowTex")
	{ }
};


template <typename ShapeBuilder>
class Shape
{
protected:
	// helper object building shape vertex attributes
	ShapeBuilder make_shape;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions shape_instr, edge_instr;
	// indices pointing to shape primitive elements
	typename ShapeBuilder::IndexArray shape_indices, edge_indices;

	Context gl;

	// A vertex array object for the rendered shape
	VertexArray vao;

	// VBOs for the shape's vertex attributes
	Array<Buffer> vbos;

public:
	Shape(const Program& prog, const ShapeBuilder& builder)
	 : make_shape(builder)
	 , shape_instr(make_shape.Instructions(shapes::DrawMode::Default()))
	 , edge_instr(make_shape.Instructions(shapes::DrawMode::Edges()))
	 , shape_indices(make_shape.Indices(shapes::DrawMode::Default()))
	 , edge_indices(make_shape.Indices(shapes::DrawMode::Edges()))
	 , vbos(3)
	{
		// bind the VAO for the shape
		vao.Bind();

		typename ShapeBuilder::VertexAttribs vert_attr_info;
		const GLuint nva = 3;
		const GLchar* vert_attr_name[nva] = {
			"Position",
			"Normal",
			"TexCoord"
		};
		for(GLuint va=0; va!=nva; ++va)
		{
			const GLchar* name = vert_attr_name[va];
			std::vector<GLfloat> data;
			auto getter = vert_attr_info.VertexAttribGetter(data, name);
			if(getter != nullptr)
			{
				// bind the VBO for the vertex attribute
				vbos[va].Bind(Buffer::Target::Array);
				GLuint n_per_vertex = getter(make_shape, data);
				// upload the data
				Buffer::Data(Buffer::Target::Array, data);
				// setup the vertex attribs array
				VertexArrayAttrib attr(prog, name);
				attr.Setup<GLfloat>(n_per_vertex);
				attr.Enable();
			}
		}
	}

	void Draw(void)
	{
		vao.Bind();
		gl.FrontFace(make_shape.FaceWinding());
		shape_instr.Draw(shape_indices, 1);
	}

	void Draw(const std::function<bool (GLuint)>& drawing_driver)
	{
		vao.Bind();
		gl.FrontFace(make_shape.FaceWinding());
		shape_instr.Draw(shape_indices, 1, 0, drawing_driver);
	}

	void DrawEdges(void)
	{
		vao.Bind();
		edge_instr.Draw(edge_indices);
	}
};

class SketchExample : public Example
{
private:

	// wrapper around the current OpenGL context
	Context gl;

	TransformProgram transf_prog;
	ShadowProgram shadow_prog;
	SketchProgram sketch_prog;
	LineProgram line_prog;

	ProgramPipeline shadow_pp, sketch_pp, line_pp;

	Shape<shapes::Plane> plane;
	Shape<shapes::WickerTorus> torus;

	// The sketch texture
	Texture sketch_texture;

	GLuint width, height;
	const GLuint sketch_tex_layers, shadow_tex_side;

	// The metal frame shadow texture and its framebuffer
	Texture shadow_tex;
	Framebuffer frame_shadow_fbo;

public:
	SketchExample(void)
	 : transf_prog()
	 , sketch_prog()
	 , plane(
		transf_prog,
		shapes::Plane(
			Vec3f(0, 0, 0),
			Vec3f(9, 0, 0),
			Vec3f(0, 0,-9),
			9, 9
		)
	), torus(transf_prog, shapes::WickerTorus())
	 , sketch_tex_layers(8)
	 , shadow_tex_side(1024)
	{

		NoProgram().Use();

		shadow_pp.Bind();
		shadow_pp.UseStages(transf_prog).Vertex();
		shadow_pp.UseStages(shadow_prog).Fragment();

		sketch_pp.Bind();
		sketch_pp.UseStages(transf_prog).Vertex();
		sketch_pp.UseStages(sketch_prog).Fragment();

		line_pp.Bind();
		line_pp.UseStages(transf_prog).Vertex();
		line_pp.UseStages(line_prog).Geometry().Fragment();

		Texture::Active(0);
		sketch_prog.sketch_tex.Set(0);
		{
			auto bound_tex = gl.Bound(Texture::Target::_3D, sketch_texture);

			for(GLuint i=0; i<sketch_tex_layers; ++i)
			{
				auto image = images::BrushedMetalUByte(
					512, 512,
					64 + i*128,
					-(2+i*4), +(2+i*4),
					64, 256-i*4
				);
				if(i == 0)
				{
					bound_tex.Image3D(
						0,
						PixelDataInternalFormat::RGB,
						image.Width(),
						image.Height(),
						sketch_tex_layers,
						0,
						image.Format(),
						image.Type(),
						nullptr
					);
				}
				bound_tex.SubImage3D(
					0,
					0, 0, i,
					image.Width(),
					image.Height(),
					1,
					image.Format(),
					image.Type(),
					image.RawData()
				);
			}
			bound_tex.GenerateMipmap();
			bound_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::Repeat);
			bound_tex.WrapT(TextureWrap::Repeat);
			bound_tex.WrapR(TextureWrap::ClampToEdge);
		}

		Texture::Active(1);
		sketch_prog.shadow_tex.Set(1);

		gl.Bound(Texture::Target::_2D, shadow_tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.CompareMode(TextureCompareMode::CompareRefToTexture)
			.Image2D(
				0,
				PixelDataInternalFormat::DepthComponent32,
				shadow_tex_side, shadow_tex_side,
				0,
				PixelDataFormat::DepthComponent,
				PixelDataType::Float,
				nullptr
			);

		gl.Bound(Framebuffer::Target::Draw, frame_shadow_fbo)
			.AttachTexture(
				FramebufferAttachment::Depth,
				shadow_tex,
				0
			);

		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);

		gl.DepthFunc(CompareFn::LEqual);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);

		gl.PolygonOffset(1.0, 1.0);
		gl.LineWidth(1.5);

	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
	}

	void RenderShadowMap(
		const Vec3f& light_position,
		const Mat4f& torus_matrix,
		const Mat4f& light_proj_matrix
	)
	{
		frame_shadow_fbo.Bind(Framebuffer::Target::Draw);

		gl.Viewport(shadow_tex_side, shadow_tex_side);
		gl.Clear().DepthBuffer();
		gl.CullFace(Face::Back);

		transf_prog.camera_matrix.Set(light_proj_matrix);
		transf_prog.camera_position.Set(light_position);

		// Render the torus' frame
		transf_prog.model_matrix.Set(torus_matrix);

		shadow_pp.Bind();

		gl.Enable(Capability::PolygonOffsetFill);
		torus.Draw();
		gl.Disable(Capability::PolygonOffsetFill);
	}

	void RenderImage(
		double time,
		const Mat4f& torus_matrix,
		const Mat4f& light_proj_matrix
	)
	{
		// this is going into the on-screen framebuffer
		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);

		gl.ClearColor(1.0f, 0.9f, 0.8f, 0.0f);
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();
		gl.CullFace(Face::Back);
		//
		transf_prog.light_proj_matrix.Set(light_proj_matrix);

		Mat4f perspective = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 60
		);

		// setup the camera
		Vec3f camera_target(0.0f, 0.8f, 0.0f);
		auto camera = CamMatrixf::Orbiting(
			camera_target,
			GLfloat(7.0 - SineWave(time / 14.0)*3.0),
			FullCircles(time / 26.0),
			Degrees(45 + SineWave(time / 17.0) * 40)
		);
		Vec3f camera_position = camera.Position();
		transf_prog.camera_matrix.Set(perspective*camera);
		transf_prog.camera_position.Set(camera_position);

		// render into the depth buffer
		shadow_pp.Bind();

		gl.Enable(Capability::PolygonOffsetFill);
		gl.ColorMask(false, false, false, false);

		transf_prog.model_matrix = ModelMatrixf();
		plane.Draw();

		transf_prog.model_matrix.Set(torus_matrix);
		torus.Draw();

		gl.ColorMask(true, true, true, true);
		gl.Disable(Capability::PolygonOffsetFill);

		gl.Enable(Capability::Blend);
		gl.Disable(Capability::Blend);

		// render into the color buffer
		sketch_pp.Bind();

		gl.Enable(Capability::Blend);

		transf_prog.model_matrix = ModelMatrixf();
		transf_prog.texture_matrix.Set(Mat2f(Vec2f(3.0, 0.0), Vec2f(0.0, 3.0)));
		plane.Draw();

		transf_prog.model_matrix.Set(torus_matrix);
		transf_prog.texture_matrix.Set(Mat2f(Vec2f(8.0, 0.0), Vec2f(0.0, 2.0)));
		torus.Draw([](GLuint phase) -> bool { return phase <  4; });
		transf_prog.texture_matrix.Set(Mat2f(Vec2f(0.0, 2.0), Vec2f(8.0, 0.0)));
		torus.Draw([](GLuint phase) -> bool { return phase >= 4; });

		// render the edges
		line_pp.Bind();

		transf_prog.model_matrix = ModelMatrixf();
		plane.DrawEdges();

		transf_prog.model_matrix.Set(torus_matrix);
		torus.DrawEdges();

		gl.Disable(Capability::Blend);
	}

	void Render(double time)
	{
		const Vec3f light_position(16.0, 10.0, 9.0);
		const Vec3f torus_center(0.0, 1.5, 0.0);

		const Mat4f torus_matrix =
			ModelMatrixf::Translation(torus_center) *
			ModelMatrixf::RotationA(
				Vec3f(1, 2, 1),
				FullCircles(time / 17.0)
			);

		const Mat4f light_proj_matrix =
			CamMatrixf::PerspectiveX(Degrees(10), 1.0, 1, 100) *
			CamMatrixf::LookingAt(light_position, torus_center);

		transf_prog.light_position.Set(light_position);

		RenderShadowMap(
			light_position,
			torus_matrix,
			light_proj_matrix
		);
		RenderImage(
			time,
			torus_matrix,
			light_proj_matrix
		);
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double ScreenshotTime(void) const
	{
		return 3.0;
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
	return std::unique_ptr<Example>(new SketchExample);
}

} // namespace oglplus
