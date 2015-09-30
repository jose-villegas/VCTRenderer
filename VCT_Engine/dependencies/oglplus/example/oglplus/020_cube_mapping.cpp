/**
 *  @example oglplus/020_cube_mapping.cpp
 *  @brief Shows how to draw an object with cube mapped surface
 *
 *  @oglplus_screenshot{020_cube_mapping}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/spiral_sphere.hpp>
#include <oglplus/images/newton.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class CubeMapExample : public Example
{
private:
	// helper object building shape vertex attributes
	shapes::SpiralSphere make_shape;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions shape_instr;
	// indices pointing to shape primitive elements
	shapes::SpiralSphere::IndexArray shape_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// Uniforms
	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered shape
	VertexArray shape;

	// VBOs for the shape's vertex attributes
	Buffer verts, normals;

	// The environment cube map
	Texture tex;
public:
	CubeMapExample(void)
	 : shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	{
		// Set the vertex shader source
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"
			"out vec3 vertNormal;"
			"out vec3 vertLightDir;"
			"out vec3 vertLightRefl;"
			"out vec3 vertViewDir;"
			"out vec3 vertViewRefl;"
			"uniform vec3 LightPos;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertLightDir = LightPos - gl_Position.xyz;"
			"	vertLightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		normalize(vertNormal)"
			"	);"
			"	vertViewDir = ("
			"		vec4(0.0, 0.0, 1.0, 1.0)*"
			"		CameraMatrix"
			"	).xyz;"
			"	vertViewRefl = reflect("
			"		normalize(vertViewDir),"
			"		normalize(vertNormal)"
			"	);"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		// compile it
		vs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 140\n"
			"uniform samplerCube TexUnit;"
			"in vec3 vertNormal;"
			"in vec3 vertLightDir;"
			"in vec3 vertLightRefl;"
			"in vec3 vertViewDir;"
			"in vec3 vertViewRefl;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float l = length(vertLightDir);"
			"	float d = dot("
			"		normalize(vertNormal), "
			"		normalize(vertLightDir)"
			"	) / l;"
			"	float s = dot("
			"		normalize(vertLightRefl),"
			"		normalize(vertViewDir)"
			"	);"
			"	vec3 lt = vec3(1.0, 1.0, 1.0);"
			"	vec3 env = texture(TexUnit, vertViewRefl).rgb;"
			"	fragColor = vec4("
			"		env * 0.4 + "
			"		(lt + env) * 1.5 * max(d, 0.0) + "
			"		lt * pow(max(s, 0.0), 64), "
			"		1.0"
			"	);"
			"}"
		);
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the shape
		shape.Bind();

		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_shape.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_shape.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// setup the texture
		{
			GLuint tex_side = 256;
			auto image = images::NewtonFractal(
				tex_side, tex_side,
				Vec3f(0.3f, 0.1f, 0.2f),
				Vec3f(1.0f, 0.8f, 0.9f),
				Vec2f(-1.0f, -1.0f),
				Vec2f( 1.0f,  1.0f),
				images::NewtonFractal::X4Minus1(),
				images::NewtonFractal::DefaultMixer()
			);
			auto tex_target = Texture::Target::CubeMap;
			// texture syntax sugar
			tex << tex_target;
			tex_target << TextureMinFilter::Linear;
			tex_target << TextureMagFilter::Linear;
			tex_target << TextureWrap::ClampToEdge;

			for(int i=0; i!=6; ++i)
			{
				Texture::CubeMapFace(i) << image;
			}
		}
		// typechecked uniform with the exact sampler type
		Typechecked<Uniform<SLtoCpp<SLDataType::SamplerCube>>>(prog, "TexUnit").Set(0);

		//
		Uniform<Vec3f>(prog, "LightPos").Set(Vec3f(3.0f, 5.0f, 4.0f));
		//
		gl.ClearColor(0.2f, 0.05f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_shape.FaceWinding());
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				float(width)/height,
				1, 100
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		// set the matrix for camera orbiting the origin
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(4.5 - SineWave(time / 16.0) * 2.0),
				FullCircles(time / 12.0),
				Degrees(SineWave(time / 30.0) * 90)
			)
		);

		// set the model matrix
		model_matrix.Set(
			ModelMatrixf::RotationA(
				Vec3f(1.0f, 1.0f, 1.0f),
				FullCircles(time / 10.0)
			)
		);

		shape.Bind();
		shape_instr.Draw(shape_indices);
	}

	bool Continue(double time)
	{
		return time < 60.0;
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
	return std::unique_ptr<Example>(new CubeMapExample);
}

} // namespace oglplus
