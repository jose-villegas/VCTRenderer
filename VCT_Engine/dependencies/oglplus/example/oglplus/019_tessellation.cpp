/**
 *  @example oglplus/019_tessellation.cpp
 *  @brief Shows simple polyhedron tessellation
 *
 *  @oglplus_screenshot{019_tessellation}
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
#include <oglplus/shapes/icosahedron.hpp>

#include "example.hpp"

namespace oglplus {

class TessellationExample : public Example
{
private:
	// helper object building shape vertex attributes
	shapes::SimpleIcosahedron make_shape;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions shape_instr;
	// indices pointing to shape primitive elements
	shapes::SimpleIcosahedron::IndexArray shape_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Shaders
	VertexShader vs;
	TessControlShader cs;
	TessEvaluationShader es;
	GeometryShader gs;
	FragmentShader fs;

	// Program
	Program prog;

	// Uniforms
	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix, model_matrix;
	Lazy<Uniform<Vec3f>> offset, view_position;
	Lazy<Uniform<Vec2f>> viewport_dimensions;

	// A vertex array object for the rendered shape
	VertexArray shape;

	// VBOs for the shape's vertices and element indices
	Buffer verts, indices;
public:
	TessellationExample(void)
	 : shape_instr(make_shape.Instructions(PrimitiveType::Patches))
	 , shape_indices(make_shape.Indices())
	 , vs(ObjectDesc("Vertex"))
	 , cs(ObjectDesc("Tessellation Control"))
	 , es(ObjectDesc("Tessellation Evaluation"))
	 , gs(ObjectDesc("Geometry"))
	 , fs(ObjectDesc("Fragment"))
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , offset(prog, "Offset")
	 , view_position(prog, "ViewPosition")
	 , viewport_dimensions(prog, "ViewportDimensions")
	{
		vs.Source(
			"#version 150\n"

			"uniform vec3 ViewPosition;"

			"in vec3 Position;"

			"out vec3 vertPosition;"
			"out float vertDistance;"

			"void main(void)"
			"{"
			"	vertPosition = Position;"
			"	vertDistance = length(ViewPosition - vertPosition);"
			"}"
		);
		vs.Compile();

		cs.Source(
			"#version 150\n"
			"#extension ARB_tessellation_shader : enable\n"

			"layout(vertices = 3) out;"

			"in vec3 vertPosition[];"
			"in float vertDistance[];"

			"out vec3 tecoPosition[];"

			"int tessLevel(float dist)"
			"{"
			"	return int(9.0 / sqrt(dist+0.1));"
			"}"

			"void main(void)"
			"{"
			"	tecoPosition[gl_InvocationID] ="
			"		vertPosition[gl_InvocationID];"

			"	if(gl_InvocationID == 0)"
			"	{"
			"		gl_TessLevelInner[0] = tessLevel(("
			"			vertDistance[0]+"
			"			vertDistance[1]+"
			"			vertDistance[2] "
			"		)*0.333);"
			"		gl_TessLevelOuter[0] = tessLevel(("
			"			vertDistance[1]+"
			"			vertDistance[2] "
			"		)*0.5);"
			"		gl_TessLevelOuter[1] = tessLevel(("
			"			vertDistance[2]+"
			"			vertDistance[0] "
			"		)*0.5);"
			"		gl_TessLevelOuter[2] = tessLevel(("
			"			vertDistance[0]+"
			"			vertDistance[1] "
			"		)*0.5);"
			"	}"
			"}"
		);
		cs.Compile();

		es.Source(
			"#version 150\n"
			"#extension ARB_tessellation_shader : enable\n"

			"layout(triangles, equal_spacing, ccw) in;"

			"const vec3 LightPosition = vec3(12.0, 10.0, 7.0);"

			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"

			"in vec3 tecoPosition[];"

			"out vec3 teevNormal;"
			"out vec3 teevLightDir;"

			"void main(void)"
			"{"
			"	vec3 p0 = gl_TessCoord.x * tecoPosition[0];"
			"	vec3 p1 = gl_TessCoord.y * tecoPosition[1];"
			"	vec3 p2 = gl_TessCoord.z * tecoPosition[2];"

			"	vec4 tempPosition = vec4(normalize(p0+p1+p2), 0.0);"
			"	teevNormal = (ModelMatrix * tempPosition).xyz;"
			"	tempPosition.w = 1.0;"
			"	tempPosition = ModelMatrix * tempPosition;"
			"	teevLightDir = LightPosition - tempPosition.xyz;"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		tempPosition;"
			"}"
		);
		es.Compile();

		gs.Source(
			"#version 150\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 3) out;"

			"uniform vec3 Offset;"
			"uniform vec2 ViewportDimensions;"

			"in vec3 teevNormal[], teevLightDir[];"

			"noperspective out vec3 geomDist;"
			"flat out vec3 geomNormal;"
			"out vec3 geomColor;"
			"out vec3 geomLightDir;"

			"void main(void)"
			"{"
			"	geomNormal = normalize("
			"		teevNormal[0]+"
			"		teevNormal[1]+"
			"		teevNormal[2]"
			"	);"

			"	vec2 ScreenPos[3];"
			"	for(int i=0; i!=3; ++i)"
			"	{"
			"		ScreenPos[i] = "
			"			ViewportDimensions*"
			"			gl_in[i].gl_Position.xy/"
			"			gl_in[i].gl_Position.w;"
			"	}"

			"	vec2 TmpVect[3];"
			"	for(int i=0; i!=3; ++i)"
			"	{"
			"		TmpVect[i] = "
			"			ScreenPos[(i+2)%3]-"
			"			ScreenPos[(i+1)%3];"
			"	}"

			"	const vec3 EdgeMask[3] = vec3[3]("
			"		vec3(1.0, 0.0, 0.0),"
			"		vec3(0.0, 1.0, 0.0),"
			"		vec3(0.0, 0.0, 1.0) "
			"	);"

			"	for(int i=0; i!=3; ++i)"
			"	{"
			"		float Dist = abs("
			"			TmpVect[(i+1)%3].x*TmpVect[(i+2)%3].y-"
			"			TmpVect[(i+1)%3].y*TmpVect[(i+2)%3].x "
			"		) / length(TmpVect[i]);"
			"		vec3 DistVect = vec3(Dist, Dist, Dist);"

			"		gl_Position = gl_in[i].gl_Position;"
			"		geomColor = normalize(abs("
			"			vec3(2.0, 2.0, 2.0)-"
			"			teevNormal[i]-"
			"			Offset"
			"		));"
			"		geomLightDir = teevLightDir[i];"
			"		geomDist = EdgeMask[i] * DistVect;"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();

		fs.Source(
			"#version 150\n"

			"noperspective in vec3 geomDist;"
			"flat in vec3 geomNormal;"
			"in vec3 geomColor;"
			"in vec3 geomLightDir;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float MinDist = min(min(geomDist.x,geomDist.y),geomDist.z);"
			"	float EdgeAlpha = exp2(-pow(MinDist, 2.0));"

			"	const float Ambient = 0.8;"
			"	float Diffuse = max(dot("
			"		normalize(geomNormal),"
			"		normalize(geomLightDir)"
			"	), 0.0);"

			"	vec3 FaceColor = geomColor * (Diffuse + Ambient);"
			"	const vec3 EdgeColor = vec3(0.0, 0.0, 0.0);"

			"	fragColor = mix(FaceColor, EdgeColor, EdgeAlpha);"
			"}"
		);
		fs.Compile();

		prog.AttachShader(vs);
		prog.AttachShader(cs);
		prog.AttachShader(es);
		prog.AttachShader(gs);
		prog.AttachShader(fs);
		prog.Link();
		prog.Use();

		shape.Bind();

		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_shape.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();

			indices.Bind(Buffer::Target::ElementArray);
			Buffer::Data(Buffer::Target::ElementArray, shape_indices);
			shape_indices.clear();
		}

		//
		gl.ClearColor(0.8f, 0.8f, 0.8f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		prog.Use();
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		viewport_dimensions.Set(Vec2f(width, height));
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				width, height,
				1, 40
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(14.0 - SineWave(time / 13)*8.0),
			Degrees(time * 33),
			Degrees(SineWave(time / 21.0) * 31)
		);
		camera_matrix.Set(camera);

		const Vec3f offsets[6] = {
			Vec3f( 2, 0, 0),
			Vec3f(-2, 0, 0),
			Vec3f( 0, 2, 0),
			Vec3f( 0,-2, 0),
			Vec3f( 0, 0, 2),
			Vec3f( 0, 0,-2)
		};

		for(int i=0; i!=6; ++i)
		{
			auto model =
				ModelMatrixf::RotationX(Degrees(time * 11))*
				ModelMatrixf::Translation(offsets[i])*
				ModelMatrixf::RotationZ(Degrees(time * (37+9*i)));

			model_matrix.Set(model);
			offset.Set(offsets[i]);
			view_position.Set((
				Inverse(model)*
				Vec4f(camera.Position(), 1)
			).xyz());

			shape_instr.Draw(shape_indices);
		}
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
	return std::unique_ptr<Example>(new TessellationExample);
}

} // namespace oglplus
