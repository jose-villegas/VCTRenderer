/**
 *  @example oglplus/022_parallax_map.cpp
 *  @brief Shows how to achieve a simple view parallax effect on a checker cube
 *
 *  @oglplus_screenshot{022_parallax_map}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/images/sphere_bmap.hpp>
#include <oglplus/bound/buffer.hpp>
#include <oglplus/bound/texture.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class CubeExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions cube_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray cube_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;

	static Program make(void)
	{
		Shader vs(ShaderType::Vertex);
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"uniform vec3 LightPos;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec3 Tangent;"
			"in vec2 TexCoord;"
			"out vec3 vertEye;"
			"out vec3 vertLight;"
			"out vec3 vertNormal;"
			"out vec2 vertTexCoord;"
			"out vec3 vertViewTangent;"
			"out mat3 NormalMatrix;"
			"void main(void)"
			"{"
			"	vec4 EyePos = "
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		Position;"
			"	vertEye = EyePos.xyz;"
			"	vec3 fragTangent = ("
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		vec4(Tangent, 0.0)"
			"	).xyz;"
			"	vertNormal = ("
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		vec4(Normal, 0.0)"
			"	).xyz;"
			"	vertLight = ("
			"		CameraMatrix *"
			"		vec4(LightPos-vertEye, 1.0)"
			"	).xyz;"
			"	NormalMatrix = mat3("
			"		fragTangent,"
			"		cross(vertNormal, fragTangent),"
			"		vertNormal"
			"	);"
			"	vertViewTangent = vec3("
			"		dot(NormalMatrix[0], vertEye),"
			"		dot(NormalMatrix[1], vertEye),"
			"		dot(NormalMatrix[2], vertEye) "
			"	);"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = ProjectionMatrix *"
			"		EyePos;"
			"}"
		).Compile();

		Shader fs(ShaderType::Fragment);
		fs.Source(
			"#version 140\n"
			"uniform sampler2D BumpTex;"
			"uniform int BumpTexWidth;"
			"uniform int BumpTexHeight;"
			"float DepthMult = 0.1;"
			"in vec3 vertEye;"
			"in vec3 vertLight;"
			"in vec3 vertNormal;"
			"in vec2 vertTexCoord;"
			"in vec3 vertViewTangent;"
			"in mat3 NormalMatrix;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec3 ViewTangent = normalize(vertViewTangent);"
			"	float perp = -dot(normalize(vertEye), vertNormal);"
			"	float sampleInterval = 1.0/length("
			"		vec2(BumpTexWidth, BumpTexHeight)"
			"	);"
			"	vec3 sampleStep = ViewTangent*sampleInterval;"
			"	float prevD = 0.0;"
			"	float depth = texture(BumpTex, vertTexCoord).w;"
			"	float maxOffs = min((depth*DepthMult)/(-ViewTangent.z), 1.0);"
			"	vec3 viewOffs = vec3(0.0, 0.0, 0.0);"
			"	vec2 offsTexC = vertTexCoord + viewOffs.xy;"
			"	while(length(viewOffs) < maxOffs)"
			"	{"
			"		if(offsTexC.x <= 0.0 || offsTexC.x >= 1.0)"
			"			break;"
			"		if(offsTexC.y <= 0.0 || offsTexC.y >= 1.0)"
			"			break;"
			"		if(depth*DepthMult*perp <= -viewOffs.z)"
			"			break;"
			"		viewOffs += sampleStep;"
			"		offsTexC = vertTexCoord + viewOffs.xy;"
			"		prevD = depth;"
			"		depth = texture(BumpTex, offsTexC).w;"
			"	}"
			"	offsTexC = vec2("
			"		clamp(offsTexC.x, 0.0, 1.0),"
			"		clamp(offsTexC.y, 0.0, 1.0) "
			"	);"
			"	float b = ("
			"		1 +"
			"		int(offsTexC.x*16) % 2+"
			"		int(offsTexC.y*16) % 2"
			"	) % 2;"
			"	vec3 c = vec3(b, b, b);"
			"	vec3 n = texture(BumpTex, offsTexC).xyz;"
			"	vec3 finalNormal = NormalMatrix * n;"
			"	float l = length(vertLight);"
			"	float d = (l > 0.0) ? dot("
			"		normalize(vertLight), "
			"		finalNormal"
			"	) / l : 0.0;"
			"	float i = 0.1 + 2.5*max(d, 0.0);"
			"	fragColor = vec4(c*i, 1.0);"
			"}"
		).Compile();

		Program prog;
		prog.AttachShader(vs).AttachShader(fs);
		prog.Link();
		prog.Use();

		return prog;
	}

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	Uniform<Vec3f> light_pos;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertex attributes
	Buffer verts, normals, tangents, texcoords;

	// The bumpmap texture
	Texture bumpTex;
public:
	CubeExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , prog(make())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , light_pos(prog, "LightPos")
	{
		// bind the VAO for the cube
		gl.Bind(cube);

		gl.Bind(Buffer::Target::Array, verts);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			(prog|"Position").Setup<GLfloat>(n_per_vertex).Enable();
		}

		gl.Bind(Buffer::Target::Array, normals);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			(prog|"Normal").Setup<GLfloat>(n_per_vertex).Enable();
		}

		gl.Bind(Buffer::Target::Array, tangents);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Tangents(data);
			Buffer::Data(Buffer::Target::Array, data);
			(prog|"Tangent").Setup<GLfloat>(n_per_vertex).Enable();
		}

		gl.Bind(Buffer::Target::Array, texcoords);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.TexCoordinates(data);
			Buffer::Data(Buffer::Target::Array, data);
			(prog|"TexCoord").Setup<GLfloat>(n_per_vertex).Enable();
		}

		{
			auto img = images::SphereBumpMap(512, 512, 2, 2);
			Uniform<GLsizei>(prog, "BumpTexWidth").Set(img.Width());
			Uniform<GLsizei>(prog, "BumpTexHeight").Set(img.Height());
			UniformSampler(prog, "BumpTex").Set(0);
			Texture::Active(0);

			gl.Bound(Texture::Target::_2D, bumpTex)
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::Repeat)
				.Image2D(img)
				.GenerateMipmap();
		}
		//
		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_cube.FaceWinding());
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(54),
				float(width)/height,
				1, 10
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto lightAzimuth = FullCircles(time * -0.5);
		light_pos.Set(
			Vec3f(
				-Cos(lightAzimuth),
				1.0f,
				-Sin(lightAzimuth)
			) * 2.0f
		);
		//
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.0f,
				Degrees(-45),
				Degrees(SineWave(time / 30.0) * 70)
			)
		);

		// set the model matrix
		model_matrix.Set(
			ModelMatrixf::RotationA(
				Vec3f(1.0f, 1.0f, 1.0f),
				FullCircles(-time * 0.05)
			)
		);

		gl.CullFace(Face::Back);
		cube_instr.Draw(cube_indices);
	}

	bool Continue(double time)
	{
		return time < 30.0;
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
