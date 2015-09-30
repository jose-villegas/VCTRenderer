/**
 *  @example oglplus/020_golf_ball.cpp
 *  @brief Shows how to draw a bump mapped bouncing "golf ball"
 *
 *  @oglplus_screenshot{020_golf_ball}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_transform_feedback2}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/sphere.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class SphereExample : public Example
{
private:
	// helper object building sphere vertex attributes
	shapes::Sphere make_sphere;
	// helper object encapsulating sphere drawing instructions
	shapes::DrawingInstructions sphere_instr;
	// indices pointing to sphere primitive elements
	shapes::Sphere::IndexArray sphere_indices;

	GLuint hole_count;
	float hole_diameter;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader for the transform feedback program
	VertexShader vs_tfb;
	// Hole transforming program
	Program prog_tfb;

	// A vertex array for the holes in the sphere
	VertexArray holes;

	// VBO for the hole vectors
	Buffer hole_verts, transf_hole_verts;

	// makes vertices placed above the sphere used to adujst
	// the normals for bump-mapping
	// there vertices must be transformed just like
	// the sphere vertices
	static void make_hole_data(std::vector<GLfloat>& data, GLuint hole_count)
	{
		const GLuint ne = 5;
		const float el[ne] = {0.50f, 0.33f, 0.21f, 0.11f, 0.07f};
		const GLuint ea[ne] = {1, 6, 6, 6, 6};
		const float ao[ne] = {0.00f, 0.00f, 0.50f,-0.08f, 0.42f};
		const float si[2] = {1.0f, -1.0f};
		GLuint k = 0;

		if(ne != 0)
		{
			GLuint hn = 0;
			for(GLuint e=0; e!=ne; ++e)
				hn += ea[e];
			assert(hn * 2 == hole_count);
			data.resize(hn * 2 * 3);
		}
		for(GLuint s=0; s!= 2; ++s)
		for(GLuint e=0; e!=ne; ++e)
		{
			GLuint na = ea[e];
			if(na == 1)
			{
				data[k++] = 0.0f;
				data[k++] = si[s];
				data[k++] = 0.0f;
			}
			else if(na > 1)
			{
				float elev = float(el[e] * math::Pi());
				float a_step = 1.0f / na;
				for(GLuint a=0; a!=na; ++a)
				{
					float azim = float(si[s]*ao[e]+a*a_step*math::TwoPi());
					data[k++] = std::cos(elev)*std::cos(azim);
					data[k++] = std::sin(elev * si[s]);
					data[k++] = std::cos(elev)*std::sin(azim);
				}
			}
		}
		OGLPLUS_FAKE_USE(hole_count);
		assert(k == hole_count * 3);
		assert(k == data.size());
	}

	// Vertex shader
	VertexShader vs;
	// Fragment shader
	FragmentShader fs;
	// Program
	Program prog;

	// A vertex array object for the rendered sphere
	VertexArray sphere;

	// VBOs for the sphere's vertex attributes
	Buffer verts, normals;

public:
	SphereExample(void)
	 : sphere_instr(make_sphere.Instructions())
	 , sphere_indices(make_sphere.Indices())
	 , hole_count(50)
	 , hole_diameter(0.30f)
	{
		// This shader will be used in transform fedback mode
		// to transform the vertices used to "cut out the holes"
		// the same way the sphere is transformed
		vs_tfb.Source(
			"#version 140\n"
			"uniform mat4 CameraMatrix, ModelMatrix;"
			"uniform float Diameter;"
			"in vec3 Hole;"
			"out vec3 vertTransfHole;"
			"void main(void)"
			"{"
			"	vertTransfHole = ("
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		vec4(Hole * (1.0 + 0.5 * Diameter), 0.0)"
			"	).xyz;"
			"}"
		);
		// compile, setup transform feedback output variables
		// link and use the program
		vs_tfb.Compile();
		prog_tfb.AttachShader(vs_tfb);

		const GLchar* var_name = "vertTransfHole";
		prog_tfb.TransformFeedbackVaryings(
			1, &var_name,
			TransformFeedbackMode::InterleavedAttribs
		);
		prog_tfb.Link();
		prog_tfb.Use();

		Uniform<GLfloat> diameter(prog_tfb, "Diameter");
		diameter.Set(hole_diameter);

		// bind the VAO for the holes
		holes.Bind();

		// bind the VBO for the hole vertices
		hole_verts.Bind(Buffer::Target::Array);
		// and the VBO for the transformed hole vertices captured by tfb
		transf_hole_verts.Bind(Buffer::Target::TransformFeedback);
		{
			std::vector<GLfloat> data;
			make_hole_data(data, hole_count);
			Buffer::Data(Buffer::Target::TransformFeedback, data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexArrayAttrib attr(prog_tfb, "Hole");
			attr.Setup<Vec3f>();
			attr.Enable();
		}
		transf_hole_verts.BindBase(
			Buffer::IndexedTarget::TransformFeedback,
			0
		);

		// Set the vertex shader source
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"out vec3 vertNormal;"
			"out vec3 vertLight;"
			"const vec3 LightPos = vec3(2.0, 3.0, 3.0);"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertLight = LightPos-gl_Position.xyz;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		// compile it
		vs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 140\n"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"out vec4 fragColor;"
			"const int HoleCount = 50;"
			"uniform vec3 TransfHole[50];"
			"uniform float Diameter;"
			"void main(void)"
			"{"
			"	int imax = 0;"
			"	float dmax = -1.0;"
			"	for(int i=0; i!=HoleCount; ++i)"
			"	{"
			"		float d = dot(vertNormal, TransfHole[i]);"
			"		if(dmax < d)"
			"		{"
			"			dmax = d;"
			"			imax = i;"
			"		}"
			"	}"
			"	float l = length(vertLight);"
			"	vec3 FragDiff = TransfHole[imax] - vertNormal;"
			"	vec3 FinalNormal = "
			"		length(FragDiff) > Diameter?"
			"		vertNormal:"
			"		normalize(FragDiff+vertNormal*Diameter);"
			"	float i = (l > 0.0) ? dot("
			"		FinalNormal, "
			"		normalize(vertLight)"
			"	) / l : 0.0;"
			"	i = 0.2+max(i*2.5, 0.0);"
			"	fragColor = vec4(i, i, i, 1.0);"
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

		diameter.Set(hole_diameter);

		// bind the VAO for the sphere
		sphere.Bind();

		// bind the VBO for the sphere vertices
		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_sphere.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the sphere normals
		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_sphere.Normals(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		Uniform<Mat4f>(prog, "ProjectionMatrix").Set(
			CamMatrixf::PerspectiveX(
				Degrees(75),
				float(width)/height,
				1, 20
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		// camera matrix
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			4.5f,
			Degrees(time * 50),
			Degrees(SineWave(time / 15.0) * 70)
		);
		// model matrix
		auto model =
			ModelMatrixf::Translation(
				0.0f,
				GLfloat(sqrt(1.0f+SineWave(time / 2.0))),
				0.0f
			) *
			ModelMatrixf::RotationX(FullCircles(time));
		//
		// use transform feedback to get transformed hole vertices
		prog_tfb.Use();
		Uniform<Mat4f>(prog_tfb, "CameraMatrix").Set(camera);
		Uniform<Mat4f>(prog_tfb, "ModelMatrix").Set(model);
		holes.Bind();
		{
			TransformFeedback::Activator activates_tfb(
				TransformFeedbackPrimitiveType::Points
			);
			gl.DrawArrays(PrimitiveType::Points, 0, hole_count);
		}
		prog.Use();
		//
		Uniform<Mat4f>(prog, "CameraMatrix").Set(camera);
		Uniform<Mat4f>(prog, "ModelMatrix").Set(model);

		// map the transform feedback buffer
		BufferTypedMap<GLfloat> transf_hole_verts_map(
			Buffer::Target::TransformFeedback,
			BufferMapAccess::Read
		);
		// use the values stored in the buffer as the input
		// for the fragment shader, that will use them to
		// calculate the bump map
		Uniform<Vec3f>(prog, "TransfHole").SetValues(
			transf_hole_verts_map.Count()*3,
			transf_hole_verts_map.Data()
		);
		// bind the VAO for the spere and render it
		sphere.Bind();
		sphere_instr.Draw(sphere_indices);
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
	return std::unique_ptr<Example>(new SphereExample);
}

} // namespace oglplus
