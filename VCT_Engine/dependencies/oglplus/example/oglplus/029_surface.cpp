/**
 *  @example oglplus/029_surface.cpp
 *  @brief Shows implicit surface polygonization and single-pass wireframe
 *
 *  @oglplus_screenshot{029_surface}
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

#include <oglplus/shapes/tetrahedrons.hpp>

#include "example.hpp"

namespace oglplus {

class SurfaceVertShader
 : public VertexShader
{
public:
	SurfaceVertShader(void)
	 : VertexShader(
		ObjectDesc("Surface vertex shader"),
		"#version 150\n"
		"uniform vec3 GridOffset;"
		"uniform float Time;"

		"in vec4 Position;"

		"out vec3 vertNormal;"
		"out float vertValue;"
		"const vec4 Source[4] = vec4[4]("
		"	vec4(-0.2, 0.3, 0.1, 0.3),"
		"	vec4(-0.5, 3.1,-0.1, 0.1),"
		"	vec4( 0.2, 0.4,-0.4, 0.3),"
		"	vec4(-0.1, 0.2,-0.5, 0.7) "
		");"

		"void main(void)"
		"{"
		"	gl_Position = Position + vec4(GridOffset, 0.0);"
		"	vertValue = -gl_Position.y;"
		"	vertNormal = vec3(0.0, 1.0, 0.0);"
		"	for(int s=0; s!=4; ++s)"
		"	{"
		"		float x = gl_Position.x - Source[s].x;"
		"		float z = gl_Position.z - Source[s].z;"

		"		float a = Source[s].y;"
		"		float w = Source[s].w*20.0;"
		"		float r = 0.7*x*x + 0.4*z*z;"
		"		float t = r + s - Time*(0.5 - 0.1*s);"
		"		float g = w * t;"
		"		float d = 2.0*a*exp(-r-1.0)*(w*cos(g)-sin(g));"

		"		vertValue += sin(g)*exp(-r-1.0)*a;"
		"		vertNormal += vec3(x*d, 0.0, z*d);"
		"	}"
		"}"
	)
	{ }
};

class SurfaceGeomShader
 : public GeometryShader
{
public:
	SurfaceGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Surface geometry shader"),
		"#version 150\n"
		"layout(triangles_adjacency) in;"
		"layout(triangle_strip, max_vertices = 6) out;"

		"uniform mat4 CameraMatrix;"
		"uniform vec3 CameraPosition, LightPosition;"
		"uniform vec2 ViewportDimensions;"

		"in vec3 vertNormal[];"
		"in float vertValue[];"

		"noperspective out vec3 geomDist;"
		"out float geomSpecular, geomDiffuse, geomValue;"

		"void do_nothing(void){ }"

		"float find_t(int i1, int i2)"
		"{"
		"	return vertValue[i1]/(vertValue[i1] - vertValue[i2]);"
		"}"

		"vec4 make_position(int i1, int i2, float t)"
		"{"
		"	return mix(gl_in[i1].gl_Position,gl_in[i2].gl_Position,t);"
		"}"

		"void make_vertex("
		"	int i1, "
		"	int i2, "
		"	float t, "
		"	vec4 p0, "
		"	vec4 p1, "
		"	vec4 p2, "
		"	vec4 c0, "
		"	vec2 s0, "
		"	vec2 s1, "
		"	vec2 s2, "
		"	vec3 m"
		")"
		"{"
		"	vec3 Normal = normalize(mix(vertNormal[i1], vertNormal[i2], t));"
		"	vec3 LightDir = normalize(LightPosition - p0.xyz);"
		"	vec3 ViewDir = normalize(CameraPosition - p0.xyz);"

		"	float LightRefl = dot(reflect(-LightDir, Normal), ViewDir);"
		"	float LightHit = dot(Normal, LightDir);"

		"	gl_Position = c0;"
		"	geomSpecular = pow(clamp(LightRefl+0.1, 0.0, 1.0), 32);"
		"	geomDiffuse = pow(max((LightHit*0.7+0.3), 0.0), 2.0);"
		"	geomValue = pow(dot(normalize(p1-p0), normalize(p2-p0)),2.0);"

		"	vec2 v0 = s2 - s1;"
		"	vec2 v1 = s2 - s0;"
		"	vec2 v2 = s1 - s0;"
		"	float a = abs(v1.x*v2.y - v1.y*v2.x);"
		"	float d = a / length(v0);"
		"	geomDist = m * vec3(d, d, d);"
		"	EmitVertex();"
		"}"

		"void make_triangle(int a1, int a2, int b1, int b2, int c1, int c2)"
		"{"
		"	float ta = find_t(a1, a2);"
		"	float tb = find_t(b1, b2);"
		"	float tc = find_t(c1, c2);"

		"	vec4 pa = make_position(a1, a2, ta);"
		"	vec4 pb = make_position(b1, b2, tb);"
		"	vec4 pc = make_position(c1, c2, tc);"

		"	vec4 ca = CameraMatrix*pa;"
		"	vec4 cb = CameraMatrix*pb;"
		"	vec4 cc = CameraMatrix*pc;"

		"	vec2 sa = ViewportDimensions * ca.xy / ca.w;"
		"	vec2 sb = ViewportDimensions * cb.xy / cb.w;"
		"	vec2 sc = ViewportDimensions * cc.xy / cc.w;"

		"	make_vertex(a1, a2, ta, pa, pb, pc, ca, sa, sb, sc, vec3(1,0,0));"
		"	make_vertex(b1, b2, tb, pb, pa, pc, cb, sb, sa, sc, vec3(0,1,0));"
		"	make_vertex(c1, c2, tc, pc, pa, pb, cc, sc, sa, sb, vec3(0,0,1));"
		"	EndPrimitive();"
		"}"

		"void make_quad(int a1,int a2,int b1,int b2,int c1,int c2,int d1,int d2)"
		"{"
		"	float ta = find_t(a1, a2);"
		"	float tb = find_t(b1, b2);"
		"	float tc = find_t(c1, c2);"
		"	float td = find_t(d1, d2);"

		"	vec4 pa = make_position(a1, a2, ta);"
		"	vec4 pb = make_position(b1, b2, tb);"
		"	vec4 pc = make_position(c1, c2, tc);"
		"	vec4 pd = make_position(d1, d2, td);"

		"	vec4 ca = CameraMatrix*pa;"
		"	vec4 cb = CameraMatrix*pb;"
		"	vec4 cc = CameraMatrix*pc;"
		"	vec4 cd = CameraMatrix*pd;"

		"	vec2 sa = ViewportDimensions * ca.xy / ca.w;"
		"	vec2 sb = ViewportDimensions * cb.xy / cb.w;"
		"	vec2 sc = ViewportDimensions * cc.xy / cc.w;"
		"	vec2 sd = ViewportDimensions * cd.xy / cd.w;"

		"	make_vertex(a1, a2, ta, pa, pb, pc, ca, sa, sb, sc, vec3(1,0,0));"
		"	make_vertex(b1, b2, tb, pb, pa, pc, cb, sb, sa, sc, vec3(0,1,0));"
		"	make_vertex(c1, c2, tc, pc, pa, pb, cc, sc, sa, sb, vec3(0,0,1));"
		"	EndPrimitive();"

		"	make_vertex(c1, c2, tc, pc, pb, pd, cc, sc, sb, sd, vec3(1,0,0));"
		"	make_vertex(b1, b2, tb, pb, pc, pd, cb, sb, sc, sd, vec3(0,1,0));"
		"	make_vertex(d1, d2, td, pd, pc, pb, cd, sd, sc, sb, vec3(0,0,1));"
		"	EndPrimitive();"

		"}"

		"void process_tetrahedron(int a, int b, int c, int d)"
		"{"
		"	if(vertValue[a] >= 0.0)"
		"	{"
		"		if(vertValue[b] >= 0.0)"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					do_nothing();"
		"				else make_triangle(d,a, d,b, d,c);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(c,a, c,d, c,b);"
		"				else make_quad(c,a, d,a, c,b, d,b);"
		"			}"
		"		}"
		"		else"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(b,c, b,d, b,a);"
		"				else make_quad(b,c, d,c, b,a, d,a);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_quad(c,a, c,d, b,a, b,d);"
		"				else make_triangle(c,a, d,a, b,a);"
		"			}"
		"		}"
		"	}"
		"	else"
		"	{"
		"		if(vertValue[b] >= 0.0)"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(a,b, a,d, a,c);"
		"				else make_quad(a,c, a,b, d,c, d,b);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_quad(a,b, a,d, c,b, c,d);"
		"				else make_triangle(a,b, d,b, c,b);"
		"			}"
		"		}"
		"		else"
		"		{"
		"			if(vertValue[c] >= 0.0)"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_quad(b,c, b,d, a,c, a,d);"
		"				else make_triangle(b,c, d,c, a,c);"
		"			}"
		"			else"
		"			{"
		"				if(vertValue[d] >= 0.0)"
		"					make_triangle(a,d, c,d, b,d);"
		"				else do_nothing();"
		"			}"
		"		}"
		"	}"
		"}"

		"void main(void)"
		"{"
		"	process_tetrahedron(0, 2, 4, 1);"
		"}"
	)
	{ }
};

class SurfaceFragShader
 : public FragmentShader
{
public:
	SurfaceFragShader(void)
	 : FragmentShader(
		ObjectDesc("Surface fragment shader"),
		"#version 150\n"

		"noperspective in vec3 geomDist;"
		"in float geomSpecular, geomDiffuse, geomValue;"

		"out vec3 fragColor;"

		"void main(void)"
		"{"

		"	float MinDist = min(min(geomDist.x, geomDist.y), geomDist.z);"
		"	float EdgeAlpha = exp2(-pow(MinDist, 2.0));"

		"	float Ambient = 0.8;"

		"	vec3 Color = vec3("
		"		1.0-geomValue*(gl_FrontFacing?1:0),"
		"		1.0-geomValue*(gl_FrontFacing?0:1),"
		"		1.0-geomValue"
		"	);"
		"	vec3 Fill = Color * (Ambient + geomDiffuse + geomSpecular);"
		"	vec3 Line = vec3(0, 0, 0);"

		"	fragColor = mix(Fill, Line, EdgeAlpha);"
		"}"
	)
	{ }
};

class SurfaceProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(SurfaceVertShader());
		prog.AttachShader(SurfaceGeomShader());
		prog.AttachShader(SurfaceFragShader());
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix;
	ProgramUniform<Vec3f> grid_offset, camera_position, light_position;
	ProgramUniform<Vec2f> viewport_dimensions;
	ProgramUniform<GLfloat> time;

	SurfaceProgram(void)
	 : Program(make())
	 , camera_matrix(prog(), "CameraMatrix")
	 , grid_offset(prog(), "GridOffset")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	 , viewport_dimensions(prog(), "ViewportDimensions")
	 , time(prog(), "Time")
	{ }
};

class Grid
{
protected:
	shapes::Tetrahedrons make_grid;
	shapes::DrawingInstructions grid_instr;
	shapes::Tetrahedrons::IndexArray grid_indices;

	Context gl;

	// A vertex array object for the rendered shape
	VertexArray vao;

	// VBO for the grids's vertex positions
	Buffer positions, indices;

public:
	Grid(const Program& prog, float quality)
	 : make_grid(1.0, unsigned(8 + quality*quality*8))
	 , grid_instr(make_grid.Instructions(shapes::DrawMode::WithAdjacency()))
	 , grid_indices(make_grid.Indices(shapes::DrawMode::WithAdjacency()))
	{
		// bind the VAO for the shape
		vao.Bind();

		std::vector<GLfloat> data;
		// bind the VBO for the positions
		positions.Bind(Buffer::Target::Array);
		GLuint n_per_vertex = make_grid.Positions(data);
		// upload the data
		Buffer::Data(Buffer::Target::Array, data);
		// setup the vertex attribs array
		VertexArrayAttrib attr(prog, "Position");
		attr.Setup<GLfloat>(n_per_vertex);
		attr.Enable();

		// bind the VBO for the indices
		indices.Bind(Buffer::Target::ElementArray);
		// upload them
		Buffer::Data(Buffer::Target::ElementArray, grid_indices);
		// clear the indexs buffer
		grid_indices.clear();
	}

	void Use(void)
	{
		vao.Bind();
	}

	void Draw(void)
	{
		grid_instr.Draw(grid_indices);
	}
};

class SurfaceExample : public Example
{
private:

	// wrapper around the current OpenGL context
	Context gl;

	SurfaceProgram liquid_prog;

	Grid grid;

	Mat4f perspective;

	const int grid_repeat;
public:
	SurfaceExample(const ExampleParams& params)
	 : liquid_prog()
	 , grid(liquid_prog, params.quality)
	 , grid_repeat(int(1 + params.quality*2))
	{
		const Vec3f light_position(12.0f, 1.0f, 8.0f);
		liquid_prog.light_position.Set(light_position);

		gl.ClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		perspective = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 60
		);
		liquid_prog.viewport_dimensions.Set(Vec2f(width, height));
	}

	void Render(ExampleClock& clock)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		double time = clock.Now().Seconds();
		//
		liquid_prog.time.Set(GLfloat(time));

		double t = SineWave(time / 13.0);
		if(t+0.95 < 0.0) clock.Pace(0.05);
		else if(t+0.91 < 0.0) clock.Pace(0.10);
		else if(t+0.87 < 0.0) clock.Pace(0.15);
		else if(t+0.83 < 0.0) clock.Pace(0.20);
		else if(t+0.81 < 0.0) clock.Pace(0.50);
		else clock.Pace(1.0);


		auto camera = CamMatrixf::Orbiting(
			Vec3f(0, 0, 0),
			GLfloat(4.0 - SineWave(time / 14.0)),
			FullCircles(time / 26.0),
			Degrees(55 + SineWave(time / 14.0) * 30)
		);
		Vec3f camera_position = camera.Position();

		liquid_prog.camera_position.Set(camera_position);
		liquid_prog.camera_matrix.Set(perspective*camera);

		for(int y=-1; y!=1; ++y)
		for(int z=-grid_repeat; z!=grid_repeat; ++z)
		for(int x=-grid_repeat; x!=grid_repeat; ++x)
		{
			liquid_prog.grid_offset.Set(x, y, z);
			grid.Draw();
		}

	}

	bool Continue(double time)
	{
		return time < 60.0;
	}

	double ScreenshotTime(void) const
	{
		return 10.0;
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
	return std::unique_ptr<Example>(new SurfaceExample(params));
}

} // namespace oglplus
