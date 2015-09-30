/**
 *  @example oglplus/017_single_pass_edges.cpp
 *  @brief Shows how render the faces and the wireframe in single pass
 *
 *  @oglplus_screenshot{017_single_pass_edges}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/subdiv_sphere.hpp>

#include "example.hpp"

namespace oglplus {

class SinglePassEdgeExample : public Example
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

	// Vertex shader
	VertexShader vs;

	// Geometry shader
	GeometryShader gs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// Uniforms
	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix, model_matrix;
	Lazy<Uniform<Vec2f>> viewport_dimensions;
	Lazy<Uniform<GLfloat>> edge_width;

	// A vertex array object for the rendered shape
	VertexArray shape;

	// VBOs for the shape's vertices and element indices
	Buffer verts, indices;
public:
	SinglePassEdgeExample(void)
	 : make_shape(1, shapes::SubdivSphereInitialShape::Octohedron)
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , vs(ObjectDesc("Vertex"))
	 , gs(ObjectDesc("Geometry"))
	 , fs(ObjectDesc("Fragment"))
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , viewport_dimensions(prog, "ViewportDimensions")
	 , edge_width(prog, "EdgeWidth")
	{
		vs.Source(
			"#version 150\n"

			"const vec3 LightPosition = vec3(10.0, 10.0, 7.0);"

			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"

			"in vec4 Position;"

			"out vec3 vertNormal;"
			"out vec3 vertLightDir;"

			"void main(void)"
			"{"
			"	gl_Position = "
			"		ModelMatrix *"
			"		Position;"
			"	vertNormal = ("
			"		ModelMatrix *"
			"		vec4(Position.xyz, 0.0)"
			"	).xyz;"
			"	vertLightDir = "
			"		LightPosition -"
			"		gl_Position.xyz;"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		gl_Position;"
			"}"
		);
		vs.Compile();

		gs.Source(
			"#version 150\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 3) out;"

			"uniform vec2 ViewportDimensions;"

			"in vec3 vertNormal[], vertLightDir[];"

			"noperspective out vec3 geomDist;"
			"flat out vec3 geomNormal;"
			"flat out vec3 geomColor;"
			"out vec3 geomLightDir;"

			"void main(void)"
			"{"
			"	geomNormal = normalize("
			"		vertNormal[0]+"
			"		vertNormal[1]+"
			"		vertNormal[2]"
			"	);"
			"	geomColor = normalize(abs("
			"		vec3(1.0, 1.0, 1.0)-"
			"		geomNormal"
			"	));"

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
			"		geomLightDir = vertLightDir[i];"
			"		geomDist = EdgeMask[i] * DistVect;"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();

		fs.Source(
			"#version 150\n"

			"uniform float EdgeWidth;"

			"noperspective in vec3 geomDist;"
			"flat in vec3 geomNormal;"
			"flat in vec3 geomColor;"
			"in vec3 geomLightDir;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	float MinDist = min(min(geomDist.x,geomDist.y),geomDist.z);"
			"	float EdgeAlpha = exp2(-pow(MinDist/EdgeWidth, 2.0));"

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

		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
		gl.FrontFace(make_shape.FaceWinding());

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
				1, 20
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		edge_width.Set(GLfloat(4.0+SineWave(time / 7.0)*3.0));

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(5.5 - SineWave(time / 27)*2.0),
				Degrees(time * 33),
				Degrees(SineWave(time / 21.0) * 31)
			)
		);

		model_matrix.Set(ModelMatrixf::RotationZ(Degrees(time * 37)));

		shape_instr.Draw(shape_indices);
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
	return std::unique_ptr<Example>(new SinglePassEdgeExample);
}

} // namespace oglplus
