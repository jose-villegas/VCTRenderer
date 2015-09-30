/**
 *  @example oglplus/029_flares.cpp
 *  @brief Shows how to use queries to influence rendering of effects like flares
 *
 *  @oglplus_screenshot{029_flares}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 *
 *  @oglplus_example_uses_texture{flare_1}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/spiral_sphere.hpp>

#include <oglplus/images/brushed_metal.hpp>
#include <oglplus/images/load.hpp>

#include <vector>
#include <cstdlib>

#include "example.hpp"

namespace oglplus {

class ShapeVertShader
 : public VertexShader
{
public:
	ShapeVertShader(void)
	 : VertexShader(
		ObjectDesc("Shape vertex shader"),
		StrCRef("#version 150\n"

		"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
		"uniform vec3 CameraPosition;"

		"in vec4 Position;"
		"in vec3 Normal;"
		"in vec3 Tangent;"
		"in vec2 TexCoord;"

		"out vec3 vertPosition;"
		"out vec3 vertViewDir;"
		"out vec3 vertNormal;"
		"out vec3 vertTangent;"
		"out vec3 vertBitangent;"
		"out vec2 vertTexCoord;"
		"void main(void)"
		"{"
		"	gl_Position = ModelMatrix * Position;"
		"	vertPosition = gl_Position.xyz;"
		"	vertViewDir = CameraPosition - gl_Position.xyz;"
		"	vertNormal =  (ModelMatrix * vec4(Normal,  0.0)).xyz;"
		"	vertTangent =  (ModelMatrix * vec4(Tangent,  0.0)).xyz;"
		"	vertBitangent = cross(vertNormal, vertTangent);"
		"	vertTexCoord = mat2(0.0, 2.0, 1.0, 0.0) * TexCoord;"
		"	gl_Position = "
		"		ProjectionMatrix * "
		"		CameraMatrix * "
		"		gl_Position;"
		"}")
	)
	{ }
};

class ShapeFragShader
 : public FragmentShader
{
public:
	ShapeFragShader(void)
	 : FragmentShader(
		ObjectDesc("Shape fragment shader"),
		StrCRef("#version 150\n"

		"const int LightCount = 32;"

		"uniform vec3 Color1, Color2;"
		"uniform vec3 LightPosition[LightCount];"
		"uniform sampler2D MetalTex;"

		"in vec3 vertPosition;"
		"in vec3 vertViewDir;"
		"in vec3 vertNormal;"
		"in vec3 vertTangent;"
		"in vec3 vertBitangent;"
		"in vec2 vertTexCoord;"

		"out vec3 fragColor;"

		"const vec3 LightColor = vec3(1.0, 1.0, 1.0);"

		"void main(void)"
		"{"
		"	vec4 Sample = texture(MetalTex, vertTexCoord);"

		"	vec3 fragNormal = normalize("
		"		(Sample.b + 0.5)*vertNormal + "
		"		(Sample.r - 0.5)*vertTangent + "
		"		(Sample.g - 0.5)*vertBitangent"
		"	);"

		"	float Specular = 0.0, Diffuse = 0.0;"
		"	for(int l=0; l!=LightCount; ++l)"
		"	{"
		"		vec3 LightDir = normalize(LightPosition[l]-vertPosition);"
		"		vec3 LightRefl = reflect("
		"			-LightDir,"
		"			fragNormal"
		"		);"

		"		Specular += pow(max(dot("
		"			normalize(LightRefl),"
		"			normalize(vertViewDir)"
		"		)+0.04, 0.0), 32+Sample.b*32)*pow(0.4+Sample.b*1.6, 4.0);"

		"		Diffuse += pow(max(dot("
		"			LightDir, "
		"			normalize(vertNormal*2.0 + fragNormal)"
		"		), 0.0), 2.0);"
		"	}"

		"	float Ambient = 0.1;"
		"	Diffuse /= LightCount;"
		"	Specular /= sqrt(float(LightCount));"

		"	vec3 Color = mix(Color1, Color2, Sample.b);"

		"	fragColor = "
		"		Color * Ambient +"
		"		Color * Diffuse +"
		"		LightColor * Specular;"
		"}")
	)
	{ }
};

class ShapeProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Shape program"));
		prog.AttachShader(ShapeVertShader());
		prog.AttachShader(ShapeFragShader());
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniform<Vec3f> color_1, color_2;
	ProgramUniformSampler metal_tex;

	ShapeProgram(void)
	 : Program(make())
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	 , color_1(prog(), "Color1")
	 , color_2(prog(), "Color2")
	 , metal_tex(prog(), "MetalTex")
	{ }
};

class LightVertShader
 : public VertexShader
{
public:
	LightVertShader(void)
	 : VertexShader(
		ObjectDesc("Light vertex shader"),
		StrCRef("#version 150\n"

		"uniform mat4 ProjectionMatrix, CameraMatrix;"
		"in vec4 Position;"

		"void main(void)"
		"{"
		"	gl_Position = ProjectionMatrix * CameraMatrix * Position;"
		"	gl_PointSize = 9.0;"
		"}")
	)
	{ }
};

class LightFragShader
 : public FragmentShader
{
public:
	LightFragShader(void)
	 : FragmentShader(
		ObjectDesc("Light fragment shader"),
		StrCRef("#version 150\n"

		"out vec3 fragColor;"

		"void main(void)"
		"{"
		"	fragColor = vec3(1.0, 1.0, 1.0);"
		"}")
	)
	{ }

};

class LightProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Light program"));
		prog.AttachShader(LightVertShader());
		prog.AttachShader(LightFragShader());
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;

	LightProgram(void)
	 : Program(make())
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	{ }
};

class FlareVertShader
 : public VertexShader
{
public:
	FlareVertShader(void)
	 : VertexShader(
		ObjectDesc("Flare vertex shader"),
		StrCRef("#version 150\n"

		"uniform mat4 CameraMatrix;"
		"in vec4 Position;"

		"void main(void)"
		"{"
		"	gl_Position = CameraMatrix * Position;"
		"	gl_PointSize = 9.0;"
		"}")
	)
	{ }
};

class FlareGeomShader
 : public GeometryShader
{
public:
	FlareGeomShader(void)
	 : GeometryShader(
		ObjectDesc("Flare geometry shader"),
		StrCRef("#version 150\n"
		"layout(points) in;"
		"layout(triangle_strip, max_vertices = 72) out;"

		"uniform mat4 ProjectionMatrix;"
		"uniform int Samples;"

		"out vec2 geomTexCoord;"

		"void main(void)"
		"{"
		"	for(int l=0; l!=3; ++l)"
		"	{"
		"		int i = 0, n = 8 + l*4;"
		"		float step = (2.0 * 3.1415)/float(n-1);"
		"		float a = length(gl_in[0].gl_Position)*(0.3+l*0.4);"
		"		float Radius = "
		"			sqrt(Samples)*0.01 + "
		"			Samples*sqrt(float(l))*0.001;"
		"		while(i != n)"
		"		{"
		"			vec4 Offs = vec4(cos(a)*(1.0+l*0.2),sin(a),0,0);"
		"			gl_Position = "
		"				ProjectionMatrix * "
		"				gl_in[0].gl_Position;"
		"			geomTexCoord = vec2(float(i), 0.0);"
		"			EmitVertex();"
		"			gl_Position = "
		"				ProjectionMatrix * "
		"				(gl_in[0].gl_Position + Offs*Radius);"
		"			geomTexCoord = vec2(float(i), 1.05-l*0.05);"
		"			EmitVertex();"
		"			++i;"
		"			a += step;"
		"		}"
		"		EndPrimitive();"
		"	}"
		"}")
	)
	{ }

};

class FlareFragShader
 : public FragmentShader
{
public:
	FlareFragShader(void)
	 : FragmentShader(
		ObjectDesc("Flare fragment shader"),
		StrCRef("#version 150\n"
		"uniform sampler2D FlareTex;"

		"in vec2 geomTexCoord;"
		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	vec4 Sample = texture(FlareTex, geomTexCoord);"
		"	fragColor = vec4(Sample.rgb, Sample.a*0.4);"
		"}")
	)
	{ }

};

class FlareProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Flare"));
		prog.AttachShader(FlareVertShader());
		prog.AttachShader(FlareGeomShader());
		prog.AttachShader(FlareFragShader());
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<GLint> samples;
	ProgramUniformSampler flare_tex;

	FlareProgram(void)
	 : Program(make())
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , samples(prog(), "Samples")
	 , flare_tex(prog(), "FlareTex")
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

	// VBOs for the shape's vertex attributes
	Array<Buffer> vbos;
public:
	Shape(
		const Program& prog,
		const ShapeBuilder& builder
	): make_shape(builder)
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , vbos(4)
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
		for(int va=0; va!=4; ++va)
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
		shape_instr.Draw(shape_indices, 1);
	}
};

class FlareExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	ShapeProgram shape_prog;

	Shape<shapes::SpiralSphere> shape;

	Texture metal_texture;

	LightProgram light_prog;
	FlareProgram flare_prog;

	VertexArray lights;
	Buffer light_pos;

	Texture flare_texture;

	const GLuint n_flares;
	Array<Query> queries;
public:
	FlareExample(void)
	 : shape(shape_prog, shapes::SpiralSphere())
	 , n_flares(32)
	 , queries(n_flares)
	{
		std::vector<Vec3f> light_positions(n_flares);
		for(GLuint i=0; i!=n_flares; ++i)
		{
			const float rand_max = float(RAND_MAX);
			auto angle = FullCircles(std::rand()/rand_max);

			light_positions[i] = Vec3f(
				7.0f*Cos(angle),
				0.2f*(std::rand()/rand_max)-0.1f,
				7.0f*Sin(angle)
			);
		}

		shape_prog.light_position.Set(light_positions);
		shape_prog.color_1 = Vec3f(0.3f, 0.3f, 0.5f);
		shape_prog.color_2 = Vec3f(0.8f, 0.8f, 1.0f);

		Texture::Active(0);
		shape_prog.metal_tex.Set(0);
		metal_texture
			<< TextureTarget::_2D
			<< TextureMinFilter::LinearMipmapLinear
			<< TextureMagFilter::Linear
			<< TextureWrap::Repeat
			<< images::BrushedMetalUByte(
				512, 512,
				5120,
				-12, +12,
				32, 64
			)
			<< TextureMipmap();

		Texture::Active(1);
		UniformSampler(flare_prog, "FlareTex").Set(1);
		flare_texture
			<< TextureTarget::_2D
			<< TextureMinFilter::LinearMipmapLinear
			<< TextureMagFilter::Linear
			<< images::LoadTexture("flare_1")
			<< TextureMipmap();

		(TextureTarget::_2D|0) << TextureWrap::MirroredRepeat;
		(TextureTarget::_2D|1) << TextureWrap::Repeat;

		lights.Bind();
		try
		{
			light_pos.Bind(Buffer::Target::Array);
			Buffer::Data(Buffer::Target::Array, light_positions);

			light_prog.Use();
			VertexArrayAttrib light_attr(light_prog, "Position");
			light_attr.Setup<Vec3f>();
			light_attr.Enable();

			flare_prog.Use();
			VertexArrayAttrib flare_attr(flare_prog, "Position");
			flare_attr.Setup<Vec3f>();
			flare_attr.Enable();
		}
		catch(Error&)
		{ }

		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::ProgramPointSize);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::One);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		auto projection = CamMatrixf::PerspectiveX(
			Degrees(65),
			float(width)/height,
			1, 20
		);

		shape_prog.projection_matrix.Set(projection);
		light_prog.projection_matrix.Set(projection);
		flare_prog.projection_matrix.Set(projection);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		CamMatrixf camera = CamMatrixf::Orbiting(
			Vec3f(),
			4.5f + float(SineWave(time / 25.0)),
			FullCircles(time / 30.0),
			Degrees(SineWave(time / 19.0) * 20)
		);
		light_prog.camera_matrix.Set(camera);
		flare_prog.camera_matrix.Set(camera);
		shape_prog.camera_matrix.Set(camera);
		shape_prog.camera_position.Set(camera.Position());

		shape_prog.model_matrix.Set(
			ModelMatrixf::RotationX(FullCircles(time / 30.0))
		);

		shape_prog.Use();
		shape.Draw();
		NoProgram().Use();

		lights.Bind();

		light_prog.Use();

		for(GLuint l=0; l!=n_flares; ++l)
		{
			queries[l].Begin(Query::Target::SamplesPassed);
			gl.DrawArrays(PrimitiveType::Points, l, 1);
			queries[l].End(Query::Target::SamplesPassed);
		}

		gl.Enable(Capability::Blend);
		gl.Disable(Capability::DepthTest);
		flare_prog.Use();
		for(GLuint l=0; l!=n_flares; ++l)
		{
			GLint samples = 0;
			queries[l].WaitForResult(samples);
			if(samples != 0)
			{
				flare_prog.samples = samples;
				gl.DrawArrays(PrimitiveType::Points, l, 1);
			}
		}
		gl.Enable(Capability::DepthTest);
		gl.Disable(Capability::Blend);
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
	return std::unique_ptr<Example>(new FlareExample);
}

} // namespace oglplus
