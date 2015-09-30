/**
 *  @example oglplus/022_volumetric_light.cpp
 *  @brief Shows how to render a volumetric light
 *
 *  @oglplus_screenshot{022_volumetric_light}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_texture{flower_glass}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/images/load.hpp>

#include "example.hpp"

namespace oglplus {

class VolLightExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shaders
	Shader volume_vs, plane_vs;
	// Gemoetry shader
	Shader volume_gs;
	// Fragment shaders
	Shader volume_fs, plane_fs;

	Program volume_prog, plane_prog;

	VertexArray volume, plane;
	Buffer volume_pos, plane_pos;

	const GLuint samples;

	Texture light_tex;
public:
	VolLightExample(void)
	 : volume_vs(ShaderType::Vertex, ObjectDesc("Volume vertex"))
	 , plane_vs(ShaderType::Vertex, ObjectDesc("Plane vertex"))
	 , volume_gs(ShaderType::Geometry, ObjectDesc("Volume geometry"))
	 , volume_fs(ShaderType::Fragment, ObjectDesc("Volume fragment"))
	 , plane_fs(ShaderType::Fragment, ObjectDesc("Plane fragment"))
	 , volume_prog(ObjectDesc("Volume"))
	 , plane_prog(ObjectDesc("Plane"))
	 , samples(150)
	{
		volume_vs.Source(
			"#version 150\n"
			"in vec4 Position;"
			"out float vertZOffs;"
			"uniform int SampleCount;"
			"uniform mat4 CameraMatrix;"
			"uniform vec3 ViewZ;"
			"uniform float Size;"
			"void main(void)"
			"{"
			"	float hp = (SampleCount-1) * 0.5;"
			"	vertZOffs = (gl_InstanceID - hp)/hp;"
			"	gl_Position = vec4("
			"		Position.xyz +"
			"		ViewZ*vertZOffs*Size*0.5,"
			"		1.0"
			"	);"
			"}"
		);
		volume_vs.Compile();

		volume_gs.Source(
			"#version 150\n"
			"layout(points) in;"
			"layout(triangle_strip, max_vertices = 4) out;"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform mat4 TexProjectionMatrix;"
			"uniform vec3 ViewX, ViewY;"
			"uniform float Size;"
			"in float vertZOffs[];"
			"out vec4 geomTexCoord;"
			"void main(void)"
			"{"
			"	float zo = vertZOffs[0];"
			"	float yo[2] = float[2](-1.0, 1.0);"
			"	float xo[2] = float[2](-1.0, 1.0);"
			"	for(int j=0;j!=2;++j)"
			"	for(int i=0;i!=2;++i)"
			"	{"
			"		vec4 v = vec4("
			"			gl_in[0].gl_Position.xyz+"
			"			ViewX * xo[i] * Size+"
			"			ViewY * yo[j] * Size,"
			"			1.0"
			"		);"
			"		geomTexCoord = "
			"			TexProjectionMatrix *"
			"			v;"
			"		gl_Position = "
			"			ProjectionMatrix *"
			"			CameraMatrix * v;"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		volume_gs.Compile();

		volume_fs.Source(
			"#version 150\n"
			"uniform sampler2D LightTex;"
			"uniform int SampleCount;"
			"in vec4 geomTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec2 coord = geomTexCoord.st/geomTexCoord.q;"
			"	float depth = geomTexCoord.z;"
			"	if(depth < 0.0) discard;"
			"	vec4 t  = texture(LightTex, coord*0.5 + 0.5);"
			"	if(t.a == 0.0) discard;"
			"	float alpha = 10.0*(1.0-t.a)/SampleCount;"
			"	alpha *= (t.r+t.g+t.b)*0.3333;"
			"	alpha /= sqrt(depth);"
			"	fragColor = vec4(t.rgb, alpha);"
			"}"
		);
		volume_fs.Compile();

		volume_prog.AttachShader(volume_vs);
		volume_prog.AttachShader(volume_gs);
		volume_prog.AttachShader(volume_fs);
		volume_prog.Link();
		volume_prog.Use();

		// bind the VAO for the volumes
		volume.Bind();

		// bind the VBO for the volume positions
		volume_pos.Bind(Buffer::Target::Array);
		{
			GLfloat position[3] = {0.0, 0.0, 0.0};
			Buffer::Data(Buffer::Target::Array, 3, position);
			VertexArrayAttrib attr(volume_prog, "Position");
			attr.Setup<Vec3f>();
			attr.Enable();
		}

		Vec3f lightPos(2.0f, 4.0f, -3.0f);
		auto texProjMat =
			CamMatrixf::PerspectiveX(Degrees(30), 1.0f, 0.3f, 20) *
			CamMatrixf::LookingAt(lightPos, Vec3f(0, 0, 0));

		Uniform<GLint>(volume_prog, "SampleCount").Set(samples);
		Uniform<GLfloat>(volume_prog, "Size").Set(Length(lightPos));
		Uniform<Mat4f>(volume_prog, "TexProjectionMatrix").Set(texProjMat);

		plane_vs.Source(
			"#version 150\n"
			"in vec4 Position;"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform mat4 TexProjectionMatrix;"
			"out vec2 vertChecker;"
			"out vec4 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		Position;"
			"	vertTexCoord = "
			"		TexProjectionMatrix *"
			"		Position;"
			"	vertChecker = Position.xz;"
			"}"
		);
		plane_vs.Compile();

		plane_fs.Source(
			"#version 150\n"
			"uniform sampler2D LightTex;"
			"in vec4 vertTexCoord;"
			"in vec2 vertChecker;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec2 coord = vertTexCoord.st/vertTexCoord.q;"
			"	vec4 t  = texture(LightTex, coord*0.5 + 0.5);"
			"	float i = ("
			"		1 +"
			"		int(vertChecker.x+10) % 2+"
			"		int(vertChecker.y+10) % 2"
			"	) % 2;"
			"	vec3 color = vec3(0.1, 0.1, 0.1);"
			"	color += t.rgb * (1.0 - t.a);"
			"	color *= mix("
			"		vec3(0.9, 0.9, 1.0), "
			"		vec3(0.4, 0.4, 0.9), "
			"		i"
			"	);"
			"	fragColor = vec4(color, 1.0);"
			"}"
		);
		plane_fs.Compile();

		plane_prog.AttachShader(plane_vs);
		plane_prog.AttachShader(plane_fs);
		plane_prog.Link();
		plane_prog.Use();

		Uniform<Mat4f>(plane_prog, "TexProjectionMatrix").Set(texProjMat);

		plane.Bind();

		// bind the VBO for the plane vertices
		plane_pos.Bind(Buffer::Target::Array);
		{
			GLfloat data[4*3] = {
				-9.0f, -4.0f,  9.0f,
				-9.0f, -4.0f, -9.0f,
				 9.0f, -4.0f,  9.0f,
				 9.0f, -4.0f, -9.0f
			};
			Buffer::Data(Buffer::Target::Array, 4*3, data);
			plane_prog.Use();
			VertexArrayAttrib attr(plane_prog, "Position");
			attr.Setup<Vec3f>();
			attr.Enable();
		}

		Texture::Active(0);
		ProgramUniformSampler(volume_prog, "LightTex").Set(0);

		light_tex
			<< Texture::Target::_2D
			<< TextureMinFilter::LinearMipmapLinear
			<< TextureMagFilter::Linear
			<< TextureWrap::ClampToBorder
			<< Vec4f(0.0f, 0.0f, 0.0f, 0.0f)
			<< images::LoadTexture("flower_glass")
			<< TextureMipmap();

		gl.ClearColor(0.0f, 0.05f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::One);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		auto perspective = CamMatrixf::PerspectiveX(
			Degrees(60),
			width, height,
			1, 40
		);
		ProgramUniform<Mat4f>(
			plane_prog,
			"ProjectionMatrix"
		).Set(perspective);
		ProgramUniform<Mat4f>(
			volume_prog,
			"ProjectionMatrix"
		).Set(perspective);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto cameraMatrix = CamMatrixf::Orbiting(
			Vec3f(0.0f, 3.0f, 0.0f),
			8.0f,
			FullCircles(time / 12.0),
			Degrees(SineWave(time / 20.0) * 80)
		);

		plane.Bind();
		plane_prog.Use();
		Uniform<Mat4f>(plane_prog, "CameraMatrix").Set(cameraMatrix);

		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);

		gl.Enable(Capability::Blend);

		volume.Bind();
		volume_prog.Use();
		Uniform<Mat4f>(volume_prog, "CameraMatrix").Set(cameraMatrix);
		Uniform<Vec3f>(volume_prog, "ViewX").Set(
			cameraMatrix.Row(0).xyz()
		);
		Uniform<Vec3f>(volume_prog, "ViewY").Set(
			cameraMatrix.Row(1).xyz()
		);
		Uniform<Vec3f>(volume_prog, "ViewZ").Set(
			cameraMatrix.Row(2).xyz()
		);
		gl.DrawArraysInstanced(
			PrimitiveType::Points,
			0, 1,
			samples
		);

		gl.Disable(Capability::Blend);
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
	return std::unique_ptr<Example>(new VolLightExample);
}

} // namespace oglplus
