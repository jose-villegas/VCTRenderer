/**
 *  @example oglplus/032_object_tracking.cpp
 *  @brief On-screen object tracking and 2D text rendering
 *
 *  @oglplus_screenshot{032_object_tracking}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_model{arrow_z}
 *  @oglplus_example_uses_model{suzanne}
 *  @oglplus_example_uses_font{FreeSans}
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_transform_feedback2}
 *  @oglplus_example_uses_cpp_feat{UNICODE_LITERALS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/vector.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/torus.hpp>
#include <oglplus/shapes/spiral_sphere.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/obj_mesh.hpp>

#include <oglplus/text/font2d.hpp>

#include <oglplus/opt/list_init.hpp>
#include <oglplus/opt/resources.hpp>

#include <list>
#include <cassert>
#include <sstream>
#include <iomanip>

#include "example.hpp"

namespace oglplus {

class Path
{
private:
	static std::vector<Vec3f> make_positions(void)
	{
		std::vector<Vec3f> pos_data;
		pos_data.reserve(17);

		pos_data.push_back(Vec3f( 10.0f,  0.0f, 70.0f));
		pos_data.push_back(Vec3f( 60.0f,  0.0f, 60.0f));
		pos_data.push_back(Vec3f( 95.0f,  0.0f,  0.0f));
		pos_data.push_back(Vec3f( 60.0f,  0.0f,-60.0f));
		pos_data.push_back(Vec3f( 10.0f,  0.0f,-70.0f));
		pos_data.push_back(Vec3f(-30.0f, 20.0f,-70.0f));
		pos_data.push_back(Vec3f(-70.0f, 40.0f,-70.0f));
		pos_data.push_back(Vec3f(-90.0f, 40.0f,-30.0f));
		pos_data.push_back(Vec3f(-30.0f, 40.0f, 10.0f));
		pos_data.push_back(Vec3f( 40.0f, 40.0f, 10.0f));
		pos_data.push_back(Vec3f( 90.0f,  0.0f,  0.0f));
		pos_data.push_back(Vec3f( 50.0f,-40.0f,-10.0f));
		pos_data.push_back(Vec3f(  0.0f,-40.0f,-10.0f));
		pos_data.push_back(Vec3f(-50.0f,-40.0f,-10.0f));
		pos_data.push_back(Vec3f(-90.0f,-40.0f, 30.0f));
		pos_data.push_back(Vec3f(-70.0f,-40.0f, 70.0f));
		pos_data.push_back(Vec3f(-30.0f,-20.0f, 70.0f));

		return pos_data;
	}
	CubicBezierLoop<Vec3f, double> path_pos;

	static std::vector<Vec3f> make_normals(void)
	{
		std::vector<Vec3f> nml_data;
		nml_data.reserve(17);

		nml_data.push_back(Vec3f(  0.0f,  1.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f, -1.0f));
		nml_data.push_back(Vec3f( -1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  2.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  1.0f));
		nml_data.push_back(Vec3f(  1.0f,  1.0f,  0.5f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  1.0f));
		nml_data.push_back(Vec3f(  1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f, -1.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  0.0f));
		nml_data.push_back(Vec3f(  1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f, -1.0f,  1.0f));
		nml_data.push_back(Vec3f(  0.0f,  0.0f,  1.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f,  1.0f));
		nml_data.push_back(Vec3f(  1.0f,  0.0f,  0.0f));
		nml_data.push_back(Vec3f(  0.0f,  1.0f, -1.0f));
		nml_data.push_back(Vec3f( -1.0f,  1.0f,  0.0f));

		for(auto i=nml_data.begin(),e=nml_data.end(); i!=e; ++i)
			*i = Normalized(*i);

		return nml_data;
	}
	CubicBezierLoop<Vec3f, double> path_nml;
public:
	Path(void)
	 : path_pos(make_positions(), 0.25)
	 , path_nml(make_normals(), 0.25)
	{ }

	Vec3f Position(double t) const
	{
		return path_pos.Position(t);
	}

	Vec3f Normal(double t) const
	{
		return path_nml.Position(t);
	}
};

class TrackProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Track"));

		VertexShader vs(ObjectDesc("Track"));
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"layout (std140) uniform ModelBlock {"
			"	mat4 ModelMatrices[64];"
			"};"

			"in vec4 Position;"
			"flat out int vertInstanceID;"

			"void main(void)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix * "
			"		CameraMatrix * "
			"		ModelMatrices[gl_InstanceID] *"
			"		Position;"
			"	vertInstanceID = gl_InstanceID;"
			"}"
		).Compile();

		GeometryShader gs(ObjectDesc("Track"));
		gs.Source(
			"#version 150\n"
			"layout(points) in;"
			"layout(points, max_vertices = 1) out;"

			"flat in int vertInstanceID[1];"

			"out vec4 xfbData;"

			"void main(void)"
			"{"
			"	xfbData = vec4("
			"		gl_in[0].gl_Position.xyz/"
			"		gl_in[0].gl_Position.w,"
			"		gl_in[0].gl_Position.z "
			"	);"
			"	bool visible = "
			"		(xfbData.x >-1.2) && "
			"		(xfbData.x < 1.0) && "
			"		(xfbData.y >-1.1) && "
			"		(xfbData.y < 1.1) && "
			"		(xfbData.w > 0.0);"
			"	if(visible)"
			"	{"
			"		xfbData.z+=float(vertInstanceID[0]);"
			"		EmitVertex();"
			"		EndPrimitive();"
			"	}"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(gs);
		prog.TransformFeedbackVarying("xfbData");
		prog.Link();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix, camera_matrix;
	UniformBlock model_block;

	TrackProgram(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_block(self(), "ModelBlock")
	{ }
};

class DrawProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Draw"));

		VertexShader vs(ObjectDesc("Draw"));
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"layout (std140) uniform ModelBlock {"
			"	mat4 ModelMatrices[64];"
			"};"

			"const vec3 LightPos = vec3(0.0, 0.0, 0.0);"

			"in vec4 Position;"
			"in vec3 Normal;"

			"out vec3 vertLightDir;"
			"out vec3 vertNormal;"
			"out vec3 vertColor;"

			"void main(void)"
			"{"
			"	mat4 ModelMatrix = ModelMatrices[gl_InstanceID];"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = normalize(LightPos - gl_Position.xyz);"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertColor = abs(normalize((ModelMatrix*Position).yxz));"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();

		FragmentShader fs(ObjectDesc("Draw"));
		fs.Source(
			"#version 150\n"

			"in vec3 vertLightDir;"
			"in vec3 vertNormal;"
			"in vec3 vertColor;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float d = max(dot(vertLightDir, vertNormal)+0.1,0.0);"
			"	fragColor = vertColor * (0.1 + d);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix, camera_matrix;
	UniformBlock model_block;

	DrawProgram(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_block(self(), "ModelBlock")
	{ }
};

class OverlayProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Overlay"));

		VertexShader vs(ObjectDesc("Overlay"));
		vs.Source(
			"#version 150\n"

			"uniform vec2 ScreenSize;"

			"in vec4 Position;"
			"in vec2 TexCoord;"

			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertTexCoord = ScreenSize*vec2(TexCoord.x,1.0-TexCoord.y);"
			"}"
		).Compile();

		FragmentShader fs(ObjectDesc("Overlay"));
		fs.Source(
			"#version 150\n"

			"uniform sampler2DRect Overlay;"

			"in vec2 vertTexCoord;"

			"out vec4 fragColor;"

			"void main(void)"
			"{"
			"	float fa = texture(Overlay, vertTexCoord).r;"
			"	float la = 0.0;"
			"	la += texture(Overlay, vertTexCoord+vec2(-1,-1)).r;"
			"	la += texture(Overlay, vertTexCoord+vec2( 0,-1)).r;"
			"	la += texture(Overlay, vertTexCoord+vec2( 1,-1)).r;"
			"	la += texture(Overlay, vertTexCoord+vec2(-1, 0)).r;"
			"	la += texture(Overlay, vertTexCoord+vec2( 1, 0)).r;"
			"	la += texture(Overlay, vertTexCoord+vec2(-1, 1)).r;"
			"	la += texture(Overlay, vertTexCoord+vec2( 0, 1)).r;"
			"	la += texture(Overlay, vertTexCoord+vec2( 1, 1)).r;"


			"	if(la+fa <= 0.0) discard;"
			"	else"
			"	{"
			"		la = min(la, 1.0);"
			"		vec3 FillColor = vec3(1.0, 1.0, 1.0);"
			"		vec3 LineColor = vec3(0.0, 0.0, 0.0);"
			"		fragColor = "
			"			vec4(LineColor*la, la)+"
			"			vec4(FillColor*fa, fa);"
			"	}"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Vec2f> screen_size;
	UniformSampler overlay;

	OverlayProgram(void)
	 : Program(make())
	 , screen_size(self(), "ScreenSize")
	 , overlay(self(), "Overlay")
	{ }
};


class HUDOverlay : public Texture
{
private:
	TextureUnitSelector tex_unit;
	std::vector<GLubyte> tex_buf;
	GLuint width, height;
public:
	HUDOverlay(TextureUnitSelector unit)
	 : tex_unit(unit)
	 , width(0)
	 , height(0)
	{
		Active(tex_unit);

		Texture::Target tex_tgt = Texture::Target::Rectangle;
		Bind(tex_tgt);


		Texture::MinFilter(tex_tgt, TextureMinFilter::Nearest);
		Texture::MagFilter(tex_tgt, TextureMagFilter::Nearest);
		Texture::WrapS(tex_tgt, TextureWrap::ClampToEdge);
		Texture::WrapT(tex_tgt, TextureWrap::ClampToEdge);
	}


	void Resize(GLuint new_width, GLuint new_height)
	{
		width = new_width;
		height = new_height;
		tex_buf.resize(width*height);

	}

	void BeginUpdate(void)
	{
		std::fill(tex_buf.begin(), tex_buf.end(), 0x00);
	}

	void RenderText(
		const text::Font2D& font,
		const text::Font2D::Layout& layout,
		int x,
		int y,
		GLuint size
	)
	{
		font.Render(size, layout, tex_buf.data(), width, height, x, y);
	}

	void RenderInfo(
		const text::Font2D& font,
		float ndc_x,
		float ndc_y,
		float ndc_z,
		float dist,
		const std::string& name,
		int inst_id
	)
	{
		float sc_x = (+ndc_x*0.5f+0.5f)*width;
		float sc_y = (-ndc_y*0.5f+0.5f)*height;

		std::string s = u8"⊗";

		auto lt = font.MakeLayout(text::UTF8ToCodePoints(s.c_str(), s.size()));
		int px = int(30*pow(ndc_z, 8.0f));

		RenderText(
			font,
			lt,
			GLint(sc_x-font.Width(px, lt)/2),
			GLint(sc_y-font.Height(px, lt)/2),
			px
		);

		std::stringstream ss;

		ss << name << " #" << inst_id;
		s = ss.str();

		RenderText(
			font,
			font.MakeLayout(text::UTF8ToCodePoints(s.c_str(), s.size())),
			GLint(sc_x+30-ndc_z*20),
			GLint(sc_y-40+ndc_z*20),
			GLuint(40-20*ndc_z)
		);

		ss.str(std::string());
		ss.clear();

		ss << std::setw(5) << std::setprecision(1) << std::fixed << dist << " [m]";
		s = ss.str();

		RenderText(
			font,
			font.MakeLayout(text::UTF8ToCodePoints(s.c_str(), s.size())),
			GLint(sc_x+30-ndc_z*20),
			GLint(sc_y+10-ndc_z*5),
			12
		);
	}

	void Finish(void)
	{
		Texture::Active(tex_unit);
		Texture::Target tex_tgt = Texture::Target::Rectangle;
		Bind(tex_tgt);
		Texture::Image2D(
			tex_tgt,
			0,
			PixelDataInternalFormat::Red,
			width, height,
			0,
			PixelDataFormat::Red,
			PixelDataType::UnsignedByte,
			tex_buf.data()
		);
	}
};

class ObjShape
 : public ResourceFile
 , public shapes::ObjMesh
{
public:
	ObjShape(const char* name)
	 : ResourceFile("models", name, ".obj")
	 , shapes::ObjMesh(stream(), LoadingOptions(false).Normals())
	{ }
};

class InstancedShape
{
private:
	static float nrand(void)
	{
		return 2.0f*(float(std::rand())/RAND_MAX - 0.5f);
	}

	static Buffer MakeModelMatrices(GLuint count)
	{
		std::vector<GLfloat> matrix_data(count*16);
		auto p = matrix_data.begin();

		for(GLuint i=0; i!=count; ++i)
		{
			float s = 120.0;
			Mat4f matrix = Transposed(
				ModelMatrixf::Translation(nrand()*s, nrand()*s, nrand()*s) *
				ModelMatrixf::RotationX(FullCircles(nrand()))*
				ModelMatrixf::RotationY(FullCircles(nrand()))*
				ModelMatrixf::RotationZ(FullCircles(nrand()))*
				ModelMatrixf::Scale(3.0f, 3.0f, 3.0f)
			);

			p = std::copy(
				Data(matrix),
				Data(matrix)+
				Size(matrix),
				p
			);
		}
		assert(p == matrix_data.end());

		Buffer matrix_buffer;
		matrix_buffer.Bind(Buffer::Target::Uniform);
		Buffer::Data(
			Buffer::Target::Uniform,
			matrix_data,
			BufferUsage::StaticDraw
		);
		return matrix_buffer;
	}

	std::string name;

	shapes::ShapeWrapper&point;
	shapes::ShapeWrapper shape;

	const GLuint instance_count;
	Buffer matrix_buffer;

	Buffer track_buffer;
	Query track_query;
	TransformFeedback track_xfb;
	DefaultTransformFeedback def_xfb;
public:
	InstancedShape(
		const std::string shape_name,
		shapes::ShapeWrapper& point_ref,
		shapes::ShapeWrapper&& wrapper,
		GLuint count
	): name(shape_name)
	 , point(point_ref)
	 , shape(std::move(wrapper))
	 , instance_count(count)
	 , matrix_buffer(MakeModelMatrices(instance_count))
	{
		track_buffer.Bind(Buffer::Target::CopyRead);
		Buffer::Resize(
			BufferTarget::CopyRead,
			BufferSize::Of<GLfloat>(instance_count*4),
			BufferUsage::DynamicCopy
		);

		track_xfb.Bind();
		track_buffer.Bind(Buffer::Target::TransformFeedback);
		track_buffer.BindBase(Buffer::IndexedTarget::TransformFeedback, 0);
		DefaultTransformFeedback().Bind();
	}


	InstancedShape(InstancedShape&& temp)
	 : name(std::move(temp.name))
	 , point(temp.point)
	 , shape(std::move(temp.shape))
	 , instance_count(temp.instance_count)
	 , matrix_buffer(std::move(temp.matrix_buffer))
	 , track_buffer(std::move(temp.track_buffer))
	 , track_query(std::move(temp.track_query))
	 , track_xfb(std::move(temp.track_xfb))
	{ }

	void StartTracking(GLuint binding_point)
	{
		matrix_buffer.BindBaseUniform(binding_point);

		track_xfb.Bind();

		Query::Activator qry_act = track_query.Activate(
			Query::Target::TransformFeedbackPrimitivesWritten
		);
		TransformFeedback::Activator xfb_act(TransformFeedbackPrimitiveType::Points);

		point.Draw(instance_count);

		xfb_act.Finish();
		qry_act.Finish();

		DefaultTransformFeedback().Bind();
	}

	void FinishTracking(const text::Font2D& font, HUDOverlay& hud_overlay)
	{
		GLuint count = 0;
		track_query.WaitForResult(count);
		track_buffer.Bind(Buffer::Target::TransformFeedback);
		BufferTypedMap<Vec4f> xfb_map(
			BufferTarget::TransformFeedback,
			BufferMapAccess::Read
		);

		for(GLuint i=0; i!=count; ++i)
		{
			Vec4f xfb_data = xfb_map.At(i);

			GLfloat ndc_x = xfb_data.x();
			GLfloat ndc_y = xfb_data.y();
			GLfloat ndc_z = xfb_data.z()-std::floor(xfb_data.z());
			int inst_id = int(xfb_data.z()-ndc_z);
			GLfloat dist = xfb_data.w();

			hud_overlay.RenderInfo(
				font,
				ndc_x,
				ndc_y,
				ndc_z,
				dist,
				name,
				inst_id
			);
		}
	}

	void Draw(GLuint binding_point)
	{
		shape.Use();
		matrix_buffer.BindBaseUniform(binding_point);
		shape.Draw(instance_count);
	}
};

class ObjectTrackingExample : public Example
{
private:
	Context gl;

	Path path;

	TrackProgram track_prog;
	DrawProgram draw_prog;
	OverlayProgram overlay_prog;

	shapes::ShapeWrapper point;
	std::list<InstancedShape> objects;

	shapes::ShapeWrapper screen;

	const GLuint overlay_tex_unit;
	HUDOverlay hud_overlay;

	template <class ShapeBuilder>
	void AddInstancedObject(
		const std::string& shape_name,
		const ShapeBuilder& shape_builder,
		GLuint count
	)
	{
		auto vert_attrs = List("Position")("Normal").Get();
		objects.push_back(
			InstancedShape(
				shape_name,
				point,
				shapes::ShapeWrapper(
					vert_attrs,
					shape_builder,
					draw_prog
				),
				count
			)
		);
	}

	text::Font2D font;
public:
	ObjectTrackingExample(void)
	 : gl()
	 , path()
	 , point(List("Position").Get(), shapes::PointAndVector(), track_prog)
	 , screen(List("Position")("TexCoord").Get(), shapes::Screen(), overlay_prog)
	 , overlay_tex_unit(0)
	 , hud_overlay(overlay_tex_unit)
	 , font(ResourceFile ("fonts", "FreeSans", ".ttf"))
	{
		track_prog.Use();
		track_prog.model_block.Binding(0);

		draw_prog.Use();
		draw_prog.model_block.Binding(0);

		AddInstancedObject("Arrow", ObjShape("arrow_z"), 16);
		AddInstancedObject("Monkey", ObjShape("suzanne"), 16);
		AddInstancedObject("Cube", shapes::Cube(), 16);
		AddInstancedObject("Torus", shapes::Torus(), 16);
		AddInstancedObject("Sphere", shapes::SpiralSphere(), 16);

		overlay_prog.Use();
		overlay_prog.overlay.Set(overlay_tex_unit);

		gl.ClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		gl.ClearDepth(1.0f);

		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		hud_overlay.Resize(width, height);

		auto projection =
			CamMatrixf::PerspectiveX(
				Degrees(75),
				float(width)/height,
				1, 1000
			);

		track_prog.Use();
		track_prog.projection_matrix.Set(projection);

		draw_prog.Use();
		draw_prog.projection_matrix.Set(projection);

		overlay_prog.Use();
		overlay_prog.screen_size.Set(width, height);
	}

	void Render(ExampleClock& clock)
	{
		double time = clock.Now().Seconds();

		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);

		gl.Enable(Capability::DepthTest);
		gl.Clear().ColorBuffer().DepthBuffer();

		double pos = time/60.0;

		auto camera =
			CamMatrixf::LookingAt(
				path.Position(pos-0.03+SineWave(time/7.0)*0.01),
				path.Position(pos+0.02+SineWave(time/11.0)*0.01),
				path.Normal(pos-0.02+SineWave(time/9.0)*0.02)
			);

		// -- Start tracking --

		track_prog.Use();
		track_prog.camera_matrix.Set(camera);

		point.Use();
		gl.Enable(Capability::RasterizerDiscard);
		for(auto i=objects.begin(); i!=objects.end(); ++i)
		{
			i->StartTracking(0);
		}
		gl.Disable(Capability::RasterizerDiscard);

		// -- Draw --

		draw_prog.Use();
		draw_prog.camera_matrix.Set(camera);

		for(auto i=objects.begin(); i!=objects.end(); ++i)
		{
			i->Draw(0);
		}

		// -- Finish tracking --
		hud_overlay.BeginUpdate();
		for(auto i=objects.begin(); i!=objects.end(); ++i)
		{
			i->FinishTracking(font, hud_overlay);
		}
		hud_overlay.Finish();

		// -- Draw overlay --
		overlay_prog.Use();

		gl.Disable(Capability::DepthTest);
		gl.Enable(Capability::Blend);
		screen.Use();
		screen.Draw();
		gl.Disable(Capability::Blend);
		gl.Enable(Capability::DepthTest);
	}

	bool Continue(double time)
	{
		return time < 90.0;
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
	return std::unique_ptr<Example>(new ObjectTrackingExample);
}

} // namespace oglplus
