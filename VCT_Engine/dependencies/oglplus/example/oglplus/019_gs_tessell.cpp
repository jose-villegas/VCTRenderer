/**
 *  @example oglplus/019_gs_tessell.cpp
 *  @brief Shows how to do simple tessellation with the geometry shader
 *
 *  @oglplus_screenshot{019_gs_tessell}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
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
	GeometryShader gs;
	FragmentShader fs;

	// Program
	Program prog;

	// Handles for uniforms in prog
	Uniform<GLint> tess_level;
	Uniform<Vec2f> viewport_dimensions;
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered shape
	VertexArray shape;

	// VBOs for the shape's vertices and element indices
	Buffer verts, indices;
public:
	TessellationExample(void)
	 : shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , tess_level(prog)
	 , viewport_dimensions(prog)
	 , projection_matrix(prog)
	 , camera_matrix(prog)
	 , model_matrix(prog)
	{
		vs.Source(
			"#version 150\n"

			"in vec4 Position;"

			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"}"
		);
		vs.Compile();

		gs.Source(
			"#version 150\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 48) out;"

			"const vec3 LightPosition = vec3(12.0, 10.0, 7.0);"

			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"uniform vec2 ViewportDimensions;"
			"uniform int TessLevel;"

			"noperspective out vec3 geomDist;"
			"flat out vec3 geomNormal;"
			"out vec3 geomColor;"
			"out vec3 geomLightDir;"

			"void make_triangle(vec4 p0, vec4 p1, vec4 p2)"
			"{"
			"	vec3 n0 = (ModelMatrix*vec4(p0.xyz, 0)).xyz;"
			"	vec3 n1 = (ModelMatrix*vec4(p1.xyz, 0)).xyz;"
			"	vec3 n2 = (ModelMatrix*vec4(p2.xyz, 0)).xyz;"

			"	vec4 m0 = ModelMatrix*p0;"
			"	vec4 m1 = ModelMatrix*p1;"
			"	vec4 m2 = ModelMatrix*p2;"

			"	vec4 c0 = ProjectionMatrix*CameraMatrix*m0;"
			"	vec4 c1 = ProjectionMatrix*CameraMatrix*m1;"
			"	vec4 c2 = ProjectionMatrix*CameraMatrix*m2;"

			"	vec2 s0 = ViewportDimensions * c0.xy/c0.w;"
			"	vec2 s1 = ViewportDimensions * c1.xy/c1.w;"
			"	vec2 s2 = ViewportDimensions * c2.xy/c2.w;"

			"	vec2 v0 = s2 - s1;"
			"	vec2 v1 = s0 - s2;"
			"	vec2 v2 = s1 - s0;"

			"	float d0 = abs(v1.x*v2.y-v1.y*v2.x)/length(v0);"
			"	float d1 = abs(v2.x*v0.y-v2.y*v0.x)/length(v1);"
			"	float d2 = abs(v0.x*v1.y-v0.y*v1.x)/length(v2);"

			"	geomNormal = normalize(n0+n1+n2);"

			"	gl_Position = c0;"
			"	geomColor = normalize(abs(vec3(1, 1, 1) - n0));"
			"	geomLightDir = LightPosition - m0.xyz;"
			"	geomDist = vec3(d0, 0.0, 0.0);"
			"	EmitVertex();"

			"	gl_Position = c1;"
			"	geomColor = normalize(abs(vec3(1, 1, 1) - n1));"
			"	geomLightDir = LightPosition - m1.xyz;"
			"	geomDist = vec3(0.0, d1, 0.0);"
			"	EmitVertex();"

			"	gl_Position = c2;"
			"	geomColor = normalize(abs(vec3(1, 1, 1) - n2));"
			"	geomLightDir = LightPosition - m2.xyz;"
			"	geomDist = vec3(0.0, 0.0, d2);"
			"	EmitVertex();"

			"	EndPrimitive();"
			"}"

			"void do_tess_1(vec4 p_0, vec4 p_1, vec4 p_2, int l)"
			"{"
			"	if(l == 1) make_triangle(p_0, p_1, p_2);"
			"	else"
			"	{"
			"		vec4 p01 = vec4(normalize(p_0.xyz+p_1.xyz), 1.0);"
			"		vec4 p12 = vec4(normalize(p_1.xyz+p_2.xyz), 1.0);"
			"		vec4 p20 = vec4(normalize(p_2.xyz+p_0.xyz), 1.0);"
			"		make_triangle(p_0, p01, p20);"
			"		make_triangle(p01, p_1, p12);"
			"		make_triangle(p20, p12, p_2);"
			"		make_triangle(p01, p12, p20);"
			"	}"
			"}"

			"void do_tess_0(vec4 p_0, vec4 p_1, vec4 p_2, int l)"
			"{"
			"	if(l == 0) make_triangle(p_0, p_1, p_2);"
			"	else"
			"	{"
			"		vec4 p01 = vec4(normalize(p_0.xyz+p_1.xyz), 1.0);"
			"		vec4 p12 = vec4(normalize(p_1.xyz+p_2.xyz), 1.0);"
			"		vec4 p20 = vec4(normalize(p_2.xyz+p_0.xyz), 1.0);"
			"		do_tess_1(p_0, p01, p20, l);"
			"		do_tess_1(p01, p_1, p12, l);"
			"		do_tess_1(p20, p12, p_2, l);"
			"		do_tess_1(p01, p12, p20, l);"
			"	}"
			"}"

			"void main(void)"
			"{"
			"	do_tess_0("
			"		gl_in[0].gl_Position,"
			"		gl_in[1].gl_Position,"
			"		gl_in[2].gl_Position,"
			"		TessLevel"
			"	);"
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

			"	const float Ambient = 0.7;"
			"	float Diffuse = max(dot("
			"		normalize(geomNormal),"
			"		normalize(geomLightDir)"
			"	)+0.1, 0.0)*1.4;"

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

		tess_level.BindTo("TessLevel");
		viewport_dimensions.BindTo("ViewportDimensions");
		projection_matrix.BindTo("ProjectionMatrix");
		camera_matrix.BindTo("CameraMatrix");
		model_matrix.BindTo("ModelMatrix");

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
		gl.ClearColor(0.7f, 0.7f, 0.7f, 0.0f);
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
				float(width)/height,
				1, 50
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(15.0 - SineWave(time / 13)*8.0),
			Degrees(time * 33),
			Degrees(SineWave(time / 21.0) * 31)
		);
		camera_matrix.Set(camera);

		const Vec3f offsets[4] = {
			Vec3f( 2, 0, 0),
			Vec3f(-2, 0, 0),
			Vec3f( 0, 0, 2),
			Vec3f( 0, 0,-2)
		};

		for(int i=0; i!=4; ++i)
		{
			auto model =
				ModelMatrixf::RotationX(Degrees(time * 11))*
				ModelMatrixf::Translation(offsets[i])*
				ModelMatrixf::RotationZ(Degrees(time * (37+3*i)));

			GLint level = GLint(17.0 / (Length((
				Inverse(model)*
				Vec4f(camera.Position(), 1)
			).xyz())+0.1));

			model_matrix.Set(model);
			tess_level.Set(level);

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
