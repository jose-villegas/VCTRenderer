/**
 *  @example oglplus/028_monkeycraft.cpp
 *  @brief Shows the use of geometry shader and a ambient occlusion technique.
 *
 *  @oglplus_screenshot{028_monkeycraft}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 *  @oglplus_example_uses_texture{oglcraft_ao}
 *  @oglplus_example_uses_texture{suzanne_vol}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/dsa/uniform.hpp>
#include <oglplus/dsa/ext/buffer.hpp>
#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/ext/vertex_array.hpp>
#include <oglplus/dsa/ext/vertex_attrib.hpp>
#include <oglplus/images/load.hpp>
#include <oglplus/images/transformed.hpp>
#include <oglplus/images/random.hpp>
#include <oglplus/opt/list_init.hpp>

#include "example.hpp"

namespace oglplus {

class CubeProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"in vec4 Position;"
			"in ivec3 TexCoord;"
			"out ivec3 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertTexCoord = TexCoord;"
			"}"
		).Compile();
		prog.AttachShader(vs);

		GeometryShader gs;
		gs.Source(
			"#version 150\n"
			"layout (points) in;"
			"layout (triangle_strip, max_vertices=24) out;"

			"uniform float FadeCoef;"
			"uniform sampler3D Pattern, FadeMap;"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"mat4 Matrix = ProjectionMatrix*CameraMatrix;"
			"uniform vec3 LightPosition = vec3(100, 120, 150);"

			"in ivec3 vertTexCoord[1];"

			"out vec3 geomNormal;"
			"out vec3 geomLightDir;"
			"out vec3 geomTexCoord;"
			"out float geomShadow;"

			"float get_cell(ivec3 offs)"
			"{"
			"	ivec3 coord = vertTexCoord[0]+offs;"
			"	float patt = texelFetch(Pattern, coord, 0).r;"
			"	float fade = texelFetch(FadeMap, coord, 0).r;"
			"	return clamp("
			"		(patt != 0.0)?"
			"		(FadeCoef-fade)*256.0:"
			"		0.0, 0.0, 1.0"
			"	);"
			"}"

			"const ivec3 coffs[6] = ivec3[6]("
			"	ivec3(+1, 0, 0),"
			"	ivec3(-1, 0, 0),"
			"	ivec3( 0,+1, 0),"
			"	ivec3( 0,-1, 0),"
			"	ivec3( 0, 0,+1),"
			"	ivec3( 0, 0,-1) "
			");"

			"const mat3 tmats[6] = mat3[6]("
			"	mat3( 0, 0, 1,  0,-1, 0,  1, 0, 0),"
			"	mat3( 0, 0,-1,  0,-1, 0, -1, 0, 0),"
			"	mat3(-1, 0, 0,  0, 0, 1,  0, 1, 0),"
			"	mat3(-1, 0, 0,  0, 0,-1,  0,-1, 0),"
			"	mat3( 1, 0, 0,  0, 1, 0,  0, 0, 1),"
			"	mat3(-1, 0, 0,  0, 1, 0,  0, 0,-1) "
			");"

			"const vec3[8] nvec = vec3[8]("
			"	vec3( 0,-1,+1),"
			"	vec3(+1,-1,+1),"
			"	vec3(+1, 0,+1),"
			"	vec3(+1,+1,+1),"
			"	vec3( 0,+1,+1),"
			"	vec3(-1,+1,+1),"
			"	vec3(-1, 0,+1),"
			"	vec3(-1,-1,+1) "
			");"

			"void main(void)"
			"{"
			"	float cc = get_cell(ivec3(0,0,0));"
			"	if(cc > 0.0)"
			"	{"
			"		float c05 = 0.5*cc;"
			"		float c0 = 0.5-c05;"
			"		float c1 = 0.5+c05;"

			"		for(int f=0; f!=6; ++f)"
			"		if(get_cell(coffs[f]) < 1.0)"
			"		{"
			"			int config = 0x0;"
			"			vec3 noffs[8];"
			"			vec3 nml = tmats[f][2];"
			"			float aoc = get_cell(ivec3(nml*2));"
			"			aoc += 0.8*get_cell(ivec3(nml*3));"
			"			for(int n=0; n!=8; ++n)"
			"			{"
			"				noffs[n] = nvec[n]*tmats[f];"
			"				float nc = get_cell(ivec3(noffs[n]));"
			"				aoc += 0.2*get_cell(ivec3(noffs[n]+nml));"
			"				aoc += 0.1*get_cell(ivec3(noffs[n]+nml*2));"
			"				config |= (nc>0.0)?0x1<<n:0x0;"
			"			}"
			"			float aom = float(config);"

			"			geomNormal = tmats[f][2];"

			"			geomShadow = max(1.0-sqrt(aoc), 0.0);"

			"			vec3 cpos = gl_in[0].gl_Position.xyz;"

			"			gl_Position = vec4(cpos+noffs[7]*c05, 1.0);"
			"			geomLightDir = LightPosition - gl_Position.xyz;"
			"			gl_Position = Matrix * gl_Position;"
			"			geomTexCoord = vec3(c0, c1, aom);"
			"			EmitVertex();"

			"			gl_Position = vec4(cpos+noffs[1]*c05, 1.0);"
			"			geomLightDir = LightPosition - gl_Position.xyz;"
			"			gl_Position = Matrix * gl_Position;"
			"			geomTexCoord = vec3(c1, c1, aom);"
			"			EmitVertex();"

			"			gl_Position = vec4(cpos+noffs[5]*c05, 1.0);"
			"			geomLightDir = LightPosition - gl_Position.xyz;"
			"			gl_Position = Matrix * gl_Position;"
			"			geomTexCoord = vec3(c0, c0, aom);"
			"			EmitVertex();"

			"			gl_Position = vec4(cpos+noffs[3]*c05, 1.0);"
			"			geomLightDir = LightPosition - gl_Position.xyz;"
			"			gl_Position = Matrix * gl_Position;"
			"			geomTexCoord = vec3(c1, c0, aom);"
			"			EmitVertex();"

			"			EndPrimitive();"
			"		}"
			"	}"
			"}"
		).Compile();
		prog.AttachShader(gs);

		FragmentShader fs;
		fs.Source(
			"#version 150\n"

			"uniform sampler2DArray AmbiOcclMaps;"

			"in vec3 geomNormal;"
			"in vec3 geomLightDir;"
			"in vec3 geomTexCoord;"
			"in float geomShadow;"

			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	float amoc = texture(AmbiOcclMaps, geomTexCoord).r;"
			"	float shdw = sqrt(0.5*(1.0+geomShadow));"
			"	float ambi = 0.5;"
			"	float diff = max(dot(geomNormal, normalize(geomLightDir)), 0)*0.6;"
			"	float light = ambi*amoc*shdw + (0.8*amoc*shdw+0.2)*diff;"
			"	fragColor = vec3(1,1,1)*light;"
			"}"
		).Compile();
		prog.AttachShader(fs);

		prog.Link().Use();

		return std::move(prog);
	}

	Program& self(void) { return *this; }
public:
	ProgramUniform<GLfloat> fade_coef;
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<Vec3f> light_position;

	CubeProg(void)
	 : Program(make())
	 , fade_coef(self(), "FadeCoef")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , light_position(self(), "LightPosition")
	{ }
};

class AmbiOcclMaps
 : DSATextureEXT
{
public:
	AmbiOcclMaps(const images::Image& smaps, const Program& prog)
	{

		ProgramUniformSampler(prog, "AmbiOcclMaps").Set(0);
		this->target = Texture::Target::_2DArray;
		this->BindMulti(0);
		this->Image3D(smaps);
		this->MinFilter(TextureMinFilter::Linear);
		this->MagFilter(TextureMagFilter::Linear);
		this->WrapS(TextureWrap::ClampToEdge);
		this->WrapT(TextureWrap::ClampToEdge);
	}
};

class Field
{
private:
	Context gl;

	DSABufferEXT positions, texcoords;
	DSAVertexArrayEXT vao;
	DSATextureEXT pattern, fademap;

	GLuint vert_count;

	GLfloat radius;
public:
	Field(const images::Image& map, const Program& prog)
	 : gl()
	{
		const GLuint w = map.Width();
		const GLuint h = map.Height();
		const GLuint d = map.Depth();
		const GLuint c = 3;

		radius = std::sqrt(GLfloat(w*w+h*h+d*d))*0.5f;

		vert_count = w*h*d;

		std::vector<GLfloat> pos_data(w*h*d*c);
		std::vector<GLuint>  tec_data(w*h*d*c);

		GLfloat xo = w * 0.5f;
		GLfloat yo = h * 0.5f;
		GLfloat zo = d * 0.5f;

		for(GLuint z=0; z!=d; ++z)
		{
			for(GLuint y=0; y!=h; ++y)
			{
				for(GLuint x=0; x!=w; ++x)
				{
					GLuint k = z*w*h*c+y*w*c+x*c;
					pos_data[k+0] = x-xo;
					pos_data[k+1] = y-yo;
					pos_data[k+2] = z-zo;

					tec_data[k+0] = x;
					tec_data[k+1] = y;
					tec_data[k+2] = z;
				}
			}
		}

		positions.Data(pos_data);
		texcoords.Data(tec_data);

		DSAVertexArrayAttribEXT(vao, prog, "Position")
			.Setup<Vector<GLfloat, 3>>(positions)
			.Enable();

		DSAVertexArrayAttribEXT(vao, prog, "TexCoord")
			.Setup<Vector<GLuint, 3>>(texcoords)
			.Enable();


		ProgramUniformSampler(prog, "Pattern").Set(1);
		pattern.target = Texture::Target::_3D;
		pattern.BindMulti(1);
		pattern.Image3D(map);
		pattern.Filter(TextureFilter::Nearest);
		pattern.Wrap(TextureWrap::ClampToBorder);
		pattern.BorderColor(Vec4f(0,0,0,1));


		ProgramUniformSampler(prog, "FadeMap").Set(2);
		fademap.target = Texture::Target::_3D;
		fademap.BindMulti(2);
		fademap.Image3D(images::RandomRedUByte(
			map.Width(),
			map.Height(),
			map.Depth()
		));
		fademap.Filter(TextureFilter::Nearest);
		fademap.Wrap(TextureWrap::ClampToBorder);
		fademap.BorderColor(Vec4f(0,0,0,1));
	}

	void Use(void)
	{
		vao.Bind();
	}

	void Draw(void)
	{
		gl.DrawArrays(PrimitiveType::Points, 0, vert_count);
	}

	GLfloat Radius(void) const
	{
		return radius;
	}
};

class CubeExample : public Example
{
private:
	Context gl;

	CubeProg prog;

	Field field;
	AmbiOcclMaps shadowmaps;
public:
	CubeExample(void)
	 : gl()
	 , prog()
	 , field(
		images::FlipImageAxes<GLubyte, 1>(
			images::LoadTexture("suzanne_vol"),
			0, 2,-1
		),
		prog
	), shadowmaps(
		images::LoadTexture("oglcraft_ao"),
		prog
	)
	{
		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(FaceOrientation::CCW);
		gl.CullFace(Face::Back);

		prog.Use();
		field.Use();
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				GLfloat(width)/height,
				1, field.Radius()*3
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(field.Radius()*(1.4 + SineWave(time / 6.0) * 0.2)),
			FullCircles(time * 0.1),
			Degrees(SineWave(time / 30.0) * 90)
		);

		auto light = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(field.Radius()*1.6),
			FullCircles(0.33-time * 0.07),
			Degrees(SineWave(time / 31.0) * 80)
		);

		prog.camera_matrix.Set(camera);
		prog.light_position.Set(light.Position());

		GLfloat fade_coef = GLfloat(1.1*(1.0-CosineWave01(time / 90.0)));
		prog.fade_coef.Set(fade_coef);

		field.Draw();
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double ScreenshotTime(void) const
	{
		return 28.0;
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
	return std::unique_ptr<Example>(new CubeExample);
}

} // namespace oglplus
