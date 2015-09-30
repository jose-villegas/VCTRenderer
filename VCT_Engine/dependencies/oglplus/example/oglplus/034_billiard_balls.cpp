/**
 *  @example oglplus/034_billiard_balls.cpp
 *  @brief Shows rendering into multiple cube maps and simple shadow raytracing
 *
 *  @oglplus_screenshot{034_billiard_balls}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 *
 *  @oglplus_example_uses_cpp_feat{INITIALIZER_LISTS}
 *  @oglplus_example_uses_cpp_feat{VARIADIC_TEMPLATES}
 *
 *  @oglplus_example_uses_texture{pool_ball_1}
 *  @oglplus_example_uses_texture{pool_ball_2}
 *  @oglplus_example_uses_texture{pool_ball_3}
 *  @oglplus_example_uses_texture{pool_ball_4}
 *  @oglplus_example_uses_texture{pool_ball_5}
 *  @oglplus_example_uses_texture{pool_ball_6}
 *  @oglplus_example_uses_texture{pool_ball_7}
 *  @oglplus_example_uses_texture{pool_ball_8}
 *  @oglplus_example_uses_texture{pool_ball_9}
 *  @oglplus_example_uses_texture{pool_ball10}
 *  @oglplus_example_uses_texture{pool_ball11}
 *  @oglplus_example_uses_texture{pool_ball12}
 *  @oglplus_example_uses_texture{pool_ball13}
 *  @oglplus_example_uses_texture{pool_ball14}
 *  @oglplus_example_uses_texture{pool_ball15}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/sphere.hpp>

#include <oglplus/images/brushed_metal.hpp>
#include <oglplus/images/load.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>

#include <cmath>

#include "example.hpp"

// numeric count of rendered pool bals
#define OGLPLUS_EXAMPLE_034BB_BALL_COUNT 15
// textual representation of the count
#define OGLPLUS_EXAMPLE_034BB_BALL_COUNT_TXT "15"

namespace oglplus {

// the common vertex shader used in most of the rendering pipelines
class CommonVertShader
 : public VertexShader
{
public:
	CommonVertShader(void)
	 : VertexShader(
		ObjectDesc("Common vertex shader"),
		StrCRef("#version 150\n"
		"uniform mat4 ModelMatrix;"
		"uniform mat3 TextureMatrix;"
		"uniform vec3 CameraPosition, LightPosition;"
		"in vec4 Position;"
		"in vec3 Normal;"
		"in vec3 Tangent;"
		"in vec2 TexCoord;"
		"out gl_PerVertex {"
		"	vec4 gl_Position;"
		"};"
		"out vec3 vertNormal;"
		"out vec3 vertTangent;"
		"out vec3 vertBitangent;"
		"out vec3 vertLightDir;"
		"out vec3 vertViewDir;"
		"out vec2 vertTexCoord;"
		"out vec2 vertSTCoord;"
		"void main(void)"
		"{"
		"	gl_Position = ModelMatrix * Position;"
		"	vertLightDir = LightPosition - gl_Position.xyz;"
		"	vertViewDir = CameraPosition - gl_Position.xyz;"
		"	vertNormal =  (ModelMatrix * vec4(Normal,  0.0)).xyz;"
		"	vertTangent = (ModelMatrix * vec4(Tangent, 0.0)).xyz;"
		"	vertBitangent = cross(vertNormal, vertTangent);"
		"	vertTexCoord = (TextureMatrix * vec3(TexCoord,1.0)).xy;"
		"	vertSTCoord = TexCoord;"
		"}")
	)
	{ }
};

class DefaultGeomShader
 : public GeometryShader
{
public:
	DefaultGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Default geometry shader"),
		StrCRef("#version 150\n"
		"layout(triangles) in;"
		"layout(triangle_strip, max_vertices = 3) out;"
		"uniform mat4 CameraMatrix, ProjectionMatrix;"

		"in gl_PerVertex {"
		"	vec4 gl_Position;"
		"} gl_in[3];"
		"in vec3 vertNormal[3];"
		"in vec3 vertTangent[3];"
		"in vec3 vertBitangent[3];"
		"in vec3 vertLightDir[3];"
		"in vec3 vertViewDir[3];"
		"in vec2 vertTexCoord[3];"
		"in vec2 vertSTCoord[3];"

		"out gl_PerVertex {"
		"	vec4 gl_Position;"
		"};"
		"out vec3 geomNormal;"
		"out vec3 geomTangent;"
		"out vec3 geomBitangent;"
		"out vec3 geomLightDir;"
		"out vec3 geomViewDir;"
		"out vec2 geomTexCoord;"
		"out vec2 geomSTCoord;"

		"void main(void)"
		"{"
		"	for(int i=0; i!=3; ++i)"
		"	{"
		"		gl_Position = "
		"			ProjectionMatrix *"
		"			CameraMatrix *"
		"			gl_in[i].gl_Position;"
		"		geomNormal = vertNormal[i];"
		"		geomTangent = vertTangent[i];"
		"		geomBitangent = vertBitangent[i];"
		"		geomLightDir = vertLightDir[i];"
		"		geomViewDir = vertViewDir[i];"
		"		geomTexCoord = vertTexCoord[i];"
		"		geomSTCoord = vertSTCoord[i];"
		"		EmitVertex();"
		"	}"
		"	EndPrimitive();"
		"}")
	)
	{ }
};

class CubemapGeomShader
 : public GeometryShader
{
public:
	CubemapGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Cubemap geometry shader"),
		StrCRef("#version 150\n"
		"layout(triangles) in;"
		"layout(triangle_strip, max_vertices = 18) out;"
		"uniform mat4 ProjectionMatrix, CameraMatrix;"

		"in gl_PerVertex {"
		"	vec4 gl_Position;"
		"} gl_in[3];"
		"in vec3 vertNormal[3];"
		"in vec3 vertTangent[3];"
		"in vec3 vertBitangent[3];"
		"in vec3 vertLightDir[3];"
		"in vec3 vertViewDir[3];"
		"in vec2 vertTexCoord[3];"
		"in vec2 vertSTCoord[3];"

		"out gl_PerVertex {"
		"	vec4 gl_Position;"
		"};"
		"out vec3 geomNormal;"
		"out vec3 geomTangent;"
		"out vec3 geomBitangent;"
		"out vec3 geomLightDir;"
		"out vec3 geomViewDir;"
		"out vec2 geomTexCoord;"
		"out vec2 geomSTCoord;"

		"const mat4 CubeFaceMatrix[6] = mat4[6]("
		"	mat4("
		"		 0.0,  0.0, -1.0,  0.0,"
		"		 0.0, -1.0,  0.0,  0.0,"
		"		-1.0,  0.0,  0.0,  0.0,"
		"		 0.0,  0.0,  0.0,  1.0 "
		"	), mat4("
		"		 0.0,  0.0,  1.0,  0.0,"
		"		 0.0, -1.0,  0.0,  0.0,"
		"		 1.0,  0.0,  0.0,  0.0,"
		"		 0.0,  0.0,  0.0,  1.0 "
		"	), mat4("
		"		 1.0,  0.0,  0.0,  0.0,"
		"		 0.0,  0.0, -1.0,  0.0,"
		"		 0.0,  1.0,  0.0,  0.0,"
		"		 0.0,  0.0,  0.0,  1.0 "
		"	), mat4("
		"		 1.0,  0.0,  0.0,  0.0,"
		"		 0.0,  0.0,  1.0,  0.0,"
		"		 0.0, -1.0,  0.0,  0.0,"
		"		 0.0,  0.0,  0.0,  1.0 "
		"	), mat4("
		"		 1.0,  0.0,  0.0,  0.0,"
		"		 0.0, -1.0,  0.0,  0.0,"
		"		 0.0,  0.0, -1.0,  0.0,"
		"		 0.0,  0.0,  0.0,  1.0 "
		"	), mat4("
		"		-1.0,  0.0,  0.0,  0.0,"
		"		 0.0, -1.0,  0.0,  0.0,"
		"		 0.0,  0.0,  1.0,  0.0,"
		"		 0.0,  0.0,  0.0,  1.0 "
		"	)"
		");"

		"void main(void)"
		"{"
		"	for(gl_Layer=0; gl_Layer!=6; ++gl_Layer)"
		"	{"
		"		mat4 TransformMatrix = "
		"			ProjectionMatrix *"
		"			CubeFaceMatrix[gl_Layer] *"
		"			CameraMatrix;"
		"		for(int i=0; i!=3; ++i)"
		"		{"
		"			gl_Position = "
		"				TransformMatrix *"
		"				gl_in[i].gl_Position;"
		"			geomNormal = vertNormal[i];"
		"			geomTangent = vertTangent[i];"
		"			geomBitangent = vertBitangent[i];"
		"			geomLightDir = vertLightDir[i];"
		"			geomViewDir = vertViewDir[i];"
		"			geomTexCoord = vertTexCoord[i];"
		"			geomSTCoord = vertSTCoord[i];"
		"			EmitVertex();"
		"		}"
		"		EndPrimitive();"
		"	}"
		"}")
	)
	{ }
};

class VertexProgram
 : public QuickProgram
{
private:
	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Mat4f> model_matrix;
	ProgramUniform<Mat3f> texture_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;

	VertexProgram(void)
	 : QuickProgram(ObjectDesc("Vertex"), true, CommonVertShader())
	 , model_matrix(self(), "ModelMatrix")
	 , texture_matrix(self(), "TextureMatrix")
	 , camera_position(self(), "CameraPosition")
	 , light_position(self(), "LightPosition")
	{ }
};

class GeometryProgram
 : public QuickProgram
{
private:
	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;

	GeometryProgram(const GeometryShader& shader)
	 : QuickProgram(ObjectDesc("Geometry"), true, shader)
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	{ }
};

class ClothFragmentShader
 : public FragmentShader
{
public:
	ClothFragmentShader(void)
	 : FragmentShader(
		ObjectDesc("Cloth fragment shader"),
		StrCRef("#version 150\n"
		"uniform vec3 Color1, Color2;"
		"uniform sampler2D ClothTex, LightMap;"
		"in vec3 geomNormal;"
		"in vec3 geomTangent;"
		"in vec3 geomBitangent;"
		"in vec3 geomLightDir;"
		"in vec3 geomViewDir;"
		"in vec2 geomTexCoord;"
		"in vec2 geomSTCoord;"
		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	vec3 LightColor = texture(LightMap, geomSTCoord).rgb;"
		"	vec3 Sample = 0.5*("
		"		0.5*texture(ClothTex, geomTexCoord*0.5).rgb+"
		"		1.0*texture(ClothTex, geomTexCoord*1.0).rgb+"
		"		0.5*texture(ClothTex, geomTexCoord*2.0).rgb "
		"	);"

		"	vec3 Normal = normalize("
		"		2.0*geomNormal + "
		"		(Sample.r - 0.5)*geomTangent + "
		"		(Sample.g - 0.5)*geomBitangent"
		"	);"
		"	vec3 LightDir = normalize(geomLightDir);"
		"	vec3 LightRefl = reflect(-LightDir, Normal);"

		"	float Amount = pow(Sample.b, 2.0);"

		"	float Specular = pow(clamp(dot("
		"		LightRefl,"
		"		normalize(geomViewDir)"
		"	)+0.1, 0.0, 1.0)*0.9, 16.0+16.0*Amount)*(0.2+2.0*Amount);"

		"	float Diffuse = clamp(2.0*(dot("
		"		Normal, "
		"		LightDir"
		"	)-0.5), 0.0, 1.0);"

		"	float Ambient = 0.2;"

		"	vec3 Color = mix(Color1, Color2, Amount);"

		"	fragColor = vec4("
		"		Color * Ambient +"
		"		LightColor * Color * Diffuse + "
		"		LightColor * Specular, "
		"		1.0"
		"	);"
		"}")
	)
	{ }
};

class ClothProgram
 : public QuickProgram
{
private:
	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Vec3f> color_1, color_2;
	ProgramUniformSampler cloth_tex;
	ProgramUniformSampler light_map;

	ClothProgram(void)
	 : QuickProgram(ObjectDesc("Cloth"), true, ClothFragmentShader())
	 , color_1(self(), "Color1")
	 , color_2(self(), "Color2")
	 , cloth_tex(self(), "ClothTex")
	 , light_map(self(), "LightMap")
	{ }
};

class BallFragmentShader
 : public FragmentShader
{
public:
	BallFragmentShader(void)
	 : FragmentShader(
		ObjectDesc("Ball fragment shader"),
		StrCRef("#version 150\n"
		"uniform vec3 Color1, Color2;"
		"uniform sampler2DArray NumberTex;"
		"uniform samplerCube ReflectTex;"
		"uniform int BallIdx;"
		"in vec3 geomNormal;"
		"in vec3 geomTangent;"
		"in vec3 geomBitangent;"
		"in vec3 geomLightDir;"
		"in vec3 geomViewDir;"
		"in vec2 geomTexCoord;"
		"in vec2 geomSTCoord;"
		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	vec3 TexCoord = vec3(geomTexCoord, float(BallIdx));"
		"	vec4 Sample = texture(NumberTex, TexCoord);"
		"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"

		"	vec3 Normal = normalize(geomNormal);"
		"	vec3 LightDir = normalize(geomLightDir);"
		"	vec3 ViewDir = normalize(geomViewDir);"
		"	vec3 LightRefl = reflect(-LightDir, Normal);"
		"	vec3 ViewRefl = reflect(-ViewDir, Normal);"

		"	vec3 ReflSample = texture(ReflectTex, ViewRefl).rgb;"

		"	float Specular = pow(max(dot("
		"		LightRefl,"
		"		normalize(geomViewDir)"
		"	)+0.1, 0.0), 64.0)*(0.5 - Sample.a*(1.0-Sample.r)*0.4);"

		"	float Diffuse = max(dot(Normal, LightDir)+0.1, 0.0);"

		"	const float Reflectivity = 0.2;"
		"	const float Ambient = 0.2;"

		"	float ColorSwitch = ("
		"		geomSTCoord.t < 0.25 ||"
		"		geomSTCoord.t > 0.75"
		"	)? 0.0 : 1.0;"
		"	vec3 Color = mix("
		"		mix(Color1, Color2, ColorSwitch),"
		"		vec3(1.0, 1.0, 0.9)*Sample.r,"
		"		Sample.a"
		"	);"

		"	fragColor = vec4("
		"		ReflSample * Reflectivity +"
		"		Color * Ambient +"
		"		LightColor * Color * Diffuse +"
		"		LightColor * Specular, "
		"		1.0"
		"	);"
		"}")
	)
	{ }
};

class BallProgram
 : public QuickProgram
{
private:
	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Vec3f> color_1, color_2;
	ProgramUniformSampler number_tex, reflect_tex;
	ProgramUniform<GLint> ball_idx;

	BallProgram(void)
	 : QuickProgram(ObjectDesc("Ball"), true, BallFragmentShader())
	 , color_1(self(), "Color1")
	 , color_2(self(), "Color2")
	 , number_tex(self(), "NumberTex")
	 , reflect_tex(self(), "ReflectTex")
	 , ball_idx(self(), "BallIdx")
	{ }
};


class LightmapVertShader
 : public VertexShader
{
public:
	LightmapVertShader(void)
	 : VertexShader(
		ObjectDesc("Lightmap vertex shader"),
		StrCRef("#version 150\n"
		"uniform mat4 TransformMatrix;"
		"in vec4 Position;"
		"out vec3 vertPosition;"
		"void main(void)"
		"{"
		"	vertPosition = Position.xyz;"
		"	gl_Position = TransformMatrix * Position;"
		"}")
	)
	{ }
};

class LightmapFragShader
 : public FragmentShader
{
public:
	LightmapFragShader(void)
	 : FragmentShader(
		ObjectDesc("Lightmap fragment shader"),
		StrCRef("#version 150\n"
		"uniform vec3 LightPosition;"
		"uniform vec3 BallPositions[" OGLPLUS_EXAMPLE_034BB_BALL_COUNT_TXT "];"
		"in vec3 vertPosition;"
		"out vec4 fragColor;"
		"void main(void)"
		"{"
		"	float d_max = 0.0;"
		"	vec3 L = LightPosition;"
		"	vec3 R = normalize(vertPosition - LightPosition);"
		"	for(int i=0; i!=" OGLPLUS_EXAMPLE_034BB_BALL_COUNT_TXT  "; ++i)"
		"	{"
		"		vec3 S = BallPositions[i];"
		"		float a = dot(R, R);"
		"		float b = 2*dot(L-S, R);"
		"		float c = dot(L-S, L-S)-1.0;"
		"		float d = b*b - 4*a*c;"
		"		if(d_max < d) d_max = d;"
		"	}"
		"	float lt = exp(-d_max*0.8);"
		"	fragColor = vec4(lt, lt, lt, 1.0);"
		"}")
	)
	{ }

};

class LightmapProgram
 : public QuickProgram
{
private:
	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Mat4f> transform_matrix;
	ProgramUniform<Vec3f> light_position;
	ProgramUniform<Vec3f> ball_positions;

	LightmapProgram(void)
	 : QuickProgram(ObjectDesc("Lightmap"), (LightmapVertShader(), LightmapFragShader()))
	 , transform_matrix(self(), "TransformMatrix")
	 , light_position(self(), "LightPosition")
	 , ball_positions(self(), "BallPositions")
	{ }
};

template <typename ShapeBuilder>
class Shape
{
protected:
	// helper object building shape vertex attributes
	ShapeBuilder make_shape;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions shape_instr;
	// indices pointing to shape primitive elements
	typename ShapeBuilder::IndexArray shape_indices;

	Context gl;

	// A vertex array object for the rendered shape
	VertexArray vao;

	// number of vertex attributes
	const GLuint nva;

	// VBOs for the shape's vertex attributes
	Array<Buffer> vbos;

public:
	Shape(const Program& prog, const ShapeBuilder& builder)
	 : make_shape(builder)
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , nva(4)
	 , vbos(nva)
	{
		// bind the VAO for the shape
		vao.Bind();

		typename ShapeBuilder::VertexAttribs vert_attr_info;
		const GLchar* vert_attr_name[] = {
			"Position",
			"Normal",
			"Tangent",
			"TexCoord"
		};
		for(GLuint va=0; va!=nva; ++va)
		{
			const GLchar* name = vert_attr_name[va];
			std::vector<GLfloat> data;
			auto getter = vert_attr_info.VertexAttribGetter(data, name);
			if(getter != nullptr)
			try
			{
				// bind the VBO for the vertex attribute
				vbos[va].Bind(Buffer::Target::Array);
				GLuint npv = getter(make_shape, data);
				// upload the data
				Buffer::Data(Buffer::Target::Array, data);
				// setup the vertex attribs array
				VertexArrayAttrib attr(prog, name);
				attr.Setup<GLfloat>(npv);
				attr.Enable();
			}
			catch(Error&)
			{ }
		}
	}

	void Draw(void)
	{
		vao.Bind();
		gl.FrontFace(make_shape.FaceWinding());
		shape_instr.Draw(shape_indices);
	}
};

class BilliardExample : public Example
{
private:

	// wrapper around the current OpenGL context
	Context gl;

	VertexProgram vert_prog;
	GeometryProgram geom_prog;
	ClothProgram cloth_prog;
	BallProgram ball_prog;

	ProgramPipeline cloth_pp, ball_pp;

	Vec3f plane_u, plane_v;
	Shape<shapes::Plane> plane;
	Shape<shapes::Sphere> sphere;

	const GLuint ball_count;

	std::vector<Vec3f> ball_colors;
	std::vector<Vec3f> ball_offsets;
	std::vector<Vec3f> ball_rotations;

	// The table light map and the associated framebuffer
	Texture table_light_map;

	// The array texture storing the ball number decals
	Texture numbers_texture;

	// The texture used to render the cloth
	Texture cloth_texture;

	// The array of cube-maps storing the environment maps of the balls
	Array<Texture> reflect_textures;
public:
	BilliardExample(void)
	 : gl()
	 , geom_prog(DefaultGeomShader())
	 , plane_u(16, 0, 0)
	 , plane_v(0, 0,-16)
	 , plane(vert_prog, shapes::Plane(plane_u, plane_v))
	 , sphere(vert_prog, shapes::Sphere(1.0, 36, 24))
	 , ball_count(OGLPLUS_EXAMPLE_034BB_BALL_COUNT)
	 , ball_colors(
		{
			{0.8f, 0.5f, 0.2f},
			{0.2f, 0.2f, 0.5f},
			{0.6f, 0.2f, 0.4f},
			{0.1f, 0.1f, 0.3f},
			{0.0f, 0.0f, 0.0f},
			{0.3f, 0.1f, 0.2f},
			{0.2f, 0.5f, 0.2f},
			{0.6f, 0.3f, 0.2f}
		}
	), ball_offsets(
		{
			{ 3.0f, 1.0f, 6.5f},
			{ 5.0f, 1.0f, 5.0f},
			{ 3.0f, 1.0f,-1.0f},
			{-0.1f, 1.0f,-1.1f},
			{-3.0f, 1.0f, 3.0f},
			{-2.8f, 1.0f, 7.0f},
			{-1.1f, 1.0f, 9.0f},
			{ 3.0f, 1.0f, 2.0f},
			{-7.0f, 1.0f, 3.0f},
			{-9.5f, 1.0f, 4.5f},
			{ 1.0f, 1.0f, 5.2f},
			{-8.0f, 1.0f, 8.0f},
			{-5.0f, 1.0f, 1.0f},
			{ 2.0f, 1.0f, 9.0f},
			{ 8.0f, 1.0f, 7.5f}
		}
	), ball_rotations(
		{
			{ 0.3f,-0.2f,-0.1f},
			{ 0.2f, 0.3f, 0.4f},
			{-0.4f,-0.4f, 0.2f},
			{ 0.2f, 0.3f,-0.4f},
			{-0.7f,-0.2f, 0.6f},
			{ 0.3f, 0.3f, 0.2f},
			{ 0.5f, 0.2f, 0.3f},
			{-0.4f, 0.4f,-0.4f},
			{ 0.3f,-0.3f, 0.1f},
			{ 0.1f,-0.2f,-0.2f},
			{-0.2f,-0.3f,-0.0f},
			{-0.3f, 0.5f, 0.3f},
			{-0.4f, 0.1f, 0.1f},
			{ 0.3f, 0.3f,-0.2f},
			{-0.2f,-0.2f, 0.4f}
		}
	), reflect_textures(ball_count)
	{
		assert(ball_offsets.size() == ball_count);
		assert(ball_rotations.size() == ball_count);

		gl.RequireAtLeast(LimitQuery::MaxCombinedTextureImageUnits, 5);

		NoProgram().Use();

		cloth_pp.Bind();
		cloth_pp.UseStages(vert_prog).Vertex();
		cloth_pp.UseStages(geom_prog).Geometry();
		cloth_pp.UseStages(cloth_prog).Fragment();

		ball_pp.Bind();
		ball_pp.UseStages(vert_prog).Vertex();
		ball_pp.UseStages(geom_prog).Geometry();
		ball_pp.UseStages(ball_prog).Fragment();

		const Vec3f light_position(0.0f, 20.0f, -2.0f);

		vert_prog.light_position.Set(light_position);

		cloth_prog.color_1.Set(Vec3f(0.1f, 0.3f, 0.1f));
		cloth_prog.color_2.Set(Vec3f(0.3f, 0.4f, 0.3f));

		Texture::Active(0);
		cloth_prog.light_map.Set(0);
		GLuint light_map_side = 512;
		gl.Bound(Texture::Target::_2D, table_light_map)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.Image2D(
				0,
				PixelDataInternalFormat::RGB,
				light_map_side,
				light_map_side,
				0,
				PixelDataFormat::RGB,
				PixelDataType::UnsignedByte,
				nullptr
			);

		PrerenderLightmap(light_position, light_map_side);

		gl.ClearColor(0.12f, 0.13f, 0.11f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);

		Texture::Active(1);
		ball_prog.number_tex.Set(1);
		{
			const char* tex_names[OGLPLUS_EXAMPLE_034BB_BALL_COUNT] = {
				"pool_ball_1",
				"pool_ball_2",
				"pool_ball_3",
				"pool_ball_4",
				"pool_ball_5",
				"pool_ball_6",
				"pool_ball_7",
				"pool_ball_8",
				"pool_ball_9",
				"pool_ball10",
				"pool_ball11",
				"pool_ball12",
				"pool_ball13",
				"pool_ball14",
				"pool_ball15",
			};

			auto bound_tex = gl.Bound(Texture::Target::_2DArray, numbers_texture)
				.BorderColor(Vec4f(0,0,0,0))
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.Wrap(TextureWrap::ClampToBorder);

			for(GLuint i=0; i!=ball_count; ++i)
			{
				auto image = images::LoadTexture(tex_names[i]);
				if(i == 0)
				{
					bound_tex.Image3D(
						0,
						PixelDataInternalFormat::RGBA,
						image.Width(),
						image.Height(),
						16,
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
		}

		Texture::Active(2);
		cloth_prog.cloth_tex.Set(2);
		gl.Bound(Texture::Target::_2D, cloth_texture)
			.MinFilter(TextureMinFilter::LinearMipmapLinear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::Repeat)
			.WrapT(TextureWrap::Repeat)
			.Image2D(
				images::BrushedMetalUByte(
					512, 512,
					10240,
					-16, +16,
					8, 32
				)
			).GenerateMipmap();

		Texture::Active(3);
		ball_prog.reflect_tex.Set(3);

		GLuint cubemap_side = 128;

		Array<Texture> temp_cubemaps(ball_count);

		InitializeCubemaps(reflect_textures, cubemap_side);
		InitializeCubemaps(temp_cubemaps, cubemap_side);

		// prerender the cubemaps
		PrerenderCubemaps(temp_cubemaps, reflect_textures, cubemap_side);

		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);
	}

	void InitializeCubemaps(Array<Texture>& cubemaps, GLuint cubemap_side)
	{
		std::vector<GLubyte> black(cubemap_side*cubemap_side*3, 0x00);

		for(GLuint b=0; b!=ball_count; ++b)
		{
			gl.Bound(Texture::Target::CubeMap, cubemaps[b])
				.Filter(TextureFilter::Linear)
				.Wrap(TextureWrap::ClampToEdge);

			for(int f=0; f!=6; ++f)
			{
				Texture::ImageCM(
					f,
					0,
					PixelDataInternalFormat::RGB,
					cubemap_side, cubemap_side,
					0,
					PixelDataFormat::RGB,
					PixelDataType::UnsignedByte,
					black.data()
				);
			}
		}
	}

	void PrerenderLightmap(const Vec3f& light_position, GLuint tex_side)
	{
		Framebuffer table_light_fbo;
		gl.Bound(Framebuffer::Target::Draw, table_light_fbo)
			.AttachTexture(
				FramebufferAttachment::Color,
				table_light_map,
				0
			);

		gl.Viewport(tex_side, tex_side);
		gl.ClearColor(1.0 ,1.0, 1.0, 0.0);
		gl.Clear().ColorBuffer();

		LightmapProgram prog;

		Shape<shapes::Plane> tmp_plane(prog, shapes::Plane(plane_u, plane_v));


		GLfloat i_u = Length(plane_u);
		i_u = 1.0f / (i_u*i_u);
		GLfloat i_v = Length(plane_v);
		i_v = 1.0f / (i_v*i_v);
		prog.transform_matrix.Set(
			Mat4f(
				Vec4f(plane_u * i_u, 0.0f),
				Vec4f(plane_v * i_v, 0.0f),
				Vec4f(0.0f, 1.0f, 0.0f, 0.0f),
				Vec4f(0.0f, 0.0f, 0.0f, 1.0f)
			)
		);
		prog.light_position.Set(light_position);
		prog.ball_positions.Set(ball_offsets);

		gl.Disable(Capability::DepthTest);

		tmp_plane.Draw();

		gl.Enable(Capability::DepthTest);

		NoProgram().Use();
	}

	void PrerenderCubemaps(
		Array<Texture>& src_texs,
		Array<Texture>& dst_texs,
		GLuint tex_side
	)
	{
		Texture::Active(4);
		Texture z_buffer;
		gl.Bound(Texture::Target::CubeMap, z_buffer)
			.Filter(TextureFilter::Nearest)
			.Wrap(TextureWrap::ClampToEdge);

		for(int i=0; i!=6; ++i)
		{
			Texture::Image2D(
				Texture::CubeMapFace(i),
				0,
				PixelDataInternalFormat::DepthComponent,
				tex_side, tex_side,
				0,
				PixelDataFormat::DepthComponent,
				PixelDataType::Float,
				nullptr
			);
		}

		Texture::Active(3);

		Framebuffer fbo;
		gl.Bound(Framebuffer::Target::Draw, fbo)
			.AttachTexture(
				FramebufferAttachment::Depth,
				z_buffer,
				0
			);

		CubemapGeomShader cmap_geom_shader;
		GeometryProgram cmap_geom_prog(cmap_geom_shader);

		ProgramPipeline cmap_cloth_pp, cmap_ball_pp;

		NoProgram().Use();

		cmap_cloth_pp.Bind();
		cmap_cloth_pp.UseStages(vert_prog).Vertex();
		cmap_cloth_pp.UseStages(cmap_geom_prog).Geometry();
		cmap_cloth_pp.UseStages(cloth_prog).Fragment();

		cmap_ball_pp.Bind();
		cmap_ball_pp.UseStages(vert_prog).Vertex();
		cmap_ball_pp.UseStages(cmap_geom_prog).Geometry();
		cmap_ball_pp.UseStages(ball_prog).Fragment();

		cmap_geom_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(Degrees(90), 1.0, 1, 80)
		);
		gl.Viewport(tex_side, tex_side);

		for(GLuint b=0; b!=ball_count; ++b)
		{
			Framebuffer::AttachTexture(
				Framebuffer::Target::Draw,
				FramebufferAttachment::Color,
				dst_texs[b],
				0
			);

			vert_prog.camera_position.Set(ball_offsets[b]);
			cmap_geom_prog.camera_matrix.Set(
				ModelMatrixf::Translation(-ball_offsets[b])
			);

			RenderScene(src_texs, cmap_cloth_pp, cmap_ball_pp, b);
		}
	}

	void RenderScene(
		Array<Texture>& cubemaps,
		const ProgramPipeline& cloth_prog_pipeline,
		const ProgramPipeline& ball_prog_pipeline,
		int skipped = -1
	)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		// Render the plane
		cloth_prog_pipeline.Bind();

		vert_prog.model_matrix = ModelMatrixf();
		vert_prog.texture_matrix.Set(Mat3f(
			16.0,  0.0,  0.0,
			 0.0, 16.0,  0.0,
			 0.0,  0.0,  1.0
		));

		plane.Draw();

		// Render the balls
		ball_prog_pipeline.Bind();

		vert_prog.texture_matrix.Set(Mat3f(
			6.0f, 0.0f, 0.0f,
			0.0f, 3.0f,-1.0f,
			0.0f, 0.0f, 1.0f
		));
		for(GLuint i=0; i!=ball_count; ++i)
		{
			if(int(i) == skipped) continue;
			Vec3f rot = ball_rotations[i];
			int ci = ((i / 4) % 2 == 0)?i : ((i/4)+2)*4-i-1;
			ci %= 8;
			Vec3f col = ball_colors[ci];
			vert_prog.model_matrix = ModelMatrixf(
				ModelMatrixf::Translation(ball_offsets[i]) *
				ModelMatrixf::RotationZ(FullCircles(rot.z())) *
				ModelMatrixf::RotationY(FullCircles(rot.y())) *
				ModelMatrixf::RotationX(FullCircles(rot.x()))
			);
			if(i > 7)
			{
				ball_prog.color_1 = Vec3f(1.0f, 0.9f, 0.8f);
			}
			else
			{
				ball_prog.color_1 = col;
			}
			ball_prog.color_2 = col;
			ball_prog.ball_idx = i;

			cubemaps[i].Bind(Texture::Target::CubeMap);

			sphere.Draw();
		}
	}

	void Render(double time)
	{
		// setup the camera
		Vec3f camera_target(0.0f, 2.2f, 5.0f);
		auto camera = CamMatrixf::Orbiting(
			camera_target,
			GLfloat(16.0 - SineWave(time / 15.0)*12.0),
			FullCircles(time / 24.0),
			Degrees(50 + SineWave(time / 20.0) * 35)
		);

		Vec3f camera_position = camera.Position();
		vert_prog.camera_position.Set(camera_position);
		geom_prog.camera_matrix.Set(camera);

		RenderScene(reflect_textures, cloth_pp, ball_pp);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		geom_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				width, height,
				1, 80
			)
		);
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double ScreenshotTime(void) const
	{
		return 1.5;
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
	return std::unique_ptr<Example>(new BilliardExample);
}

} // namespace oglplus
