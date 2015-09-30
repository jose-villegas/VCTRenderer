/**
 *  @example oglplus/020_icosphere.cpp
 *  @brief Shows a cube-mapped icosphere.
 *
 *  @oglplus_screenshot{020_icosphere}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 *  @oglplus_example_uses_texture{cube_0_right}
 *  @oglplus_example_uses_texture{cube_1_left}
 *  @oglplus_example_uses_texture{cube_2_top}
 *  @oglplus_example_uses_texture{cube_3_bottom}
 *  @oglplus_example_uses_texture{cube_4_front}
 *  @oglplus_example_uses_texture{cube_5_back}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/subdiv_sphere.hpp>
#include <oglplus/images/load.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class CubeMapExample : public Example
{
private:
	// helper object building shape vertex attributes
	shapes::SimpleSubdivSphere make_shape;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions shape_instr;
	// indices pointing to shape primitive elements
	shapes::SimpleSubdivSphere::IndexArray shape_indices;

	// wrapper around the current OpenGL context
	Context gl;

	VertexShader vs;
	GeometryShader gs;
	FragmentShader fs;

	Program prog;

	// Uniforms
	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered shape
	VertexArray shape;

	// VBOs for the shape's vertex attributes
	Buffer positions;

	// The environment cube map
	Texture tex;
public:
	CubeMapExample(void)
	 : make_shape(4)
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	{
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"out vec3 vertNormal;"
			"out vec3 vertTexCoord;"
			"out vec3 vertLightDir;"
			"out vec3 vertViewDir;"
			"uniform vec3 LightPos;"
			"void main(void)"
			"{"
			"	vec3 Normal = Position.xyz;"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertTexCoord = Normal;"
			"	vertLightDir = LightPos - gl_Position.xyz;"
			"	vertViewDir = (vec4(0.0, 0.0, 1.0, 1.0)*CameraMatrix).xyz;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		vs.Compile();

		gs.Source(
			"#version 150\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 3) out;"

			"in vec3 vertNormal[3];"
			"in vec3 vertTexCoord[3];"
			"in vec3 vertLightDir[3];"
			"in vec3 vertViewDir[3];"

			"out vec3 geomNormal;"
			"out vec3 geomTexCoord;"
			"out vec3 geomLightDir;"
			"out vec3 geomLightRefl;"
			"out vec3 geomViewDir;"

			"void main(void)"
			"{"
			"	vec3 FaceNormal = 0.333333*("
			"		vertNormal[0]+"
			"		vertNormal[1]+"
			"		vertNormal[2] "
			"	);"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		gl_Position = gl_in[v].gl_Position;"
			"		geomNormal = 0.5*(vertNormal[v]+FaceNormal);"
			"		geomTexCoord = vertTexCoord[v];"
			"		geomLightDir = vertLightDir[v];"
			"		geomLightRefl = reflect("
			"			-normalize(geomLightDir),"
			"			normalize(FaceNormal)"
			"		);"
			"		geomViewDir = vertViewDir[v];"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();

		fs.Source(
			"#version 150\n"
			"uniform samplerCube TexUnit;"
			"in vec3 geomNormal;"
			"in vec3 geomTexCoord;"
			"in vec3 geomLightDir;"
			"in vec3 geomLightRefl;"
			"in vec3 geomViewDir;"
			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	vec3 lt = vec3(1.0, 1.0, 1.0);"
			"	vec3 tex = texture(TexUnit, geomTexCoord).rgb;"
			"	float d = dot("
			"		normalize(geomNormal), "
			"		normalize(geomLightDir)"
			"	);"
			"	float s = dot("
			"		normalize(geomLightRefl),"
			"		normalize(geomViewDir)"
			"	);"
			"	float b = 1.0-sqrt(max(dot("
			"		normalize(geomNormal),"
			"		normalize(geomViewDir)"
			"	), 0.0));"
			"	float ea = clamp(tex.b*(-d+0.2), 0.0, 1.0);"
			"	float sr = 1.0-tex.b*0.8;"

			"	fragColor = "
			"		tex * (0.3*ea + 0.6*b + 0.8*max(d, 0.0)) + "
			"		(tex+lt) * 0.8*sr*pow(clamp(s+0.05, 0.0, 1.0), 32);"
			"}"
		);
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(gs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the shape
		shape.Bind();

		positions.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_shape.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// setup the texture
		auto tex_target = Texture::Target::CubeMap;
		tex << tex_target;
		tex_target << TextureMinFilter::Linear;
		tex_target << TextureMagFilter::Linear;
		tex_target << TextureWrap::ClampToEdge;

		const char* tex_name[6] = {
			"cube_0_right",
			"cube_1_left",
			"cube_2_top",
			"cube_3_bottom",
			"cube_4_front",
			"cube_5_back"
		};
		for(GLuint i=0; i!=6; ++i)
		{
			Texture::CubeMapFace(i) <<
				images::LoadTexture(tex_name[i], false, true);
		}

		UniformSampler(prog, "TexUnit").Set(0);
		Uniform<Vec3f>(prog, "LightPos").Set(Vec3f(3.0f, 5.0f, 4.0f));

		gl.ClearColor(0.05f, 0.2f, 0.1f, 0.0f);
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
