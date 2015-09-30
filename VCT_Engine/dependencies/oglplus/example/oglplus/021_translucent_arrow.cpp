/**
 *  @example oglplus/021_translucent_arrow.cpp
 *  @brief Shows how to use the ObjMesh shape generator/loader
 *
 *  @oglplus_screenshot{021_translucent_arrow}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_model{arrow_z}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class DepthProgram
 : public Program
{
private:
	const Program& prog(void) const { return *this; }

	static Program make(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform mat4  ProjectionMatrix,CameraMatrix,ModelMatrix;"

			"in vec4 Position;"

			"void main(void)"
			"{"
			"	gl_Position = ProjectionMatrix * CameraMatrix * ModelMatrix * Position;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"void main(void) { }"
		).Compile();

		Program prog;
		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return std::move(prog);
	}
public:
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	DepthProgram(void)
	 : Program(make())
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	{
	}
};

class ShapeProgram
 : public Program
{
private:
	const Program& prog(void) const { return *this; }

	static Program make(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform mat4  ProjectionMatrix,CameraMatrix,ModelMatrix;"
			"mat3 RotMatrix = mat3(ModelMatrix);"

			"const vec3 LightPosition = vec3(12.0, 11.0, 17.0);"

			"in vec4 Position;"
			"in vec3 Normal;"

			"out vec3 vertLightDir;"
			"out vec3 vertNormal;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertNormal = RotMatrix * Normal;"
			"	vertColor = normalize("
			"		vec3(1, 1, 1) - "
			"		mix(vertNormal, Position.xyz, 0.5)"
			"	);"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"

			"uniform sampler2DRect DepthTex;"

			"in vec4 gl_FragCoord;"

			"in vec3 vertLightDir;"
			"in vec3 vertNormal;"
			"in vec3 vertColor;"

			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	float w = gl_FragCoord.w;"
			"	float sum_bz = 0.0;"
			"	const int nd = 2;"
			"	float ns = 0.0;"
			"	for(int dy=-nd; dy!=(nd+1); ++dy)"
			"	for(int dx=-nd; dx!=(nd+1); ++dx)"
			"	{"
			"		ivec2 TexelCoord = ivec2(gl_FragCoord.xy)+ivec2(dx, dy);"
			"		float bz = texelFetch(DepthTex, TexelCoord).x;"
			"		if(bz < 1.0)"
			"		{"
			"			sum_bz += bz;"
			"			ns += 1.0;"
			"		}"
			"	}"
			"	float DepthDiff = (sum_bz/ns)/w - gl_FragCoord.z/w;"
			"	DepthDiff =  clamp(pow(mix(1.0, 0.0, DepthDiff*4.0), 3.0), 0.0, 1.0);"
			"	vec3 Normal = normalize(vertNormal);"
			"	vec3 LightDir = normalize(vertLightDir);"
			"	float Ambi = 0.2;"
			"	float Diff  = 0.4*max(dot(LightDir, Normal)+0.1, 0.0);"
			"	fragColor = (Ambi + Diff + DepthDiff) * vertColor;"
			"}"
		).Compile();

		Program prog;
		prog.AttachShader(vs).AttachShader(fs).Link().Use();
		return std::move(prog);
	}
public:
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;


	ShapeProgram(void)
	 : Program(make())
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	{
	}
};

class ObjMeshExample : public Example
{
private:
	Context gl;

	shapes::ShapeWrapper objects;

	DepthProgram depth_prog;
	ShapeProgram draw_prog;

	static shapes::ShapeWrapper load_objects(void)
	{
		std::ifstream input;
		OpenResourceFile(input, "models", "arrow_z", ".obj");
		if(!input.good())
			throw std::runtime_error("Error opening file for reading");

		return shapes::ShapeWrapper(
			List("Position")("Normal")("Material").Get(),
			shapes::ObjMesh(
				input,
				shapes::ObjMesh::LoadingOptions(false).Normals().Materials()
			)
		);
	}

	VertexArray depth_vao, draw_vao;

	Texture depth_tex;

	GLuint width, height;
public:
	ObjMeshExample(void)
	 : gl()
	 , objects(load_objects())
	 , depth_prog()
	 , draw_prog()
	 , depth_vao(objects.VAOForProgram(depth_prog))
	 , draw_vao(objects.VAOForProgram(draw_prog))
	{
		UniformSampler(draw_prog, "DepthTex").Set(0);
		Texture::Active(0);
		depth_tex.Bind(Texture::Target::Rectangle);

		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
		gl.Viewport(width, height);

		Texture::Image2D(
			Texture::Target::Rectangle,
			0,
			PixelDataInternalFormat::DepthComponent,
			width,
			height,
			0,
			PixelDataFormat::DepthComponent,
			PixelDataType::Float,
			nullptr
		);

		auto projection =
			CamMatrixf::PerspectiveX(
				Degrees(60),
				float(width)/height,
				1, 10000
			);
		draw_prog.Use();
		draw_prog.projection_matrix.Set(projection);
		depth_prog.Use();
		depth_prog.projection_matrix.Set(projection);
	}

	void Render(double time)
	{
		gl.ClearDepth(0.0f);
		gl.Clear().DepthBuffer();

		auto camera =
			CamMatrixf::Orbiting(
				objects.BoundingSphere().Center(),
				GLfloat(objects.BoundingSphere().Radius()*2.8),
				FullCircles(time / 19.0),
				Degrees(SineWave(time / 17.0) * 90)
			);



		depth_prog.Use();
		gl.DepthFunc(CompareFn::Greater);
		gl.CullFace(Face::Front);
		depth_prog.camera_matrix.Set(camera);
		depth_prog.model_matrix.Set(Mat4f());
		objects.Draw();

		Texture::CopyImage2D(
			Texture::Target::Rectangle,
			0,
			PixelDataInternalFormat::DepthComponent,
			0, 0,
			width,
			height,
			0
		);

		gl.ClearDepth(1.0f);
		gl.Clear().ColorBuffer().DepthBuffer();

		draw_prog.Use();
		gl.DepthFunc(CompareFn::Less);
		gl.CullFace(Face::Back);
		draw_prog.camera_matrix.Set(camera);
		draw_prog.model_matrix.Set(Mat4f());

		objects.Draw();
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
	return std::unique_ptr<Example>(new ObjMeshExample);
}

} // namespace oglplus
