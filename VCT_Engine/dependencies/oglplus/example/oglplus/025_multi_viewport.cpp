/**
 *  @example oglplus/025_multi_viewport.cpp
 *  @brief Shows how to draw into multiple viewport at the same time
 *
 *  @oglplus_screenshot{025_multi_viewport}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_viewport_array}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/spiral_sphere.hpp>
#include <oglplus/images/newton.hpp>
#include <oglplus/bound/texture.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class MultiViewportExample : public Example
{
private:
	typedef shapes::SpiralSphere Shape;
	// helper object building shape vertex attributes
	Shape make_shape;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions shape_instr;
	// indices pointing to shape primitive elements
	Shape::IndexArray shape_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;

	static Program make(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4 ModelMatrix;"
			"uniform vec3 LightPos;"

			"in vec4 Position;"
			"in vec3 Normal;"

			"out vec3 vertNormal;"
			"out vec3 vertTexCoord;"
			"out vec3 vertLightDir;"
			"out vec3 vertLightRefl;"

			"void main(void)"
			"{"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertTexCoord = Normal;"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = LightPos-gl_Position.xyz;"
			"	vertLightRefl = reflect(-vertLightDir, vertNormal);"
			"}"
		);

		GeometryShader gs;
		gs.Source(
			"#version 150\n"
			"#extension GL_ARB_viewport_array : enable\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 12) out;"

			"uniform mat4 CameraMatrix[4];"
			"uniform vec3 CameraPosition[4];"

			"in vec3 vertNormal[];"
			"in vec3 vertTexCoord[];"
			"in vec3 vertLightDir[];"
			"in vec3 vertLightRefl[];"

			"out vec3 geomNormal;"
			"out vec3 geomTexCoord;"
			"out vec3 geomLightDir;"
			"out vec3 geomLightRefl;"
			"out vec3 geomViewDir;"
			"out vec3 geomViewRefl;"

			"void main(void)"
			"{"
			"	for(int vp=0; vp!=4; ++vp)"
			"	{"
			"		gl_ViewportIndex = vp;"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			geomNormal = vertNormal[v];"
			"			geomTexCoord = vertTexCoord[v];"
			"			geomLightDir = vertLightDir[v];"
			"			geomLightRefl = vertLightRefl[v];"
			"			geomViewDir = "
			"				CameraPosition[vp] - "
			"				gl_in[v].gl_Position.xyz;"
			"			geomViewRefl = reflect("
			"				-geomViewDir,"
			"				geomNormal"
			"			);"
			"			gl_Position = "
			"				CameraMatrix[vp] *"
			"				gl_in[v].gl_Position;"
			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"	}"
			"}"
		);

		FragmentShader fs;
		fs.Source(
			"#version 150\n"
			"uniform samplerCube TexUnit;"
			"in vec3 geomNormal;"
			"in vec3 geomTexCoord;"
			"in vec3 geomLightDir;"
			"in vec3 geomLightRefl;"
			"in vec3 geomViewDir;"
			"in vec3 geomViewRefl;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float l = length(geomLightDir);"
			"	float d = dot("
			"		normalize(geomNormal), "
			"		normalize(geomLightDir)"
			"	) / l;"
			"	float s = dot("
			"		normalize(geomLightRefl),"
			"		normalize(geomViewDir)"
			"	);"
			"	vec3 lt = vec3(1.0, 1.0, 1.0);"
			"	vec3 tex = texture(TexUnit, geomTexCoord).rgb;"
			"	fragColor = vec4("
			"		tex * 0.4 + "
			"		(lt + tex) * 1.5 * max(d, 0.0) + "
			"		lt * pow(max(s, 0.0), 64), "
			"		1.0"
			"	);"
			"}"
		);

		Program prog;
		prog.AttachShaders(MakeGroup(vs,gs,fs));
		prog.Build();
		prog.Use();
		return prog;
	}

	// Handles for setting uniforms in program
	Uniform<Vec3f> camera_position_3;
	Uniform<Mat4f>
		camera_matrix_0,
		camera_matrix_1,
		camera_matrix_2,
		camera_matrix_3,
		model_matrix;


	// A vertex array object for the rendered shape
	VertexArray shape;

	// VBOs for the shape's vertex attributes
	Buffer verts, normals;

	// The environment cube map
	Texture tex;

	Mat4f projection;
public:
	MultiViewportExample(void)
	 : make_shape(1.0, 0.1, 8, 4, 48)
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , prog(make())
	 , camera_position_3(prog, "CameraPosition[3]")
	 , camera_matrix_0(prog, "CameraMatrix[0]")
	 , camera_matrix_1(prog, "CameraMatrix[1]")
	 , camera_matrix_2(prog, "CameraMatrix[2]")
	 , camera_matrix_3(prog, "CameraMatrix[3]")
	 , model_matrix(prog, "ModelMatrix")
	{
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
			GLuint tex_side = 512;
			auto image = images::NewtonFractal(
				tex_side, tex_side,
				Vec3f(0.8f, 0.8f, 1.0f),
				Vec3f(0.1f, 0.0f, 0.2f),
				Vec2f(-0.707f, -0.707f),
				Vec2f( 0.707f,  0.707f),
				images::NewtonFractal::X4Minus1(),
				[](double x) -> double
				{
					return pow(SineWave(pow(x,0.5)), 4.0);
				}
			);

			gl.Bound(Texture::Target::CubeMap, tex)
				.MinFilter(TextureMinFilter::Linear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::ClampToEdge)
				.WrapT(TextureWrap::ClampToEdge)
				.WrapR(TextureWrap::ClampToEdge);

			for(int i=0; i!=6; ++i)
				Texture::ImageCM(i, image);
		}
		//
		UniformSampler(prog, "TexUnit").Set(0);
		Uniform<Vec3f>(prog, "LightPos").Set(Vec3f(3.0f, 5.0f, 4.0f));
		//
		gl.ClearColor(0.1f, 0.05f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_shape.FaceWinding());
		gl.CullFace(Face::Back);

		Uniform<Vec3f> camera_position(prog, "CameraPosition");
		camera_position[0].Set(Vec3f(2, 0, 0));
		camera_position[1].Set(Vec3f(0, 2, 0));
		camera_position[2].Set(Vec3f(0, 0, 2));
	}

	void Reshape(GLuint width, GLuint height)
	{
		GLfloat w_2f = width / 2.0f;
		GLfloat h_2f = height/ 2.0f;
		GLfloat viewports[4*4] = {
			0.0f, 0.0f, w_2f, h_2f,
			w_2f, 0.0f, w_2f, h_2f,
			0.0f, h_2f, w_2f, h_2f,
			w_2f, h_2f, w_2f, h_2f
		};
		gl.ViewportArray(0, 4, viewports);

		projection = CamMatrixf::PerspectiveX(
			Degrees(65),
			float(width)/height,
			1, 30
		);

		camera_matrix_0.Set(
			projection *
			Mat4f(
				 Vec4f(0, 0,-1, 0),
				 Vec4f(0, 1, 0, 0),
				 Vec4f(1, 0, 0,-3),
				 Vec4f(0, 0, 0, 1)
			)
		);
		camera_matrix_1.Set(
			projection *
			Mat4f(
				 Vec4f(1, 0, 0, 0),
				 Vec4f(0, 0,-1, 0),
				 Vec4f(0, 1, 0,-3),
				 Vec4f(0, 0, 0, 1)
			)
		);
		camera_matrix_2.Set(
			projection *
			Mat4f(
				 Vec4f(1, 0, 0, 0),
				 Vec4f(0, 1, 0, 0),
				 Vec4f(0, 0, 1,-3),
				 Vec4f(0, 0, 0, 1)
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		CamMatrixf camera = CamMatrixf::Orbiting(
			Vec3f(),
			4.5f - float(SineWave(time / 16.0)) * 1.5f,
			FullCircles(time / 12.0),
			Degrees(SineWave(time / 30.0) * 90)
		);
		// set the matrix for camera orbiting the origin
		camera_matrix_3.Set(projection * camera);
		camera_position_3.Set(camera.Position());

		// set the model matrix
		model_matrix.Set(
			ModelMatrixf::RotationX(FullCircles(time / 10.0))
		);

		shape_instr.Draw(shape_indices);
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}

	double ScreenshotTime(void) const
	{
		return 3.0;
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
	return std::unique_ptr<Example>(new MultiViewportExample);
}

} // namespace oglplus
