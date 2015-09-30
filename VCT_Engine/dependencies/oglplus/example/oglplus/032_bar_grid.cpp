/**
 *  @example oglplus/032_bar_grid.cpp
 *  @brief Shows the usage of uniform blocks and stencil shadows
 *
 *  @oglplus_screenshot{032_bar_grid}
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
#include <oglplus/opt/list_init.hpp>

#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/plane.hpp>

#include <oglplus/images/brushed_metal.hpp>

#include <vector>

#include "example.hpp"

namespace oglplus {

class MetalFloor
 : public shapes::ShapeWrapper
{
private:
	Texture metal_tex;
public:
	MetalFloor(const Program& metal_prog)
	 : shapes::ShapeWrapper(
		List("Position")("Normal")("Tangent")("TexCoord").Get(),
		shapes::Plane(Vec3f(100, 0, 0), Vec3f(0, 0,-100)),
		metal_prog
	)
	{
		Texture::Active(0);
		UniformSampler(metal_prog, "MetalTex").Set(0);
		metal_tex
			<< TextureTarget::_2D
			<< TextureMinFilter::LinearMipmapLinear
			<< TextureMagFilter::Linear
			<< TextureWrap::Repeat
			<< images::BrushedMetalUByte(512, 512, 5120, -3, +3, 32, 128)
			<< TextureMipmap();
	}

	void Draw(void)
	{
		shapes::ShapeWrapper::Use();
		shapes::ShapeWrapper::Draw();
	}
};

class MetalProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(VertexShader(ObjectDesc("Metal vertex"),
			"#version 150\n"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec3 CameraPosition, LightPosition;"

			"in vec4 Position;"
			"in vec3 Normal, Tangent;"
			"in vec2 TexCoord;"

			"out vec3 vertNormal, vertTangent, vertBitangent;"
			"out vec3 vertLightDir, vertViewDir;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertViewDir = CameraPosition - gl_Position.xyz;"
			"	vertNormal = Normal;"
			"	vertTangent = Tangent;"
			"	vertBitangent = cross(vertNormal, vertTangent);"
			"	vertTexCoord = TexCoord * 50.0;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		));

		prog.AttachShader(FragmentShader(ObjectDesc("Metal fragment"),
			"#version 150\n"
			"const vec3 Color1 = vec3(0.5, 0.5, 0.6);"
			"const vec3 Color2 = vec3(0.7, 0.7, 0.8);"

			"uniform sampler2D MetalTex;"
			"uniform float LightMultiplier;"

			"in vec3 vertNormal, vertTangent, vertBitangent;"
			"in vec3 vertLightDir, vertViewDir;"
			"in vec2 vertTexCoord;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	bool odd = (int(vertTexCoord.x)+int(vertTexCoord.y)) % 2 == 0;"
			"	vec2 TexCoord = odd?vertTexCoord.yx:vertTexCoord.xy;"
			"	vec3 Sample = texture(MetalTex, TexCoord).rgb;"
			"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"

			"	vec3 Normal = normalize("
			"		2.0*vertNormal + "
			"		(Sample.r - 0.5)*vertTangent + "
			"		(Sample.g - 0.5)*vertBitangent"
			"	);"

			"	vec3 LightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		Normal"
			"	);"

			"	float Specular = LightMultiplier * pow(max(dot("
			"		normalize(LightRefl),"
			"		normalize(vertViewDir)"
			"	)+0.04, 0.0), 16+Sample.b*48)*pow(0.4+Sample.b*1.6, 4.0);"

			"	Normal = normalize(vertNormal*3.0 + Normal);"

			"	float Diffuse = LightMultiplier * pow(max(dot("
			"		normalize(Normal), "
			"		normalize(vertLightDir)"
			"	), 0.0), 2.0);"

			"	vec3 Color = mix(Color1, Color2, Sample.b)*(odd?0.95:1.05);"

			"	fragColor = "
			"		LightColor * Color * Diffuse + "
			"		LightColor * Specular;"
			"}"
		));

		prog.Link();
		return prog;
	}

	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix, projection_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniform<GLfloat> light_multiplier;

	MetalProgram(void)
	 : Program(make())
	 , camera_matrix(self(), "CameraMatrix")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_position(self(), "CameraPosition")
	 , light_position(self(), "LightPosition")
	 , light_multiplier(self(), "LightMultiplier")
	{ }
};


class BarGridBarData
{
private:
	Context gl;

	VertexArray draw_vao, shdw_vao;

	Buffer positions, normals, indices;
	const GLuint vertex_count, index_count;
public:
	BarGridBarData(
		const Program& draw_prog,
		const Program& shdw_prog,
		GLfloat size,
		GLfloat chamfer
	): vertex_count(8*2+4)
	 , index_count((9*2+1)+4*(4+1+3+1)+(4))
	{
		assert(size > 0.0f);
		assert(chamfer > 0.0f && chamfer < 0.5f);
		const GLfloat a = size * 0.5f;
		const GLfloat b = a * chamfer;
		const GLfloat z = 0.0f;
		const GLfloat o = 1.0f;

		std::vector<GLfloat> pos_data(4*vertex_count);
		std::vector<GLfloat> nml_data(3*vertex_count);
		auto pi = pos_data.begin(), pe = pos_data.end();
		auto ni = nml_data.begin(), ne = nml_data.end();

		/*   (10/11)   (8/9)
		 *      ^        ^
		 *      |        |
		 *      o--------o
		 *     /          \
		 * <--o (12/13)    o--> (6/7)
		 *    |            |
		 *    |            |
		 * <--o (14/15)    o--> (4/5)
		 *     \          /
		 *      o--------o
		 *      |        |
		 *      v        v
		 *    (0/1)    (2/3)
		 */

		//  0/1
		*pi++ =-a+b; *pi++ =   z; *pi++ =   a; *pi++ =   z;
		*ni++ =   z; *ni++ =   z; *ni++ =   o;
		*pi++ =-a+b; *pi++ =   b; *pi++ =   a; *pi++ =   o;
		*ni++ =   z; *ni++ =   z; *ni++ =   o;
		//  2/3
		*pi++ = a-b; *pi++ =   z; *pi++ =   a; *pi++ =   z;
		*ni++ =   z; *ni++ =   z; *ni++ =   o;
		*pi++ = a-b; *pi++ =   b; *pi++ =   a; *pi++ =   o;
		*ni++ =   z; *ni++ =   z; *ni++ =   o;
		//  4/5
		*pi++ =   a; *pi++ =   z; *pi++ = a-b; *pi++ =   z;
		*ni++ =   o; *ni++ =   z; *ni++ =   z;
		*pi++ =   a; *pi++ =   b; *pi++ = a-b; *pi++ =   o;
		*ni++ =   o; *ni++ =   z; *ni++ =   z;
		//  6/7
		*pi++ =   a; *pi++ =   z; *pi++ =-a+b; *pi++ =   z;
		*ni++ =   o; *ni++ =   z; *ni++ =   z;
		*pi++ =   a; *pi++ =   b; *pi++ =-a+b; *pi++ =   o;
		*ni++ =   o; *ni++ =   z; *ni++ =   z;
		//  8/9
		*pi++ = a-b; *pi++ =   z; *pi++ =  -a; *pi++ =   z;
		*ni++ =   z; *ni++ =   z; *ni++ =  -o;
		*pi++ = a-b; *pi++ =   b; *pi++ =  -a; *pi++ =   o;
		*ni++ =   z; *ni++ =   z; *ni++ =  -o;
		//  10/11
		*pi++ =-a+b; *pi++ =   z; *pi++ =  -a; *pi++ =   z;
		*ni++ =   z; *ni++ =   z; *ni++ =  -o;
		*pi++ =-a+b; *pi++ =   b; *pi++ =  -a; *pi++ =   o;
		*ni++ =   z; *ni++ =   z; *ni++ =  -o;
		//  12/13
		*pi++ =  -a; *pi++ =   z; *pi++ =-a+b; *pi++ =   z;
		*ni++ =  -o; *ni++ =   z; *ni++ =   z;
		*pi++ =  -a; *pi++ =   b; *pi++ =-a+b; *pi++ =   o;
		*ni++ =  -o; *ni++ =   z; *ni++ =   z;
		//  14/15
		*pi++ =  -a; *pi++ =   z; *pi++ = a-b; *pi++ =   z;
		*ni++ =  -o; *ni++ =   z; *ni++ =   z;
		*pi++ =  -a; *pi++ =   b; *pi++ = a-b; *pi++ =   o;
		*ni++ =  -o; *ni++ =   z; *ni++ =   z;

		/*
		 *       (11)     (9)
		 *        o--------o
		 *       /|(19)(18)|\
		 * (13) o-o--------o-o (7)
		 *      | |        | |
		 *      | |(16)(17)| |
		 * (15) o-o--------o-o (5)
		 *       \|        |/
		 *        o--------o
		 *       (1)      (3)
		 */

		//  16
		*pi++ =-a+b; *pi++ = b+b; *pi++ = a-b; *pi++ =   o;
		*ni++ =   z; *ni++ =   o; *ni++ =   z;
		//  17
		*pi++ = a-b; *pi++ = b+b; *pi++ = a-b; *pi++ =   o;
		*ni++ =   z; *ni++ =   o; *ni++ =   z;
		//  18
		*pi++ = a-b; *pi++ = b+b; *pi++ =-a+b; *pi++ =   o;
		*ni++ =   z; *ni++ =   o; *ni++ =   z;
		//  19
		*pi++ =-a+b; *pi++ = b+b; *pi++ =-a+b; *pi++ =   o;
		*ni++ =   z; *ni++ =   o; *ni++ =   z;

		assert(pi == pe);
		assert(ni == ne);

		std::vector<GLushort> idx_data(index_count);
		auto ii = idx_data.begin(), ie = idx_data.end();

		// primitive restart index
		GLshort pri = GLshort(vertex_count);
		// sides
		for(GLushort i=0; i!=18; ++i) *ii++ = i % 16; *ii++ = pri;
		// chamfer
		*ii++ =  1; *ii++ = 16; *ii++ =  3; *ii++ = 17; *ii++ = pri;
		*ii++ =  3; *ii++ = 17; *ii++ =  5;             *ii++ = pri;
		*ii++ =  5; *ii++ = 17; *ii++ =  7; *ii++ = 18; *ii++ = pri;
		*ii++ =  7; *ii++ = 18; *ii++ =  9;             *ii++ = pri;
		*ii++ =  9; *ii++ = 18; *ii++ = 11; *ii++ = 19; *ii++ = pri;
		*ii++ = 11; *ii++ = 19; *ii++ = 13;             *ii++ = pri;
		*ii++ = 13; *ii++ = 19; *ii++ = 15; *ii++ = 16; *ii++ = pri;
		*ii++ = 15; *ii++ = 16; *ii++ =  1;             *ii++ = pri;
		// top
		*ii++ = 16; *ii++ = 19; *ii++ = 17; *ii++ = 18;

		assert(ii == ie);

		gl.Bind(NoVertexArray());

		positions
			<< Buffer::Target::Array
			<< pos_data;

		normals	<< Buffer::Target::Array
			<< nml_data;

		indices	<< Buffer::Target::ElementArray
			<< idx_data;

		InitVAO(draw_prog, draw_vao);
		InitVAO(shdw_prog, shdw_vao);
	}

	void InitVAO(const Program& prog, const VertexArray& vao)
	{
		gl.Bind(vao);

		gl.Bind(Buffer::Target::Array, positions);
		VertexArrayAttrib(prog, "PosAndOffs")
			.Setup<Vec4f>()
			.Enable();

		gl.Bind(Buffer::Target::Array, normals);
		VertexArrayAttrib(prog, "Normal")
			.Setup<Vec3f>()
			.Enable();

		gl.Bind(Buffer::Target::ElementArray, indices);
	}

	void Render(GLsizei instance_count) const
	{
		gl.PrimitiveRestartIndex(vertex_count);
		gl.Enable(Capability::PrimitiveRestart);

		gl.FrontFace(FaceOrientation::CW);
		gl.DrawElementsInstanced(
			PrimitiveType::TriangleStrip,
			index_count,
			DataType::UnsignedShort,
			instance_count
		);

		gl.Disable(Capability::PrimitiveRestart);
	}

	void Draw(GLsizei instance_count) const
	{
		gl.Bind(draw_vao);
		Render(instance_count);
	}

	void Shadow(GLsizei instance_count) const
	{
		gl.Bind(shdw_vao);
		Render(instance_count);
	}
};

class BarGrid
{
private:
	Context gl;
	GLuint bar_count;
	BarGridBarData bar_data;

	std::vector<GLfloat> bar_pos_data;
	Buffer bar_positions;
public:
	BarGrid(
		const Program& draw_prog,
		const Program& shdw_prog,
		GLuint side,
		GLfloat size,
		GLfloat chamfer,
		GLfloat space
	): bar_count(side*side)
	 , bar_data(draw_prog, shdw_prog, size, chamfer)
	 , bar_pos_data(4 * bar_count)
	{
		assert(space > 0.0);

		const GLfloat h1 = 0.5f*(size*side + space*(side-1));
		const GLfloat h2 = 0.5f*size;

		auto pi = bar_pos_data.begin(), pe = bar_pos_data.end();

		for(GLuint z=0; z!=side; ++z)
		{
			const GLfloat zoffs = -h1 + h2 + z*(size+space);
			for(GLuint x=0; x!=side; ++x)
			{
				const GLfloat xoffs = -h1 + h2 + x*(size+space);
				assert(pi != pe); *pi++ = xoffs;
				*pi++ = 4.f*(GLfloat(std::rand()) / GLfloat(RAND_MAX));
				assert(pi != pe); *pi++ = zoffs;
				*pi++ = h1;
			}
		}
		assert(pi == pe);

		// upload the bar offsets
		gl.Bind(Buffer::Target::Uniform, bar_positions);
		Buffer::Data(Buffer::Target::Uniform, bar_pos_data);
		bar_positions.BindBaseUniform(0);
		UniformBlock(draw_prog, "BarOffsetBlock").Binding(0);
		UniformBlock(shdw_prog, "BarOffsetBlock").Binding(0);
	}

	void Update(float interval, const std::vector<GLuint>& triggered)
	{
		const GLfloat max = 5.0f;
		GLfloat falloff = interval * 0.5f;
		if(falloff > 0.0f) falloff = 1.0f - falloff ;
		for(std::size_t i=0, n=bar_pos_data.size(); i!=n; i+=4)
		{
			GLfloat incr = (GLfloat(std::rand()) / GLfloat(RAND_MAX));
			incr *= incr*incr;
			if(incr > 0.95f)
			{
				bar_pos_data[i+1] += interval*incr*10.0f;
				if(bar_pos_data[i+1] > max)
					bar_pos_data[i+1] = max;
			}
			bar_pos_data[i+1] *= falloff;
		}

		for(std::size_t i=0, n=triggered.size(); i!=n; ++i)
		{
			GLuint idx = 4*triggered[i]+1;
			assert(idx < bar_pos_data.size());
			bar_pos_data[idx] += 1.0f;
			if(bar_pos_data[idx] > max)
				bar_pos_data[idx] = max;
		}

		gl.Bind(Buffer::Target::Uniform, bar_positions);
		Buffer::Data(Buffer::Target::Uniform, bar_pos_data);
	}

	void Draw(void) const
	{
		bar_positions.BindBaseUniform(0);
		bar_data.Draw(bar_count);
	}

	void Shadow(void) const
	{
		bar_positions.BindBaseUniform(0);
		bar_data.Shadow(bar_count);
	}
};

class BarGridDrawProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(VertexShader(ObjectDesc("Draw vertex"),
			"#version 150\n"

			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec3 LightPosition, CameraPosition;"

			"layout(std140) uniform BarOffsetBlock {"
			"	vec4 BarOffsets[4096];"
			"};"

			"in vec4 PosAndOffs;"
			"in vec3 Normal;"

			"out vec4 vertColor;"
			"out vec3 vertNormal;"
			"out vec3 vertLightDir, vertViewDir;"

			"void main(void)"
			"{"
			"	gl_Position = vec4(PosAndOffs.xyz, 1.0);"
			"	vec4 Offs = BarOffsets[gl_InstanceID];"

			"	gl_Position.x += Offs.x;"
			"	gl_Position.y += PosAndOffs.w * Offs.y;"
			"	gl_Position.z += Offs.z;"

			"	vertColor = vec4("
			"		abs(Offs.yxz)/Offs.w,"
			"		PosAndOffs.w*sqrt(Offs.y)"
			"	);"

			"	vertNormal = Normal;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertViewDir = CameraPosition - gl_Position.xyz;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		));

		prog.AttachShader(FragmentShader(ObjectDesc("Draw fragment"),
			"#version 150\n"

			"uniform float LightMultiplier;"

			"in vec4 vertColor;"
			"in vec3 vertNormal;"
			"in vec3 vertLightDir, vertViewDir;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec3 Color = vertColor.rgb;"

			"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"
			"	vec3 LightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		normalize(vertNormal)"
			"	);"

			"	float Ambient = 0.2;"
			"	float Incandescent = 0.6 * vertColor.a;"

			"	float Diffuse = LightMultiplier * max(dot("
			"		normalize(vertNormal),"
			"		normalize(vertLightDir)"
			"	)+0.1, 0.0);"

			"	float Specular = pow(LightMultiplier,2.0)*pow(max(dot("
			"		normalize(LightRefl),"
			"		normalize(vertViewDir)"
			"	)-0.04, 0.0), 2.0);"

			"	fragColor = "
			"		Color * Incandescent+"
			"		LightColor * Color * (Ambient+Diffuse)+"
			"		LightColor * Specular;"
			"}"
		));

		prog.Link();
		return prog;
	}

	Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix, projection_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniform<GLfloat> light_multiplier;

	BarGridDrawProgram(void)
	 : Program(make())
	 , camera_matrix(self(), "CameraMatrix")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_position(self(), "CameraPosition")
	 , light_position(self(), "LightPosition")
	 , light_multiplier(self(), "LightMultiplier")
	{ }
};

class ShadowProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(VertexShader(ObjectDesc("Shadow vertex"),
			"#version 150\n"

			"uniform vec3 LightPosition;"

			"layout(std140) uniform BarOffsetBlock {"
			"	vec4 BarOffsets[4096];"
			"};"

			"in vec4 PosAndOffs;"
			"in vec3 Normal;"

			"out vec3 vertNormal, vertLightDir;"

			"void main(void)"
			"{"
			"	gl_Position = vec4(PosAndOffs.xyz, 1.0);"
			"	vec4 Offs = BarOffsets[gl_InstanceID];"

			"	gl_Position.x += Offs.x;"
			"	gl_Position.y += PosAndOffs.w * Offs.y;"
			"	gl_Position.z += Offs.z;"

			"	vertNormal = Normal;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"}"
		));

		prog.AttachShader(GeometryShader(ObjectDesc("Shadow geometry"),
			"#version 150\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 12) out;"

			"uniform mat4 CameraMatrix, ProjectionMatrix;"

			"in vec3 vertNormal[3], vertLightDir[3];"

			"void make_near_vertex(int index)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix*"
			"		CameraMatrix*"
			"		gl_in[index].gl_Position;"
			"	EmitVertex();"
			"}"

			"void make_far_vertex(int index)"
			"{"
			"	vec3 Pos = gl_in[index].gl_Position.xyz;"
			"	Pos -= vertLightDir[index];"
			"	gl_Position = "
			"		ProjectionMatrix*"
			"		CameraMatrix*"
			"		vec4(Pos, 1.0);"
			"	EmitVertex();"
			"}"

			"void make_plane(int a, int b)"
			"{"
			"	make_near_vertex(a);"
			"	make_near_vertex(b);"
			"	make_far_vertex(a);"
			"	make_far_vertex(b);"
			"	EndPrimitive();"
			"}"

			"void main(void)"
			"{"
			"	vec3 ld = ("
			"		vertLightDir[0]+"
			"		vertLightDir[1]+"
			"		vertLightDir[2] "
			"	);"

			"	vec3 fn = ("
			"		vertNormal[0]+"
			"		vertNormal[1]+"
			"		vertNormal[2] "
			"	);"

			"	if(dot(fn, ld) >= 0.0)"
			"	{"
			"		make_plane(1, 0);"
			"		make_plane(2, 1);"
			"		make_plane(0, 2);"
			"	}"
			"}"
		));

		prog.Link();
		return prog;
	}

	const Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix, projection_matrix;
	ProgramUniform<Vec3f> light_position;

	ShadowProgram(void)
	 : Program(make())
	 , camera_matrix(self(), "CameraMatrix")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , light_position(self(), "LightPosition")
	{ }
};


class BarGridExample : public Example
{
private:
	Context gl;

	MetalProgram metal_prog;
	MetalFloor floor;

	const GLuint side;
	ShadowProgram shadow_prog;
	BarGridDrawProgram draw_prog;
	BarGrid grid;

	std::vector<GLuint> triggered;
public:
	BarGridExample(void)
	 : gl()
	 , floor(metal_prog)
	 , side(16)
	 , grid(draw_prog, shadow_prog, side, 0.8f, 0.2f, 0.2f)
	{
		triggered.reserve(10);

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.ClearStencil(128);

		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		auto projection = CamMatrixf::PerspectiveX(
			Degrees(65),
			width, height,
			1.0, 100.0
		);
		metal_prog.projection_matrix.Set(projection);
		draw_prog.projection_matrix.Set(projection);
		shadow_prog.projection_matrix.Set(projection);
	}

	void Render(ExampleClock& clock)
	{
		double time = clock.Time();

		gl.Clear().ColorBuffer().DepthBuffer().StencilBuffer();

		Vec3f light_position(
			20.f+float(  SineWave(time / 37.f))*10.f,
			25.f+float(CosineWave(time / 37.f))*15.f,
			10.f
		);

		metal_prog.light_position = light_position;
		draw_prog.light_position = light_position;
		shadow_prog.light_position = light_position;

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			18.0,
			FullCircles(time / 17.0),
			Degrees(45 + SineWave(time / 21.0) * 40)
		);
		auto cam_pos = camera.Position();

		grid.Update(float(clock.Interval().Seconds()), triggered);
		triggered.clear();

		metal_prog.camera_matrix.Set(camera);
		metal_prog.camera_position.Set(cam_pos);
		draw_prog.camera_matrix.Set(camera);
		draw_prog.camera_position.Set(cam_pos);
		shadow_prog.camera_matrix.Set(camera);

		StencilOperation keep = StencilOperation::Keep;
		StencilOperation incr = StencilOperation::Incr;
		StencilOperation decr = StencilOperation::Decr;

		// Draw objects with low light and the depth
		metal_prog.light_multiplier.Set(0.4f);
		draw_prog.light_multiplier.Set(0.2f);

		gl.ColorMask(true, true, true, true);
		gl.DepthMask(true);
		gl.Disable(Capability::StencilTest);

		metal_prog.Use();
		floor.Draw();

		draw_prog.Use();
		grid.Draw();

		// Draw the shadow "volume" into the stencil buffer
		gl.ColorMask(false, false, false, false);
		gl.DepthMask(false);

		gl.Enable(Capability::StencilTest);
		gl.StencilFunc(CompareFunction::Always, 0);

		gl.StencilOpSeparate(Face::Front, keep, keep, incr);
		gl.StencilOpSeparate(Face::Back,  keep, keep, decr);

		gl.Disable(Capability::CullFace);

		shadow_prog.Use();
		grid.Shadow();

		gl.Enable(Capability::CullFace);

		// Draw stencilled parts of objects with full light
		metal_prog.light_multiplier.Set(1.0);
		draw_prog.light_multiplier.Set(1.0);

		gl.ColorMask(true, true, true, true);
		gl.DepthMask(true);

		gl.StencilFunc(CompareFunction::Equal, 128);
		gl.StencilOp(keep, keep, keep);

		gl.Clear().DepthBuffer();

		metal_prog.Use();
		floor.Draw();

		draw_prog.Use();
		grid.Draw();
	}

	void MouseMoveNormalized(float x, float y, float)
	{
		GLuint ix = GLuint(side*(0.5f * (1.0f + x)));
		GLuint iy = GLuint(side*(0.5f * (1.0f + y)));
		if(ix < side && iy < side)
		{
			triggered.push_back(iy*side + ix);
		}
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
	return std::unique_ptr<Example>(new BarGridExample);
}

} // namespace oglplus
