/**
 *  @example oglplus/024_extruded_torus.cpp
 *  @brief Shows how to draw a torus with extruded surface
 *
 *  @oglplus_screenshot{024_extruded_torus}
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
#include <oglplus/shapes/torus.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class TorusExample : public Example
{
private:
	shapes::Torus make_torus;
	shapes::DrawingInstructions torus_instr;
	shapes::Torus::IndexArray torus_indices;

	// wrapper around the current OpenGL context
	Context gl;

	Program transf_prog;
	static Program make_transf_prog(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4 ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"
			"out gl_PerVertex {"
			"	vec4 gl_Position;"
			"};"
			"out vec3 vertNormal;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = (ModelMatrix*vec4(Normal,0.0)).xyz;"
			"	vertTexCoord = TexCoord;"
			"}"
		);
		vs.Compile();

		GeometryShader gs;
		gs.Source(
			"#version 150\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 15) out;"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec3 LightPos;"
			"uniform float Time;"
			"in gl_PerVertex {"
			"	vec4 gl_Position;"
			"} gl_in[];"
			"in vec3 vertNormal[];"
			"in vec2 vertTexCoord[];"
			"out gl_PerVertex {"
			"	vec4 gl_Position;"
			"};"
			"out vec3 geomNormal;"
			"out vec3 geomLight;"
			"out float geomGlow;"
			"flat out int geomTop;"
			"void main(void)"
			"{"
			"	vec3 FaceNormal = normalize("
			"		vertNormal[0]+"
			"		vertNormal[1]+"
			"		vertNormal[2] "
			"	);"
			"	vec2 FaceCoord = 0.33333 * ("
			"		vertTexCoord[0]+"
			"		vertTexCoord[1]+"
			"		vertTexCoord[2] "
			"	);"
			"	float Offs = (sin((FaceCoord.s + Time/10.0)* 3.14 * 2.0 * 10)*0.5 + 0.5)*0.4;"
			"	Offs *= cos(FaceCoord.t * 3.1415 * 2.0)*0.5 + 0.51;"

			"	vec3 pos[3], norm[3];"
			"	for(int i=0; i!=3; ++i)"
			"		pos[i] = gl_in[i].gl_Position.xyz;"
			"	for(int i=0; i!=3; ++i)"
			"		norm[i] = cross("
			"			FaceNormal, "
			"			normalize(pos[(i+1)%3] - pos[i])"
			"		);"
			"	vec3 pofs = FaceNormal * Offs;"

			"	geomTop = 0;"
			"	for(int i=0; i!=3; ++i)"
			"	{"
			"		geomNormal = norm[i];"
			"		for(int j=0; j!=2; ++j)"
			"		{"
			"			vec3 tpos = pos[(i+j)%3];"
			"			geomLight = LightPos-tpos;"
			"			geomGlow = 1.0;"
			"			gl_Position = "
			"				ProjectionMatrix *"
			"				CameraMatrix *"
			"				vec4(tpos, 1.0);"
			"			EmitVertex();"
			"			geomGlow = 0.7;"
			"			geomLight = LightPos-tpos+pofs;"
			"			gl_Position = "
			"				ProjectionMatrix *"
			"				CameraMatrix *"
			"				vec4(tpos + pofs, 1.0);"
			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"	}"

			"	geomGlow = 0.0;"
			"	geomTop = 1;"
			"	for(int i=0; i!=3; ++i)"
			"	{"
			"		geomLight = LightPos - (pos[i]+pofs);"
			"		geomNormal = vertNormal[i];"
			"		gl_Position = "
			"			ProjectionMatrix *"
			"			CameraMatrix *"
			"			vec4(pos[i] + pofs, 1.0);"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(gs);
		prog.MakeSeparable();
		prog.Link();

		ProgramUniform<Vec3f>(prog, "LightPos").Set(4, 4, -8);

		return prog;
	}

	Program face_prog;
	static Program make_face_prog(void)
	{
		FragmentShader fs;
		fs.Source(
			"#version 150\n"
			"in vec3 geomNormal;"
			"in vec3 geomLight;"
			"in float geomGlow;"
			"flat in int geomTop;"
			"uniform vec3 TopColor, SideColor;"
			"const vec3 LightColor = vec3(1.0, 1.0, 1.0);"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = max(dot("
			"		normalize(geomLight),"
			"		normalize(geomNormal)"
			"	), 0.0);"
			"	vec3 color;"
			"	if(geomTop != 0)"
			"	{"
			"		color = TopColor * d +"
			"			LightColor * pow(d, 8.0);"
			"	}"
			"	else"
			"	{"
			"		color = SideColor * geomGlow +"
			"			LightColor *"
			"			pow(d, 2.0) * 0.2;"
			"	}"
			"	fragColor = vec4(color, 1.0);"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(fs);
		prog.MakeSeparable();
		prog.Link();

		ProgramUniform<Vec3f>(prog, "TopColor").Set(0.2f, 0.2f, 0.2f);
		ProgramUniform<Vec3f>(prog, "SideColor").Set(0.9f, 0.9f, 0.2f);

		return prog;
	}

	Program frame_prog;
	static Program make_frame_prog(void)
	{
		FragmentShader fs;
		fs.Source(
			"#version 150\n"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(0.2, 0.1, 0.0, 1.0);"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(fs);
		prog.MakeSeparable();
		prog.Link();

		return prog;
	}

	ProgramPipeline face_pp, frame_pp;

	// Uniforms
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	ProgramUniform<GLfloat> transf_time;

	// A vertex array object for the rendered torus
	VertexArray torus;

	// VBOs for the torus's vertices, normals and st coordinates
	Buffer verts, normals, texcoords;
public:
	TorusExample(void)
	 : make_torus(1.0, 0.5, 18, 36)
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , transf_prog(make_transf_prog())
	 , face_prog(make_face_prog())
	 , frame_prog(make_frame_prog())
	 , projection_matrix(transf_prog, "ProjectionMatrix")
	 , camera_matrix(transf_prog, "CameraMatrix")
	 , model_matrix(transf_prog, "ModelMatrix")
	 , transf_time(transf_prog, "Time")
	{
		transf_prog.Use();
		torus.Bind();
		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);

			VertexArrayAttrib attr(transf_prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);

			VertexArrayAttrib attr(transf_prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.TexCoordinates(data);
			Buffer::Data(Buffer::Target::Array, data);

			VertexArrayAttrib attr(transf_prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		face_pp.Bind();
		face_prog.Use();
		face_pp.UseStages(transf_prog).Vertex().Geometry();
		face_pp.UseStages(face_prog).Fragment();


		frame_pp.Bind();
		frame_prog.Use();
		frame_pp.UseStages(transf_prog).Vertex().Geometry();
		frame_pp.UseStages(frame_prog).Fragment();

		gl.Bind(NoProgramPipeline());
		gl.Use(NoProgram());

		gl.ClearColor(0.7f, 0.6f, 0.5f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.DepthFunc(CompareFn::Less);
		gl.FrontFace(make_torus.FaceWinding());
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 30
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			4.5,
			Degrees(time * 35),
			Degrees(SineWave(time / 30.0) * 60)
		);

		auto model =
			ModelMatrixf::RotationY(FullCircles(time * 0.25)) *
			ModelMatrixf::RotationX(FullCircles(time * 0.33));

		camera_matrix.Set(camera);
		model_matrix.Set(model);
		transf_time.Set(GLfloat(time));

		face_pp.Bind();
		gl.PolygonMode(PolygonMode::Fill);
		torus_instr.Draw(torus_indices);

		frame_pp.Bind();
		gl.PolygonMode(PolygonMode::Line);
		torus_instr.Draw(torus_indices);
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
	return std::unique_ptr<Example>(new TorusExample);
}

} // namespace oglplus
