/**
 *  @example oglplus/041_blob_mt.cpp
 *  @brief Shows heavily multi-threaded implicit surface polygonization
 *  @note Work-in-progress
 *
 *  @oglplus_screenshot{041_blob_mt}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_shader_subroutine}
 *  @oglplus_example_uses_gl{GL_ARB_transform_feedback2}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *  @oglplus_example_uses_cpp_feat{THREADS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>
#include <oglplus/bound/renderbuffer.hpp>

#include <oglplus/shapes/tetrahedrons.hpp>
#include <oglplus/shapes/sky_box.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/opt/list_init.hpp>

#include <cmath>

#include "example.hpp"
#include "example_mt.hpp"

namespace oglplus {

class SurfaceExample;
class SurfacePolygonizer;
class SurfaceDepthMapRenderer;
class SurfacePolyXFBData;

// Stores polygonized surface data that can be used to draw
// the surface
// Part of the main example (running in the main thread)
class SurfaceDrawData
{
public:
	const GLuint planes, workers, capacity, segment_size;

	Array<Buffer> positions, normals;

	std::vector<std::vector<GLuint>> n_vertices;

	std::vector<ExampleFrameSyncQueue> plane_to_poly, plane_to_draw;

	CamMatrixf camera, light;
	Mat4f camera_projection, light_projection;
	Vec3f camera_position, light_position, ndc_light_pos;

	GLuint viewport_width, viewport_height;

	SurfaceDrawData(GLuint pln, GLuint wrk, GLuint cap);
};

// Stores objects necessary for transform feedback in a polygonizer
// Part of a polygonizer (running in a separate thread)
class SurfacePolyXFBData
{
private:
	friend class SurfacePolygonizer;

	const GLuint planes;

	Array<TransformFeedback> xfb;

	Array<Query> query;

	std::vector<Reference<Buffer>> positions, normals;

	std::vector<GLuint>& n_vertices;

	ExampleFrameSyncQueue &plane_to_poly, &plane_to_draw;
public:
	SurfacePolyXFBData(SurfaceDrawData& draw_data, GLuint id);
};

// Common objects used by polygonizers
// Part of the main example
class SurfacePolygonizerCommon
{
public:
	Context gl;

	Program prog;
	static Program MakeProg(void);

	const unsigned grid_div;
	shapes::ShapeWrapperWithAdjacency grid;
	Buffer grid_offsets;
	void InitGridOffsets();

	Texture configurations;
	void InitCellConfigs(SurfaceExample& parent);

	double metaball_time;
	std::vector<CubicBezierLoop<Vec4f, double> > metaball_paths;
	std::vector<GLfloat> metaball_curr;
	Texture metaballs;
	void InitMetaballs(SurfaceExample& parent);
	void UpdateMetaballs(SurfaceExample& parent, double time_diff);

	SurfacePolygonizerCommon(SurfaceExample&);
};


// Surface polygonizer thread
class SurfacePolygonizer : public ExampleThread
{
private:
	friend class SurfaceExample;

	Context gl;

	SurfacePolygonizerCommon& common;

	SurfacePolyXFBData xfb_data;

	VertexArray grid_vao;

	GLuint inst_count, base_inst;
public:
	SurfacePolygonizer(SurfaceExample& parent, GLuint id);
	~SurfacePolygonizer(void);

	void Cancel(void);

	void Polygonize(GLuint p);
	void Render(double time);
};

// Renders the surface depth map from the data generated
// by the polygonizers
class SurfaceDepthMapRenderer : public ExampleThread
{
private:
	Context gl;

	SurfaceDrawData& surface_data;

	Program& prog;
	Uniform<Mat4f> projection_matrix_0, projection_matrix_1;
	Uniform<Mat4f> view_matrix_0, view_matrix_1;

	Array<VertexArray> surface_vao;

	GLuint dmap_side;
	Framebuffer fbo;
	Texture dummy_map;

	ExampleFrameSyncQueue &depth_map_ready, &depth_map_needed;
public:
	SurfaceDepthMapRenderer(SurfaceExample& parent);

	void Cancel(void);
	void Render(double);
};

class SurfaceLightMapRenderer : public ExampleThread
{
private:
	Context gl;

	SurfaceDrawData& surface_data;

	Program& prog;
	Uniform<Mat4f> projection_matrix, camera_matrix;
	Uniform<Vec3f> ndc_light_pos;
	Uniform<Vec2f> screen_size;
	Uniform<GLfloat> scale;

	UniformSubroutines subroutines;
	SubroutineUniform get_intensity;
	Subroutine get_intensity_surface, get_intensity_background;


	Array<VertexArray> surface_vao;
	FaceOrientation surface_face_winding;

	shapes::ShapeWrapper sky_box;

	Framebuffer fbo;
	Renderbuffer rbo;

	GLuint width, height;

	ExampleFrameSyncQueue &light_map_ready, &light_map_needed;
public:
	SurfaceLightMapRenderer(SurfaceExample& parent);

	void Cancel(void);
	void Render(double);
};

class SurfaceColorMapRenderer : public ExampleThread
{
private:
	Context gl;

	SurfaceDrawData& surface_data;

	Program& prog;
	Uniform<Mat4f> projection_matrix, camera_matrix, light_matrix;
	Uniform<Vec3f> camera_position, light_position;

	Array<VertexArray> surface_vao;

	Framebuffer fbo;
	Renderbuffer rbo;

	GLuint width, height;

	ExampleFrameSyncQueue &color_map_ready, &color_map_needed;
public:
	SurfaceColorMapRenderer(SurfaceExample& parent);

	void Cancel(void);
	void Render(double);
};

class SurfaceExample : public Example
{
private:
	friend class SurfacePolygonizer;
	friend class SurfaceDepthMapRenderer;
	friend class SurfaceLightMapRenderer;
	friend class SurfaceColorMapRenderer;

	Context gl;

	// common resources used by the surface
	// polygonizer(s)
	SurfacePolygonizerCommon polygonizer_common;

	Texture depth_map, color_map, light_map;
	ExampleFrameSyncQueue depth_map_ready, depth_map_needed;
	ExampleFrameSyncQueue color_map_ready, color_map_needed;
	ExampleFrameSyncQueue light_map_ready, light_map_needed;

	Program depth_prog;
	static Program MakeDepthProg(void);

	Program color_prog;
	static Program MakeColorProg(void);

	Program light_ray_prog;
	static Program MakeLightMapProg(void);

	Program screen_prog;
	static Program MakeScreenProg(void);
	Uniform<Vec3f> ndc_light_pos;
	Uniform<Vec2f> screen_size;

	SurfaceDrawData surface_data;

	shapes::ShapeWrapper screen;
public:
	SurfaceExample(const ExampleParams& params);

	void Reshape(GLuint width, GLuint height);
	void Render(ExampleClock& clock);
	double ScreenshotTime(void) const { return 50.8; }

	bool Continue(double time) { return time < 120.0; }

	GLuint CellConfigTexUnit(void) const { return 0; }
	GLuint MetaballPosTexUnit(void) const { return 1; }
	GLuint DepthMapTexUnit(void) const { return 2; }
	GLuint DummyMapTexUnit(void) const { return 3; }
	GLuint ColorMapTexUnit(void) const { return 4; }
	GLuint LightMapTexUnit(void) const { return 5; }
};

SurfaceDrawData::SurfaceDrawData(GLuint pln, GLuint wrkr, GLuint cap)
 : planes(pln)
 , workers(wrkr)
 , capacity(cap)
 , segment_size(capacity*sizeof(GLfloat) / workers)
 , positions(planes)
 , normals(planes)
 , n_vertices(workers, std::vector<GLuint>(planes, 0))
 , plane_to_poly(workers)
 , plane_to_draw(workers)
{
	std::vector<GLfloat> init_data(capacity, 0.0f);

	for(GLuint p=0; p!=planes; ++p)
	{
		positions[p].Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, init_data, BufferUsage::DynamicCopy);

		normals[p].Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, init_data, BufferUsage::DynamicCopy);
	}

	for(GLuint w=0; w!=workers; ++w)
	{
		plane_to_poly[w].Push(0);
	}
}

SurfacePolyXFBData::SurfacePolyXFBData(SurfaceDrawData& draw_data, GLuint id)
 : planes(draw_data.planes)
 , xfb(planes)
 , query(planes)
 , n_vertices(draw_data.n_vertices[id])
 , plane_to_poly(draw_data.plane_to_poly[id])
 , plane_to_draw(draw_data.plane_to_draw[id])
{
	const GLuint size = draw_data.segment_size;
	const GLintptr offs = size*id;
	for(GLuint i=0; i!=planes; ++i)
	{
		positions.push_back(draw_data.positions[i]);
		normals.push_back(draw_data.normals[i]);

		xfb[i].Bind();

		positions[i].Bind(Buffer::Target::TransformFeedback);
		positions[i].BindRange(Buffer::IndexedTarget::TransformFeedback, 0, offs, size);

		normals[i].Bind(Buffer::Target::TransformFeedback);
		normals[i].BindRange(Buffer::IndexedTarget::TransformFeedback, 1, offs, size);
	}
	DefaultTransformFeedback().Bind();
}

Program SurfacePolygonizerCommon::MakeProg(void)
{
	Program prog;

	VertexShader vs(ObjectDesc("Polygonization vertex"));
	vs.Source(
		"#version 330\n"
		"uniform sampler1D Metaballs;"

		"in vec3 Position;"
		"in vec3 GridOffset;"

		"out vec3 vertPosition;"
		"out vec3 vertCenter;"
		"out float vertValue;"
		"flat out int vertInside;"

		"void main(void)"
		"{"
		"	vertPosition = Position + GridOffset;"

		"	float Sum = 0.0;"
		"	vertValue = 0.0;"
		"	vertCenter = vec3(0.0, 0.0, 0.0);"
		"	int Ball = 0, BallCount = textureSize(Metaballs, 0);"
		"	while(Ball != BallCount)"
		"	{"
		"		vec4 Metaball = texelFetch(Metaballs, Ball, 0);"
		"		vec3 Center = Metaball.xyz;"
		"		float Radius = Metaball.w;"
		"		vec3 Vect = vertPosition - Center;"
		"		float Tmp = (Radius*Radius)/dot(Vect, Vect);"
		"		vertValue += Tmp - 0.25;"
		"		float Mul = max(Tmp - 0.10, 0.0);"
		"		vertCenter += Mul * Center;"
		"		Sum += Mul;"
		"		++Ball;"
		"	}"
		"	if(Sum > 0.0) vertCenter = vertCenter / Sum;"
		"	vertInside = (vertValue >= 0.0)?1:0;"
		"}"
	).Compile();
	prog.AttachShader(vs);

	GeometryShader gs(ObjectDesc("Polygonization geometry"));
	gs.Source(
		"#version 330\n"
		"layout(triangles_adjacency) in;"
		"layout(triangle_strip, max_vertices = 4) out;"

		"uniform usampler1D Configurations;"

		"in vec3 vertPosition[6];"
		"in vec3 vertCenter[6];"
		"in float vertValue[6];"
		"flat in int vertInside[6];"

		"out vec3 xfbPosition;"
		"out vec3 xfbNormal;"

		"float find_t(const uint i1, const uint i2)"
		"{"
		"	float d = vertValue[i2] - vertValue[i1];"
		"	if(d == 0.0) return 0.5;"
		"	else return -vertValue[i1]/d;"
		"}"

		"void make_vertex(const uint i1, const uint i2)"
		"{"
		"	float t = find_t(i1, i2);"
		"	xfbPosition = mix(vertPosition[i1], vertPosition[i2], t);"
		"	vec3 Center = mix(vertCenter[i1], vertCenter[i2], t);"
		"	xfbNormal = xfbPosition - Center;"
		"	EmitVertex();"
		"}"

		"void make_triangle(const uvec4 v1, const uvec4 v2)"
		"{"
		"	make_vertex(v1.x, v2.x);"
		"	make_vertex(v1.y, v2.y);"
		"	make_vertex(v1.z, v2.z);"
		"	EndPrimitive();"
		"}"

		"void make_quad(const uvec4 v1, const uvec4 v2)"
		"{"
		"	make_vertex(v1.x, v2.x);"
		"	make_vertex(v1.y, v2.y);"
		"	make_vertex(v1.z, v2.z);"
		"	make_vertex(v1.w, v2.w);"
		"	EndPrimitive();"
		"}"

		"void process_tetrahedron(int a, int b, int c, int d)"
		"{"
		"	ivec4 i = ivec4("
		"		vertInside[a],"
		"		vertInside[b],"
		"		vertInside[c],"
		"		vertInside[d] "
		"	);"
		"	int si = int(dot(i, ivec4(1, 1, 1, 1))) % 4;"
		"	if(si != 0)"
		"	{"
		"		int iv = int(dot(i, ivec4(16, 8, 4, 2)));"
		"		uvec4 v1 = texelFetch(Configurations, iv+0, 0);"
		"		uvec4 v2 = texelFetch(Configurations, iv+1, 0);"
		"		if(si % 2 == 0) make_quad(v1, v2);"
		"		else make_triangle(v1, v2);"
		"	}"
		"}"

		"void main(void)"
		"{"
		"	process_tetrahedron(0, 2, 4, 1);"
		"}"
	).Compile();
	prog.AttachShader(gs);

	const GLchar* xfb_varyings[2] = {"xfbPosition", "xfbNormal"};
	prog.TransformFeedbackVaryings(
		xfb_varyings,
		TransformFeedbackMode::SeparateAttribs
	);

	prog.Link().Use();

	return std::move(prog);
}

void SurfacePolygonizerCommon::InitGridOffsets(void)
{
	int side = 1;
	int s = 2*side+1;
	int k = 0;
	std::vector<GLfloat> go_data(s*s*s*3);

	for(int z=-side; z!=side; ++z)
	for(int y=-side; y!=side; ++y)
	for(int x=-side; x!=side; ++x)
	{
		go_data[k++] = x;
		go_data[k++] = y;
		go_data[k++] = z;
	}

	grid_offsets.Bind(Buffer::Target::Array);
	Buffer::Data(Buffer::Target::Array, go_data);
}

void SurfacePolygonizerCommon::InitCellConfigs(SurfaceExample& parent)
{
	GLuint tex_unit = parent.CellConfigTexUnit();
	Texture::Active(tex_unit);
	UniformSampler(prog, "Configurations").Set(tex_unit);
	{
		const GLubyte a = 0x0, b = 0x2, c = 0x4, d = 0x1, x = 0xFF;
		const GLubyte  tex_data[] = {
			x, x, x, x,   x, x, x, x,
			a, c, b, x,   d, d, d, x,
			b, d, a, x,   c, c, c, x,
			b, b, a, a,   c, d, c, d,
			a, d, c, x,   b, b, b, x,
			a, a, c, c,   b, d, b, d,
			a, a, d, d,   c, b, c, b,
			a, a, a, x,   b, d, c, x,
			c, d, b, x,   a, a, a, x,
			c, c, b, b,   a, d, a, d,
			b, d, b, d,   c, c, a, a,
			b, b, b, x,   c, d, a, x,
			c, d, c, d,   a, a, b, b,
			c, c, c, x,   a, d, b, x,
			d, d, d, x,   a, b, c, x,
			x, x, x, x,   x, x, x, x
		};
		oglplus::Context::Bound(Texture::Target::_1D, configurations)
			.MinFilter(TextureMinFilter::Nearest)
			.MagFilter(TextureMagFilter::Nearest)
			.WrapS(TextureWrap::ClampToEdge)
			.Image1D(
				0,
				InternalFormat::RGBA8UI,
				sizeof(tex_data),
				0,
				Format::RGBAInteger,
				DataType::UnsignedByte,
				tex_data
			);
	}
}

void SurfacePolygonizerCommon::InitMetaballs(SurfaceExample& parent)
{
	metaball_time = 0.0;
	std::srand(4234);
	// initialize the metaball paths and sizes
	for(GLuint i=0; i!=24; ++i)
	{
		GLuint j = 0, n = 3+std::rand()%3;
		std::vector<Vec4f> points(n);
		GLfloat ball_size = 0.15f*std::rand()/GLfloat(RAND_MAX) + 0.2f;
		while(j != n)
		{
			points[j] = Vec4f(
				1.2f*std::rand()/GLfloat(RAND_MAX) - 0.6f,
				1.2f*std::rand()/GLfloat(RAND_MAX) - 0.6f,
				1.2f*std::rand()/GLfloat(RAND_MAX) - 0.6f,
				ball_size
			);
			++j;
		}
		metaball_paths.push_back(CubicBezierLoop<Vec4f, double>(points));
	}

	metaball_curr.resize(metaball_paths.size() * 4);

	Texture::Active(parent.MetaballPosTexUnit());
	UniformSampler(prog, "Metaballs").Set(parent.MetaballPosTexUnit());
	oglplus::Context::Bound(Texture::Target::_1D, metaballs)
		.MinFilter(TextureMinFilter::Nearest)
		.MagFilter(TextureMagFilter::Nearest)
		.WrapS(TextureWrap::ClampToEdge)
		.Image1D(
			0,
			InternalFormat::RGBA32F,
			metaball_paths.size(),
			0,
			Format::RGBA,
			DataType::Float,
			nullptr
		);

	UpdateMetaballs(parent, 0.0);
}

void SurfacePolygonizerCommon::UpdateMetaballs(SurfaceExample& parent, double time_diff)
{
	metaball_time += time_diff;
	auto path_i=metaball_paths.begin();
	auto path_e=metaball_paths.end();
	auto mb_i=metaball_curr.begin();

	while(path_i != path_e)
	{
		Vec4f pos = path_i->Position(metaball_time / 13.0);

		for(GLuint comp=0; comp != 4; ++comp)
		{
			assert(mb_i != metaball_curr.end());
			*mb_i++ = pos.At(comp);
		}
		++path_i;
	}
	assert(mb_i == metaball_curr.end());

	Texture::Active(parent.MetaballPosTexUnit());
	metaballs.Bind(Texture::Target::_1D);
	Texture::Image1D(
		Texture::Target::_1D,
		0,
		InternalFormat::RGBA32F,
		metaball_paths.size(),
		0,
		Format::RGBA,
		DataType::Float,
		metaball_curr.data()
	);
}

SurfacePolygonizerCommon::SurfacePolygonizerCommon(SurfaceExample& parent)
 : prog(MakeProg())
 , grid_div(32)
 , grid(List("Position").Get(), shapes::Tetrahedrons(1.0, grid_div))
{
	InitGridOffsets();
	InitCellConfigs(parent);
	InitMetaballs(parent);
}

SurfacePolygonizer::SurfacePolygonizer(SurfaceExample& parent, GLuint id)
 : gl()
 , common(parent.polygonizer_common)
 , xfb_data(parent.surface_data, id)
 , inst_count(8 / parent.surface_data.workers)
 , base_inst(inst_count * id)
{
	common.prog.Use();
	grid_vao = common.grid.VAOForProgram(common.prog);

	common.grid_offsets.Bind(Buffer::Target::Array);
	(common.prog|"GridOffset").Setup<Vec3f>().Enable().Divisor(1);

	Texture::Active(parent.CellConfigTexUnit());
	common.configurations.Bind(Texture::Target::_1D);

	Texture::Active(parent.MetaballPosTexUnit());
	common.metaballs.Bind(Texture::Target::_1D);

	gl.Enable(Capability::RasterizerDiscard);
}

SurfacePolygonizer::~SurfacePolygonizer(void)
{
	Cancel();
}

void SurfacePolygonizer::Cancel(void)
{
	xfb_data.plane_to_draw.Cancel();
	xfb_data.plane_to_poly.Cancel();
}

void SurfacePolygonizer::Polygonize(GLuint p)
{
	xfb_data.xfb[p].Bind();

	Query::Target query_target = Query::Target::TransformFeedbackPrimitivesWritten;
	Query::Activator qrya(xfb_data.query[p], query_target);

	TransformFeedback::Activator xfba(TransformFeedbackPrimitiveType::Triangles);

	common.grid.Draw(inst_count, base_inst);

	xfba.Finish();
	qrya.Finish();

	xfb_data.query[p].WaitForResult(xfb_data.n_vertices[p]);
}

void SurfacePolygonizer::Render(double)
{
	GLuint plane = xfb_data.plane_to_poly.Pop();

	Polygonize(plane);

	xfb_data.plane_to_draw.Push(plane);
}



SurfaceDepthMapRenderer::SurfaceDepthMapRenderer(SurfaceExample& parent)
 : gl()
 , surface_data(parent.surface_data)
 , prog(parent.depth_prog)
 , projection_matrix_0(prog, "ProjectionMatrix[0]")
 , projection_matrix_1(prog, "ProjectionMatrix[1]")
 , view_matrix_0(prog, "ViewMatrix[0]")
 , view_matrix_1(prog, "ViewMatrix[1]")
 , surface_vao(surface_data.planes)
 , dmap_side(1024)
 , depth_map_ready(parent.depth_map_ready)
 , depth_map_needed(parent.depth_map_needed)
{
	for(GLuint p=0; p!=surface_data.planes; ++p)
	{
		surface_vao[p].Bind();

		surface_data.positions[p].Bind(Buffer::Target::Array);
		(prog|"Position").Setup<Vec3f>().Enable();
	}

	Texture::Active(parent.DepthMapTexUnit());
	gl.Bound(Texture::Target::_2DArray, parent.depth_map)
		.MinFilter(TextureMinFilter::Linear)
		.MagFilter(TextureMagFilter::Linear)
		.WrapS(TextureWrap::ClampToEdge)
		.WrapT(TextureWrap::ClampToEdge)
		.WrapR(TextureWrap::ClampToEdge)
		.Image3D(
			0,
			InternalFormat::DepthComponent32,
			dmap_side, dmap_side, 2,
			0,
			Format::DepthComponent,
			DataType::Float,
			nullptr
		);

	Texture::Active(parent.DummyMapTexUnit());
	gl.Bound(Texture::Target::_2DArray, dummy_map)
		.MinFilter(TextureMinFilter::Nearest)
		.MagFilter(TextureMagFilter::Nearest)
		.WrapS(TextureWrap::ClampToEdge)
		.WrapT(TextureWrap::ClampToEdge)
		.WrapR(TextureWrap::ClampToEdge)
		.Image3D(
			0,
			InternalFormat::RGBA,
			dmap_side, dmap_side, 2,
			0,
			Format::RGBA,
			DataType::UnsignedByte,
			nullptr
		);

	gl.Bound(Framebuffer::Target::Draw, fbo)
		.AttachTexture(FramebufferAttachment::Depth, parent.depth_map, 0)
		.AttachTexture(FramebufferAttachment::Color, dummy_map, 0);

	gl.ClearDepth(1.0);
	gl.Enable(Capability::DepthTest);

	gl.Viewport(dmap_side, dmap_side);

	prog.Use();
	projection_matrix_1.Set(surface_data.light_projection);
}

void SurfaceDepthMapRenderer::Cancel(void)
{
	depth_map_needed.Cancel();
	depth_map_ready.Cancel();
}

void SurfaceDepthMapRenderer::Render(double)
{
	GLuint p = depth_map_needed.Pop();

	gl.Clear().DepthBuffer();

	projection_matrix_0.Set(surface_data.camera_projection);
	view_matrix_0.Set(surface_data.camera);
	view_matrix_1.Set(surface_data.light);

	surface_vao[p].Bind();
	for(GLuint w=0; w!=surface_data.workers; ++w)
	{
		gl.DrawArrays(
			PrimitiveType::Triangles,
			surface_data.segment_size*w,
			surface_data.n_vertices[w][p]*3
		);
	}

	depth_map_ready.Push(p);
}

SurfaceLightMapRenderer::SurfaceLightMapRenderer(SurfaceExample& parent)
 : gl()
 , surface_data(parent.surface_data)
 , prog(parent.light_ray_prog)
 , projection_matrix(prog, "ProjectionMatrix")
 , camera_matrix(prog, "CameraMatrix")
 , ndc_light_pos(prog, "NDCLightPos")
 , screen_size(prog, "ScreenSize")
 , scale(prog, "Scale")
 , subroutines(prog, ShaderType::Fragment)
 , get_intensity(prog, ShaderType::Fragment, "GetIntensity")
 , get_intensity_surface(prog, ShaderType::Fragment, "GetIntensitySurface")
 , get_intensity_background(prog, ShaderType::Fragment, "GetIntensityBackground")
 , surface_vao(surface_data.planes)
 , surface_face_winding(parent.polygonizer_common.grid.FaceWinding())
 , sky_box(List("Position").Get(), shapes::SkyBox(), prog)
 , width(0)
 , height(0)
 , light_map_ready(parent.light_map_ready)
 , light_map_needed(parent.light_map_needed)
{
	for(GLuint p=0; p!=surface_data.planes; ++p)
	{
		surface_vao[p].Bind();

		surface_data.positions[p].Bind(Buffer::Target::Array);
		(prog|"Position").Setup<Vec3f>().Enable();
	}

	Texture::Active(parent.LightMapTexUnit());
	gl.Bound(Texture::Target::Rectangle, parent.light_map)
		.MinFilter(TextureMinFilter::Linear)
		.MagFilter(TextureMagFilter::Linear)
		.WrapS(TextureWrap::ClampToEdge)
		.WrapT(TextureWrap::ClampToEdge);

	gl.Bind(Renderbuffer::Target::Renderbuffer, rbo);


	gl.Bound(Framebuffer::Target::Draw, fbo)
		.AttachTexture(FramebufferAttachment::Color, parent.light_map, 0)
		.AttachRenderbuffer(FramebufferAttachment::Depth, rbo);

	gl.Enable(Capability::CullFace);
	gl.CullFace(Face::Back);

	prog.Use();
}

void SurfaceLightMapRenderer::Cancel(void)
{
	light_map_ready.Cancel();
	light_map_needed.Cancel();
}

void SurfaceLightMapRenderer::Render(double)
{
	GLuint p = light_map_needed.Pop();

	if(width != surface_data.viewport_width || height != surface_data.viewport_height)
	{
		width = surface_data.viewport_width;
		height = surface_data.viewport_height;

		Texture::Image2D(
			Texture::Target::Rectangle,
			0,
			InternalFormat::Red,
			width/2, height/2,
			0,
			Format::Red,
			DataType::UnsignedByte,
			nullptr
		);
		Renderbuffer::Storage(
			Renderbuffer::Target::Renderbuffer,
			InternalFormat::DepthComponent,
			width/2,
			height/2
		);
		gl.Viewport(width/2, height/2);
		screen_size.Set(width/2, height/2);
		projection_matrix.Set(surface_data.camera_projection);
	}
	camera_matrix.Set(surface_data.camera);
	ndc_light_pos.Set(surface_data.ndc_light_pos);

	scale.Set(30.0);
	subroutines.Assign(get_intensity, get_intensity_background).Apply();

	sky_box.Use();
	sky_box.Draw();

	scale.Set(1.0);
	subroutines.Assign(get_intensity, get_intensity_surface).Apply();

	surface_vao[p].Bind();
	gl.FrontFace(surface_face_winding);
	for(GLuint w=0; w!=surface_data.workers; ++w)
	{
		gl.DrawArrays(
			PrimitiveType::Triangles,
			surface_data.segment_size*w,
			surface_data.n_vertices[w][p]*3
		);
	}

	light_map_ready.Push(p);
}

SurfaceColorMapRenderer::SurfaceColorMapRenderer(SurfaceExample& parent)
 : gl()
 , surface_data(parent.surface_data)
 , prog(parent.color_prog)
 , projection_matrix(prog, "ProjectionMatrix")
 , camera_matrix(prog, "CameraMatrix")
 , light_matrix(prog, "LightMatrix")
 , camera_position(prog, "CameraPosition")
 , light_position(prog, "LightPosition")
 , surface_vao(surface_data.planes)
 , width(0)
 , height(0)
 , color_map_ready(parent.color_map_ready)
 , color_map_needed(parent.color_map_needed)
{
	for(GLuint p=0; p!=surface_data.planes; ++p)
	{
		surface_vao[p].Bind();

		surface_data.positions[p].Bind(Buffer::Target::Array);
		(prog|"Position").Setup<Vec3f>().Enable();

		surface_data.normals[p].Bind(Buffer::Target::Array);
		(prog|"Normal").Setup<Vec3f>().Enable();
	}

	prog.Use();

	Texture::Active(parent.DepthMapTexUnit());
	parent.depth_map.Bind(Texture::Target::_2DArray);
	UniformSampler(prog, "DepthMap").Set(parent.DepthMapTexUnit());

	Uniform<Mat4f>(prog, "LightProjMatrix").Set(surface_data.light_projection);

	Texture::Active(parent.ColorMapTexUnit());
	gl.Bound(Texture::Target::Rectangle, parent.color_map)
		.MinFilter(TextureMinFilter::Nearest)
		.MagFilter(TextureMagFilter::Nearest)
		.WrapS(TextureWrap::ClampToEdge)
		.WrapT(TextureWrap::ClampToEdge);

	gl.Bind(Renderbuffer::Target::Renderbuffer, rbo);

	gl.Bound(Framebuffer::Target::Draw, fbo)
		.AttachTexture(FramebufferAttachment::Color, parent.color_map, 0)
		.AttachRenderbuffer(FramebufferAttachment::Depth, rbo);

	gl.ClearColor(0.35f, 0.30f, 0.25f, 0.0f);
	gl.ClearDepth(1.0);
	gl.Enable(Capability::DepthTest);

	gl.Enable(Capability::CullFace);
	gl.CullFace(Face::Back);
	gl.FrontFace(parent.polygonizer_common.grid.FaceWinding());
}

void SurfaceColorMapRenderer::Cancel(void)
{
	color_map_ready.Cancel();
	color_map_needed.Cancel();
}

void SurfaceColorMapRenderer::Render(double)
{
	GLuint p = color_map_needed.Pop();

	gl.Clear().ColorBuffer().DepthBuffer();

	if(width != surface_data.viewport_width || height != surface_data.viewport_height)
	{
		width = surface_data.viewport_width;
		height = surface_data.viewport_height;

		Texture::Image2D(
			Texture::Target::Rectangle,
			0,
			InternalFormat::RGB,
			width, height,
			0,
			Format::RGB,
			DataType::UnsignedByte,
			nullptr
		);
		Renderbuffer::Storage(
			Renderbuffer::Target::Renderbuffer,
			InternalFormat::DepthComponent,
			width,
			height
		);
		gl.Viewport(width, height);
		projection_matrix.Set(surface_data.camera_projection);
	}
	camera_matrix.Set(surface_data.camera);
	light_matrix.Set(surface_data.light);
	camera_position.Set(surface_data.camera_position);
	light_position.Set(surface_data.light_position);

	surface_vao[p].Bind();
	for(GLuint w=0; w!=surface_data.workers; ++w)
	{
		gl.DrawArrays(
			PrimitiveType::Triangles,
			surface_data.segment_size*w,
			surface_data.n_vertices[w][p]*3
		);
	}

	color_map_ready.Push(p);
}


Program SurfaceExample::MakeDepthProg(void)
{
	Program prog(ObjectDesc("Depth map"));

	VertexShader vs(ObjectDesc("Depth map vertex"));
	vs.Source(
		"#version 330\n"

		"in vec4 Position;"

		"void main(void)"
		"{"
		"	gl_Position = Position;"
		"}"
	).Compile();
	prog.AttachShader(vs);

	GeometryShader gs(ObjectDesc("Depth map geometry"));
	gs.Source(
		"#version 330\n"
		"#define NL 2\n"
		"layout(triangles) in;"
		"layout(triangle_strip, max_vertices=6) out;"

		"uniform mat4 ProjectionMatrix[NL];"
		"uniform mat4 ViewMatrix[NL];"

		"in vec4 vertPosition[];"

		"flat out int geomLayer;"

		"void main(void)"
		"{"
		"	for(int l=0; l!=NL; ++l)"
		"	{"
		"		gl_Layer = l;"
		"		geomLayer = l;"
		"		mat4 Matrix = ProjectionMatrix[l]*ViewMatrix[l];"
		"		for(int v=0; v!=3; ++v)"
		"		{"
		"			gl_Position = Matrix * gl_in[v].gl_Position;"
		"			EmitVertex();"
		"		}"
		"		EndPrimitive();"
		"	}"
		"}"
	).Compile();
	prog.AttachShader(gs);

	FragmentShader fs(ObjectDesc("Depth map fragment"));
	fs.Source(
		"#version 430\n"

		"flat in int geomLayer;"

		"void main(void)"
		"{"
		"	if((geomLayer == 1) == (gl_FrontFacing))"
		"		discard;"
		"}"
	).Compile();
	prog.AttachShader(fs);

	prog.Link().Use();

	return std::move(prog);
}

Program SurfaceExample::MakeColorProg(void)
{
	Program prog(ObjectDesc("Color"));

	VertexShader vs(ObjectDesc("Color vertex"));
	vs.Source(
		"#version 330\n"

		"uniform vec3 CameraPosition, LightPosition;"
		"uniform mat4 ProjectionMatrix, CameraMatrix;"
		"uniform mat4 LightProjMatrix, LightMatrix;"
		"mat4 Matrix = ProjectionMatrix * CameraMatrix;"
		"mat4 LtMat = LightProjMatrix * LightMatrix;"

		"in vec4 Position;"
		"in vec3 Normal;"

		"out vec4 vertPosition;"
		"out vec3 vertColor;"
		"out vec3 vertNormal;"
		"out vec3 vertViewDir;"
		"out vec3 vertLightDir;"
		"out vec4 vertShadowCoord;"

		"void main(void)"
		"{"
		"	vertColor = normalize(vec3(1,1,1)-Position.xyz);"
		"	vertNormal = Normal;"
		"	vertViewDir = CameraPosition - Position.xyz;"
		"	vertLightDir = LightPosition - Position.xyz;"
		"	vertShadowCoord = LtMat * Position;"
		"	vertPosition = Matrix * Position;"
		"	gl_Position = vertPosition;"
		"}"
	).Compile();
	prog.AttachShader(vs);

	FragmentShader fs(ObjectDesc("Color fragment"));
	fs.Source(
		"#version 330\n"

		"uniform mat4 CameraMatrix;"
		"uniform mat4 LightMatrix;"
		"uniform sampler2DArray DepthMap;"

		"in vec4 vertPosition;"
		"in vec3 vertColor;"
		"in vec3 vertNormal;"
		"in vec3 vertViewDir;"
		"in vec3 vertLightDir;"
		"in vec4 vertShadowCoord;"

		"out vec3 fragColor;"

		"void main(void)"
		"{"
		"	vec3 Normal = normalize(vertNormal);"
		"	vec3 ViewDir = normalize(vertViewDir);"
		"	vec3 LightDir = normalize(vertLightDir);"

		"	vec4 ShadowCoord = vec4(("
		"			vertShadowCoord.xyz/"
		"			vertShadowCoord.w"
		"		)*0.5+0.5,"
		"		1.0"
		"	);"

		"	vec4 DepthCoord = vec4(("
		"			vertPosition.xyz/"
		"			vertPosition.w"
		"		)*0.5+0.5,"
		"		0.0"
		"	);"

		"	float sd = texture(DepthMap, ShadowCoord.xyw).x;"
		"	float s = 1.0+clamp(pow((sd-ShadowCoord.z)*16.0, 3.0), -1.0, 0.0);"

		"	float bd = texture(DepthMap, DepthCoord.xyw).x;"
		"	if(bd >= 0.95) bd = DepthCoord.z;"
		"	float dd = pow(1.0-clamp((bd-DepthCoord.z)*5.0, 0.0, 1.0), 4.0);"

		"	float ln = max(dot(Normal, LightDir)+0.1, 0.0);"
		"	float vn = max(dot(Normal, ViewDir), 0.0);"
		"	float nn = 1.0-vn;"
		"	float ol = sqrt(max(-dot(ViewDir, LightDir)+0.9, 0.0));"

		"	float l = 0.1;"
		"	l += 0.1*dd;"
		"	l += 0.9*dd*s*nn*ol;"
		"	l += 0.9*pow(ln, 2.0)*s;"
		"	fragColor = vertColor*l;"
		"}"
	).Compile();
	prog.AttachShader(fs);

	prog.Link().Use();

	return std::move(prog);
}

Program SurfaceExample::MakeLightMapProg(void)
{
	Program prog(ObjectDesc("Light ray"));

	VertexShader vs(ObjectDesc("Light ray vertex"));
	vs.Source(
		"#version 400\n"

		"uniform mat4 ProjectionMatrix, CameraMatrix;"
		"uniform float Scale;"

		"in vec3 Position;"

		"void main(void)"
		"{"
		"	gl_Position = vec4(Position * Scale, 1.0);"
		"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
		"}"
	).Compile();
	prog.AttachShader(vs);

	FragmentShader fs(ObjectDesc("Light ray fragment"));
	fs.Source(
		"#version 400\n"

		"uniform vec3 NDCLightPos;"
		"uniform vec2 ScreenSize;"

		"subroutine float GetIntensityFunc(void);"

		"subroutine uniform GetIntensityFunc GetIntensity;"

		"subroutine(GetIntensityFunc)"
		"float GetIntensitySurface(void)"
		"{"
		"	return 0.0;"
		"}"

		"subroutine(GetIntensityFunc)"
		"float GetIntensityBackground(void)"
		"{"
		"	vec2 LPos = (NDCLightPos.xy*0.5+0.5)*ScreenSize;"
		"	vec2 d = gl_FragCoord.xy - LPos;"
		"	return clamp(1.0 / sqrt(length(d)+1), 0.0, 1.0);"
		"}"

		"out float fragIntensity;"

		"void main(void)"
		"{"
		"	fragIntensity = GetIntensity();"
		"}"
	).Compile();
	prog.AttachShader(fs);

	prog.Link().Use();

	return std::move(prog);
}


Program SurfaceExample::MakeScreenProg(void)
{
	Program prog(ObjectDesc("Screen"));

	VertexShader vs(ObjectDesc("Screen vertex"));
	vs.Source(
		"#version 330\n"

		"in vec4 Position;"
		"in vec2 TexCoord;"

		"out vec2 vertTexCoord;"

		"void main(void)"
		"{"
		"	vertTexCoord = TexCoord;"
		"	gl_Position = Position;"
		"}"
	).Compile();
	prog.AttachShader(vs);

	FragmentShader fs(ObjectDesc("Screen fragment"));
	fs.Source(
		"#version 330\n"

		"uniform sampler2DRect ColorMap, LightMap;"
		"uniform vec3 NDCLightPos;"
		"uniform vec2 ScreenSize;"

		"in vec2 vertTexCoord;"

		"out vec3 fragColor;"

		"void main(void)"
		"{"
		"	fragColor = texture(ColorMap, gl_FragCoord.xy).rgb;"
		"	if(NDCLightPos.z > 1.0) return;"
		"	vec2 LMCoord = gl_FragCoord.xy * 0.5;"
		"	vec2 LPos = (NDCLightPos.xy*0.5+0.5)*ScreenSize*0.5;"
		"	vec2 Ray = LMCoord - LPos;"
		"	float Len = length(Ray);"
		"	int NSampl = int(max(abs(Ray.x), abs(Ray.y)))+1;"
		"	vec2 RayStep = Ray / NSampl;"
		"	float Light = texture(LightMap, LMCoord).x;"
		"	NSampl = min(NSampl, int(min(ScreenSize.x, ScreenSize.y)*0.25));"
		"	for(int s=0; s!=NSampl;++s)"
		"	{"
		"		Light += texture(LightMap, LPos+RayStep*s).x;"
		"	}"
		"	Light /= NSampl;"
		"	Light = min(Light, 1.0);"
		"	fragColor = mix(fragColor, vec3(1,1,1), Light);"
		"}"
	).Compile();
	prog.AttachShader(fs);

	prog.Link().Use();

	return std::move(prog);
}

SurfaceExample::SurfaceExample(const ExampleParams& /*params*/)
 : gl()
 , polygonizer_common(*this)
 , depth_prog(MakeDepthProg())
 , color_prog(MakeColorProg())
 , light_ray_prog(MakeLightMapProg())
 , screen_prog(MakeScreenProg())
 , ndc_light_pos(screen_prog, "NDCLightPos")
 , screen_size(screen_prog, "ScreenSize")
 , surface_data(2, 1, 800000)
 , screen(List("Position").Get(), shapes::Screen(), screen_prog)
{
	surface_data.light_projection = CamMatrixf::Perspective(-1, 1,-1, 1, 18.0, 22.0);


	Texture::Active(ColorMapTexUnit());
	color_map.Bind(Texture::Target::Rectangle);
	UniformSampler(screen_prog, "ColorMap").Set(ColorMapTexUnit());

	Texture::Active(LightMapTexUnit());
	light_map.Bind(Texture::Target::Rectangle);
	UniformSampler(screen_prog, "LightMap").Set(LightMapTexUnit());

	screen.Use();
}

void SurfaceExample::Reshape(GLuint width, GLuint height)
{
	surface_data.viewport_width = width;
	surface_data.viewport_height = height;
	gl.Viewport(width, height);

	screen_size.Set(width, height);

	surface_data.camera_projection = CamMatrixf::PerspectiveX(
		Degrees(75),
		float(width)/height,
		1, 80
	);
}

void SurfaceExample::Render(ExampleClock& clock)
{
	double time = clock.Now().Seconds();

	surface_data.light = CamMatrixf::Orbiting(
		Vec3f(),
		20.0f,
		FullCircles(time / 90.0)-RightAngles(1),
		Degrees(-CosineWave(time / 37.0) * 10)
	);
	surface_data.light_position = surface_data.light.Position();

	surface_data.camera = CamMatrixf::Orbiting(
		Vec3f(),
		GLfloat(3.0 - SineWave(time / 14.0)),
		FullCircles(time / 60),
		Degrees(SineWave(time / 17.0) * 10)
	);
	surface_data.camera_position = surface_data.camera.Position();


	GLuint p = surface_data.plane_to_draw[0].Pop();
	for(GLuint w=1; w!=surface_data.workers; ++w)
	{
		GLuint np = surface_data.plane_to_draw[w].Pop();
		assert(np == p);
	}
	polygonizer_common.UpdateMetaballs(*this, clock.Interval().Seconds());

	if(surface_data.planes > 1)
	{
		GLuint np = (p+1)%surface_data.planes;
		for(GLuint w=0; w!=surface_data.workers; ++w)
		{
			surface_data.plane_to_poly[w].Push(np);
		}
	}

	depth_map_needed.Push(p);
	light_map_needed.Push(p);

	depth_map_ready.Pop();
	color_map_needed.Push(p);

	Vec4f tmp_light_pos((
		surface_data.camera_projection*
		surface_data.camera*
		Vec4f(surface_data.light_position, 1.0)
	));

	surface_data.ndc_light_pos = tmp_light_pos.xyz()/tmp_light_pos.w();
	ndc_light_pos.Set(surface_data.ndc_light_pos);

	color_map_ready.Pop();
	light_map_ready.Pop();

	screen.Draw();

	if(surface_data.planes <= 0)
	{
		for(GLuint w=0; w!=surface_data.workers; ++w)
		{
			surface_data.plane_to_poly[w].Push(0);
		}
	}
}

void setupExample(ExampleParams& params)
{
	if(params.max_threads>0)
	{
/*
		if(params.num_gpus>=8)
			params.num_threads = 8;
		else if(params.num_gpus>=4)
			params.num_threads = 4;
		else if(params.num_gpus>=2)
			params.num_threads = 2;
		else params.num_threads = 1;
*/
		params.num_threads = 1;

		params.num_threads++; // depth map renderer
		params.num_threads++; // color map renderer
		params.num_threads++; // ray map renderer
	}
	else throw std::runtime_error("This example requires multi-threading");
}

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& example,
	unsigned thread_id,
	const ExampleParams& params
)
{
	if(thread_id < params.num_threads - 3)
	{
		return std::unique_ptr<ExampleThread>(
			new SurfacePolygonizer(
				dynamic_cast<SurfaceExample&>(example),
				thread_id
			)
		);
	}
	else if(thread_id < params.num_threads - 2)
	{
		return std::unique_ptr<ExampleThread>(
			new SurfaceDepthMapRenderer(
				dynamic_cast<SurfaceExample&>(example)
			)
		);
	}
	else if(thread_id < params.num_threads - 1)
	{
		return std::unique_ptr<ExampleThread>(
			new SurfaceColorMapRenderer(
				dynamic_cast<SurfaceExample&>(example)
			)
		);
	}
	else if(thread_id < params.num_threads)
	{
		return std::unique_ptr<ExampleThread>(
			new SurfaceLightMapRenderer(
				dynamic_cast<SurfaceExample&>(example)
			)
		);
	}
	return std::unique_ptr<ExampleThread>();
}

std::unique_ptr<Example> makeExample(const ExampleParams& params)
{
	return std::unique_ptr<Example>(new SurfaceExample(params));
}

} // namespace oglplus
