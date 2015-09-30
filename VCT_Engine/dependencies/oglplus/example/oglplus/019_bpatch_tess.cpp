/**
 *  @example oglplus/019_bpatch_tess.cpp
 *  @brief Shows tesellation of a cubic bezier patch
 *
 *  @oglplus_screenshot{019_bpatch_tess}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_tessellation_shader}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include "example.hpp"

namespace oglplus {

class TessellationExample : public Example
{
private:
	Context gl;

	Program prog;

	Buffer positions;

	VertexArray vao;

	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix;
public:
	TessellationExample(void)
	 : prog()
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	{
		VertexShader vert(ObjectDesc("Vertex"));
		vert.Source(
			"#version 150\n"
			"uniform mat4 CameraMatrix;"

			"in vec4 Position;"
			"out vec4 vertPosition;"

			"void main(void)"
			"{"
			"	vertPosition = CameraMatrix * Position;"
			"}"
		);
		vert.Compile();
		prog << vert;

		TessControlShader teco(ObjectDesc("TessControl"));
		teco.Source(
			"#version 150\n"
			"#extension ARB_tessellation_shader: enable\n"
			"layout(vertices = 16) out;"

			"in vec4 vertPosition[];"
			"patch out vec3 tecoPosition[16];"

			"void main(void)"
			"{"
			"	if(gl_InvocationID == 0)"
			"	{"
			"		int tl = 1-int(100.0 / vertPosition[gl_InvocationID].z);"
			"		gl_TessLevelInner[0] = tl;"
			"		gl_TessLevelInner[1] = tl;"
			"		gl_TessLevelOuter[0] = tl;"
			"		gl_TessLevelOuter[1] = tl;"
			"		gl_TessLevelOuter[2] = tl;"
			"		gl_TessLevelOuter[3] = tl;"
			"	}"
			"	tecoPosition[gl_InvocationID] = "
			"		vertPosition[gl_InvocationID].xyz;"
			"}"
		);
		teco.Compile();
		prog << teco;

		TessEvaluationShader teev(ObjectDesc("TessEvaluation"));
		teev.Source(
			"#version 150\n"
			"#extension ARB_tessellation_shader: enable\n"
			"layout(quads, equal_spacing, ccw) in;"
			"uniform mat4 ProjectionMatrix;"
			"patch in vec3 tecoPosition[16];"

			"const mat4 B = mat4("
			"	-1, 3,-3, 1,"
			"	 3,-6, 3, 0,"
			"	-3, 3, 0, 0,"
			"	 1, 0, 0, 0 "
			");"

			"mat4 Px, Py, Pz;"

			"void main(void)"
			"{"
			"	float u = gl_TessCoord.x, v = gl_TessCoord.y;"

			"	for(int j=0; j!=4; ++j)"
			"	for(int i=0; i!=4; ++i)"
			"	{"
			"		int k = j*4+i;"
			"		Px[j][i] = tecoPosition[k].x;"
			"		Py[j][i] = tecoPosition[k].y;"
			"		Pz[j][i] = tecoPosition[k].z;"
			"	}"

			"	mat4 Cx = B * Px * B;"
			"	mat4 Cy = B * Py * B;"
			"	mat4 Cz = B * Pz * B;"

			"	vec4 up = vec4(u*u*u, u*u, u, 1);"
			"	vec4 vp = vec4(v*v*v, v*v, v, 1);"

			"	vec4 tempPosition = vec4(dot(Cx * vp, up), dot(Cy * vp, up), dot(Cz * vp, up), 1.0);"

			"	gl_Position = ProjectionMatrix * tempPosition;"
			"}"
		);
		teev.Compile();
		prog << teev;

		FragmentShader frag(ObjectDesc("Fragment"));
		frag.Source(
			"#version 150\n"
			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec3(0.1, 0.1, 0.1);"
			"}"
		);
		frag.Compile();
		prog << frag;

		prog.Link();
		prog.Use();

		vao.Bind();

		GLfloat patch_cp_pos[16*3] = {
			-2.0f,  0.0f, -2.0f,
			-1.0f,  0.0f, -3.0f,
			 1.0f,  0.0f, -5.0f,
			 2.0f,  0.0f, -2.0f,
			-1.0f,  0.0f, -1.0f,
			 0.0f,  4.0f, -1.0f,
			 1.0f,  4.0f, -1.0f,
			 3.0f,  0.0f, -1.0f,
			-1.0f,  0.0f,  1.0f,
			-1.0f,  4.0f,  1.0f,
			 0.0f,  4.0f,  1.0f,
			 1.0f,  0.0f,  1.0f,
			-2.0f,  0.0f,  2.0f,
			-1.0f,  0.0f,  5.0f,
			 1.0f,  0.0f,  3.0f,
			 2.0f,  0.0f,  2.0f
		};
		positions.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, 16*3, patch_cp_pos);
		VertexArrayAttrib position_attr(prog, "Position");
		position_attr.Setup<Vec3f>();
		position_attr.Enable();

		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.PolygonMode(PolygonMode::Line);
		gl.PatchParameter(PatchParameter::PatchVertices, 16);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				width, height,
				1, 100
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(30.0 - SineWave(time / 17.0)*25.0),
				Degrees(time * 47),
				Degrees(SineWave(time / 31.0) * 90)
			)
		);

		gl.DrawArrays(PrimitiveType::Patches, 0, 16);
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
	return std::unique_ptr<Example>(new TessellationExample);
}

} // namespace oglplus
