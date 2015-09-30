/**
 *  @example oglplus/026_clouds.cpp
 *  @brief Shows how to render dynamically lighted clouds using a 3d texture
 *
 *  @oglplus_screenshot{026_clouds}
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

#include <oglplus/images/cloud.hpp>
#include <oglplus/shapes/sphere.hpp>

#include <vector>

#include "example.hpp"

namespace oglplus {

class CloudExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	VertexShader light_vs, cloud_vs;

	GeometryShader cloud_gs;

	FragmentShader light_fs, cloud_fs;

	Program light_prog, cloud_prog;

	shapes::Sphere make_sphere;
	shapes::DrawingInstructions sphere_instr;
	shapes::Sphere::IndexArray sphere_indices;

	VertexArray light;
	Buffer buffer;

	VertexArray clouds;
	Buffer pos_buffer, size_buffer;

	const GLuint samples;

	const std::vector<Vec3f> positions;
	static std::vector<Vec3f> make_positions(void)
	{
		const Vec3f _positions[3] = {
			Vec3f(-1.4f, -0.3f, -0.7f),
			Vec3f(-1.6f, +0.4f, +0.7f),
			Vec3f(+0.6f, -0.1f, +0.1f)
		};
		return std::vector<Vec3f>(_positions, _positions+3);
	}

	const std::vector<GLfloat> sizes;
	static std::vector<GLfloat> make_sizes(void)
	{
		const GLfloat _sizes[3] = {1.0f, 1.2f, 1.5f};
		return std::vector<GLfloat>(_sizes, _sizes+3);
	}

	Array<Texture> cloud_tex;

	CubicBezierLoop<Vec3f, double> light_path;
	static std::vector<Vec3f> make_light_path_cps(void)
	{
		const Vec3f _cps[6] = {
			Vec3f(-3.0f, -3.0f, -3.0f),
			Vec3f( 0.0f,  0.0f,  0.0f),
			Vec3f( 3.0f,  3.0f,  3.0f),
			Vec3f( 3.0f, -3.0f, -3.0f),
			Vec3f( 0.0f,  0.0f,  0.0f),
			Vec3f(-3.0f,  3.0f,  3.0f)
		};
		return std::vector<Vec3f>(_cps, _cps+6);
	}
public:
	CloudExample(const ExampleParams& params)
	 : sphere_instr(make_sphere.Instructions())
	 , sphere_indices(make_sphere.Indices())
	 , samples(GLuint(25 + params.quality*params.quality*100))
	 , positions(make_positions())
	 , sizes(make_sizes())
	 , cloud_tex(positions.size())
	 , light_path(make_light_path_cps())
	{
		assert(positions.size() == sizes.size());
		std::srand(123456);

		light_vs.Source(
			"#version 150\n"
			"in vec3 Position;"
			"uniform vec3 LightPos;"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"void main(void)"
			"{"
			"	float s = 0.1;"
			"	gl_Position = "
			"		ProjectionMatrix*"
			"		CameraMatrix*"
			"		vec4(Position*s + LightPos, 1.0);"
			"}"
		).Compile();

		light_fs.Source(
			"#version 150\n"
			"out vec4 fragLight;"
			"void main(void)"
			"{"
			"	fragLight = vec4(1.0, 1.0, 1.0, 1.0);"
			"}"
		).Compile();

		light_prog << light_vs << light_fs;
		light_prog.Link().Use();

		light.Bind();

		buffer.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_sphere.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			(light_prog|"Position").Setup<GLfloat>(n_per_vertex).Enable();
		}

		cloud_vs.Source(
			"#version 150\n"
			"in vec4 Position;"
			"in float Size;"
			"uniform int SampleCount;"
			"uniform mat4 CameraMatrix;"
			"uniform vec4 ViewZ;"
			"out float vertZOffs;"
			"out float vertSize;"
			"void main(void)"
			"{"
			"	float hp = (SampleCount-1) * 0.5;"
			"	vertZOffs = (gl_InstanceID - hp)/hp;"
			"	vertSize = Size;"
			"	gl_Position = vec4("
			"		Position.xyz +"
			"		ViewZ.xyz*vertZOffs*Size*0.5,"
			"		1.0"
			"	);"
			"}"
		).Compile();

		cloud_gs.Source(
			"#version 150\n"
			"layout(points) in;"
			"layout(triangle_strip, max_vertices = 4) out;"
			"in float vertZOffs[];"
			"in float vertSize[];"
			"uniform vec3 LightPos;"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec4 ViewX, ViewY, ViewZ;"
			"out vec3 geomTexCoord;"
			"out vec3 geomLightDir;"
			"void main(void)"
			"{"
			"	float zo = vertZOffs[0];"
			"	float s = vertSize[0];"
			"	float yo[2] = float[2](-1.0, 1.0);"
			"	float xo[2] = float[2](-1.0, 1.0);"
			"	for(int j=0;j!=2;++j)"
			"	for(int i=0;i!=2;++i)"
			"	{"
			"		vec4 v = vec4("
			"			gl_in[0].gl_Position.xyz+"
			"			ViewX.xyz * xo[i] * s * 0.5+"
			"			ViewY.xyz * yo[j] * s * 0.5,"
			"			1.0"
			"		);"
			"		gl_Position = ProjectionMatrix * CameraMatrix * v;"
			"		geomLightDir = LightPos - v.xyz;"
			"		geomTexCoord = "
			"			vec3(0.5, 0.5, 0.5)+"
			"			ViewX.xyz*(xo[i])*0.707+"
			"			ViewY.xyz*(yo[j])*0.707+"
			"			ViewZ.xyz*(zo   )*0.707;"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		).Compile();

		cloud_fs.Source(
			"#version 150\n"
			"uniform sampler3D CloudTex;"
			"in vec3 geomTexCoord;"
			"in vec3 geomLightDir;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = texture(CloudTex, geomTexCoord).r;"
			"	float o = 1.0;"
			"	float s = 2.0/128.0;"
			"	float r = s * 8.0;"
			"	vec3 sampleOffs = normalize(geomLightDir) * s;"
			"	vec3 samplePos = geomTexCoord;"
			"	if(d > 0.01) while(o > 0.0)"
			"	{"
			"		if(samplePos.x<0.0 || samplePos.x>1.0)"
			"			break;"
			"		if(samplePos.y<0.0 || samplePos.y>1.0)"
			"			break;"
			"		if(samplePos.z<0.0 || samplePos.z>1.0)"
			"			break;"
			"		o -= texture(CloudTex, samplePos).r*r;"
			"		samplePos += sampleOffs;"
			"	}"
			"	float a = 0.4 * d;"
			"	float i = mix(0.2, 1.0, o);"
			"	fragColor = vec4(i, i, i, a);"
			"}"
		).Compile();

		cloud_prog << cloud_vs << cloud_gs << cloud_fs;
		cloud_prog.Link().Use();

		// bind the VAO for the clouds
		clouds.Bind();

		// bind the VBO for the cloud positions
		pos_buffer.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, positions);
			(cloud_prog|"Position").Setup<Vec3f>().Enable();
		}

		// bind the VBO for the cloud sizes
		size_buffer.Bind(Buffer::Target::Array);
		{
			Buffer::Data(Buffer::Target::Array, sizes);
			(cloud_prog|"Size").Setup<GLfloat>().Enable();
		}

		// set the number of samples
		cloud_prog/"SampleCount" = GLint(samples);

		Texture::Active(0);
		cloud_prog/"CloudTex" = 0;
		for(std::size_t i=0, n=positions.size(); i!=n; ++i)
		{
			cloud_tex[i]
				<< Texture::Target::_3D
				<< images::Cloud(
					128, 128, 128,
					Vec3f(0.1f, -0.5f, 0.3f),
					0.5f
				)
				<< TextureMipmap()
				<< TextureMinFilter::LinearMipmapLinear
				<< TextureMagFilter::Linear
				<< TextureWrap::ClampToBorder
				<< Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
		}

		gl.ClearColor(0.0f, 0.1f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Disable(Capability::DepthTest);
		gl.Enable(Capability::Blend);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		Mat4f perspective = CamMatrixf::PerspectiveX(
			Degrees(65),
			width, height,
			1, 40
		);
		ProgramUniform<Mat4f>(cloud_prog, "ProjectionMatrix").Set(perspective);
		ProgramUniform<Mat4f>(light_prog, "ProjectionMatrix").Set(perspective);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto lightPos = light_path.Position(time * 0.05);
		auto cameraMatrix = CamMatrixf::Orbiting(
			Vec3f(),
			4.5f,
			Degrees(0),
			Degrees(SineWave(time / 20.0) * 80)
		);

		light.Bind();
		light_prog.Use();

		Uniform<Vec3f>(light_prog, "LightPos").Set(lightPos);
		Uniform<Mat4f>(light_prog, "CameraMatrix").Set(cameraMatrix);

		sphere_instr.Draw(sphere_indices);

		clouds.Bind();
		cloud_prog.Use();

		Uniform<Vec3f>(cloud_prog, "LightPos").Set(lightPos);
		Uniform<Mat4f>(cloud_prog, "CameraMatrix").Set(cameraMatrix);
		Uniform<Vec4f>(cloud_prog, "ViewX").Set(cameraMatrix.Row(0));
		Uniform<Vec4f>(cloud_prog, "ViewY").Set(cameraMatrix.Row(1));
		Uniform<Vec4f>(cloud_prog, "ViewZ").Set(cameraMatrix.Row(2));
		for(std::size_t i=0, n=positions.size(); i!=n; ++i)
		{
			cloud_tex[i].Bind(Texture::Target::_3D);
			gl.DrawArraysInstanced(
				PrimitiveType::Points,
				GLint(i), 1,
				samples
			);
		}
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

std::unique_ptr<Example> makeExample(const ExampleParams& params)
{
	return std::unique_ptr<Example>(new CloudExample(params));
}

} // namespace oglplus
