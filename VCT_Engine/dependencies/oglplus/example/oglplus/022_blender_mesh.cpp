/**
 *  @example oglplus/022_blender_mesh.cpp
 *  @brief Shows how to use the BlenderMesh shape generator/loader
 *
 *  @oglplus_screenshot{022_blender_mesh}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/blender_mesh.hpp>
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
public:
	Lazy<ProgramUniform<Mat4f>>
		projection_matrix,
		camera_matrix,
		model_matrix;

	DepthProgram(void)
	 : Program(ObjectDesc("Depth"))
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
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
		);
		vs.Compile();
		AttachShader(vs);

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"void main(void) { }"
		);
		fs.Compile();
		AttachShader(fs);

		Link();
	}
};

class ShapeProgram
 : public Program
{
private:
	const Program& prog(void) const { return *this; }
public:
	Lazy<ProgramUniform<Mat4f>>
		projection_matrix,
		camera_matrix,
		model_matrix;

	Lazy<ProgramUniform<Vec3f>> colors;

	ShapeProgram(void)
	 : Program(ObjectDesc("Shape"))
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , colors(prog(), "Colors")
	{
		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform mat4  ProjectionMatrix,CameraMatrix,ModelMatrix;"
			"mat3 RotMatrix = mat3(ModelMatrix);"

			"const vec3 LightPosition = vec3(12.0, 11.0, 17.0);"

			"in vec4 Position;"
			"in vec3 Normal;"
			"in float Material;"

			"out vec3 vertLightDir;"
			"out vec3 vertNormal;"
			"flat out int vertMaterial;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertNormal = RotMatrix * Normal;"
			"	vertMaterial = int(Material);"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		vs.Compile();
		AttachShader(vs);

		FragmentShader fs;
		fs.Source(
			"#version 140\n"

			"uniform sampler2DRect DepthTex;"
			"uniform vec3 Colors[3];"

			"in vec4 gl_FragCoord;"

			"in vec3 vertLightDir;"
			"in vec3 vertNormal;"
			"flat in int vertMaterial;"

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
			"	fragColor = (Ambi + Diff + DepthDiff) * Colors[vertMaterial];"
			"}"
		);
		fs.Compile();
		AttachShader(fs);

		Link();
	}
};

class BlenderMeshExample : public Example
{
private:
	Context gl;

	shapes::ShapeWrapper monkeys;

	DepthProgram depth_prog;
	ShapeProgram draw_prog;

	static shapes::ShapeWrapper load_monkeys(void)
	{
		std::ifstream input;
		OpenResourceFile(input, "models", "monkeys", ".blend");
		if(!input.good())
			throw std::runtime_error("Error opening file for reading");

		imports::BlendFile blend_file(input);

		return shapes::ShapeWrapper(
			List("Position")("Normal")("Material").Get(),
			shapes::BlenderMesh(
				blend_file,
				List("Monkey1")("Monkey2")("Monkey3").Get(),
				shapes::BlenderMesh::LoadingOptions().Nothing().Normals().Materials()
			)
		);
	}

	VertexArray depth_vao, draw_vao;

	Texture depth_tex;

	GLuint width, height;
public:
	BlenderMeshExample(void)
	 : gl()
	 , monkeys(load_monkeys())
	 , depth_prog()
	 , draw_prog()
	 , depth_vao(monkeys.VAOForProgram(depth_prog))
	 , draw_vao(monkeys.VAOForProgram(draw_prog))
	{
		UniformSampler(draw_prog, "DepthTex").Set(0);
		Texture::Active(0);
		depth_tex.Bind(Texture::Target::Rectangle);

		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
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
				1, 20
			);
		draw_prog.projection_matrix.Set(projection);
		depth_prog.projection_matrix.Set(projection);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera =
			CamMatrixf::Orbiting(
				monkeys.BoundingSphere().Center(),
				7.0,
				FullCircles(time / 19.0),
				Degrees(SineWave(time / 17.0) * 90)
			);

		depth_prog.camera_matrix.Set(camera);
		draw_prog.camera_matrix.Set(camera);


		const Vec3f colors[3][3] = {
			{
				Vec3f(1.0f, 0.3f, 0.3f),
				Vec3f(1.0f, 0.5f, 0.5f),
				Vec3f(1.0f, 0.7f, 0.7f)
			},{
				Vec3f(0.3f, 1.0f, 0.3f),
				Vec3f(0.5f, 1.0f, 0.5f),
				Vec3f(0.7f, 1.0f, 0.7f)
			},{
				Vec3f(0.3f, 0.3f, 1.0f),
				Vec3f(0.5f, 0.5f, 1.0f),
				Vec3f(0.7f, 0.7f, 1.0f)
			}
		};

		const Mat4f rot_matrices[3] = {
			ModelMatrixf::RotationA(Vec3f(1, 1, 1), Degrees(19 * time)),
			ModelMatrixf::RotationA(Vec3f(1, 1, 1), Degrees(31 * time)),
			ModelMatrixf::RotationA(Vec3f(1, 1, 1), Degrees(47 * time))
		};
		const Mat4f pos_matrices[3] = {
			ModelMatrixf::RotationY(Degrees(120))*
			ModelMatrixf::TranslationZ(2.0),
			ModelMatrixf::RotationY(Degrees(240))*
			ModelMatrixf::TranslationZ(2.0),
			ModelMatrixf::TranslationZ(2.0)
		};

		depth_prog.Use();
		gl.CullFace(Face::Front);
		monkeys.Draw(
			[this, &rot_matrices, &pos_matrices](GLuint phase) -> bool
			{
				this->depth_prog.model_matrix.Set(pos_matrices[phase]*rot_matrices[phase]);
				return true;
			}
		);

		Texture::CopyImage2D(
			Texture::Target::Rectangle,
			0,
			PixelDataInternalFormat::DepthComponent,
			0, 0,
			width,
			height,
			0
		);

		draw_prog.Use();
		gl.CullFace(Face::Back);
		monkeys.Draw(
			[this, &colors, &rot_matrices, &pos_matrices](GLuint phase) -> bool
			{
				for(GLuint i=0; i<3; ++i)
				{
					this->draw_prog.colors[i].Set(colors[phase][i]);
				}
				this->draw_prog.model_matrix.Set(pos_matrices[phase]*rot_matrices[phase]);
				return true;
			}
		);
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
	return std::unique_ptr<Example>(new BlenderMeshExample);
}

} // namespace oglplus
