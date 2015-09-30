/**
 *  @example oglplus/023_sky.cpp
 *  @brief Shows how to render a dynamic sky using a sky box
 *
 *  @oglplus_screenshot{023_sky}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_texture{clouds01-cm_0}
 *  @oglplus_example_uses_texture{clouds01-cm_1}
 *  @oglplus_example_uses_texture{clouds01-cm_2}
 *  @oglplus_example_uses_texture{clouds01-cm_3}
 *  @oglplus_example_uses_texture{clouds01-cm_4}
 *  @oglplus_example_uses_texture{clouds01-cm_5}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/images/load.hpp>

#include <oglplus/shapes/wicker_torus.hpp>
#include <oglplus/shapes/sky_box.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/opt/list_init.hpp>

#include "example.hpp"

namespace oglplus {

class SkyBoxExample : public Example
{
private:
	shapes::ShapeWrapper sky_box;
	shapes::ShapeWrapper shape;

	Context gl;

	Program sky_box_prog, shape_prog;

	Lazy<ProgramUniform<Mat4f>>
		sky_box_projection_matrix,
		sky_box_camera_matrix,
		shape_projection_matrix,
		shape_camera_matrix,
		shape_model_matrix;

	Lazy<ProgramUniform<Vec3f>>
		sky_box_sun_position,
		shape_sun_position,
		shape_camera_position;

	Texture env_map;
public:
	SkyBoxExample(void)
	 : sky_box(List("Position").Get(), shapes::SkyBox())
	 , shape(List("Position")("Normal").Get(), shapes::WickerTorus())
	 , sky_box_prog()
	 , sky_box_projection_matrix(sky_box_prog, "ProjectionMatrix")
	 , sky_box_camera_matrix(sky_box_prog, "CameraMatrix")
	 , shape_projection_matrix(shape_prog, "ProjectionMatrix")
	 , shape_camera_matrix(shape_prog, "CameraMatrix")
	 , shape_model_matrix(shape_prog, "ModelMatrix")
	 , sky_box_sun_position(sky_box_prog, "SunPosition")
	 , shape_sun_position(shape_prog, "SunPosition")
	 , shape_camera_position(shape_prog, "CameraPosition")
	{
		VertexShader sky_box_vs;
		sky_box_vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"mat4 Matrix = ProjectionMatrix*CameraMatrix;"

			"in vec3 Position;"
			"out vec3 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = Matrix * vec4(Position * 100.0, 1.0);"
			"	vertTexCoord = Position;"
			"}"
		);
		sky_box_vs.Compile();
		sky_box_prog.AttachShader(sky_box_vs);

		VertexShader shape_vs;
		shape_vs.Source(
			"#version 140\n"

			"uniform vec3 SunPosition;"
			"uniform vec3 CameraPosition;"
			"uniform mat4  ProjectionMatrix,CameraMatrix,ModelMatrix;"

			"in vec4 Position;"
			"in vec3 Normal;"

			"out vec3 vertNormal, vertViewRefl, vertLightDir;"

			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertViewRefl = reflect("
			"		gl_Position.xyz - CameraPosition,"
			"		vertNormal"
			"	);"
			"	vertLightDir = SunPosition - gl_Position.xyz;"
			"	gl_Position = ProjectionMatrix*CameraMatrix*gl_Position;"
			"}"
		);
		shape_vs.Compile();
		shape_prog.AttachShader(shape_vs);

		FragmentShader sky_box_fs;
		sky_box_fs.Source(
			"#version 140\n"

			"in vec3 vertTexCoord;"

			"out vec3 fragColor;"

			"vec3 sky_color(vec3 vd);"

			"void main(void)"
			"{"
			"	fragColor = sky_color(normalize(vertTexCoord));"
			"}"
		);
		sky_box_fs.Compile();
		sky_box_prog.AttachShader(sky_box_fs);

		FragmentShader shape_fs;
		shape_fs.Source(
			"#version 140\n"

			"in vec3 vertNormal, vertViewRefl, vertLightDir;"

			"out vec3 fragColor;"

			"vec3 sky_color(vec3 vd);"

			"void main(void)"
			"{"
			"	float l = max(dot(normalize(vertNormal), normalize(vertLightDir))+0.1, 0.0);"
			"	float a = 0.1;"
			"	fragColor = "
			"		0.1*vec3(1.0, 1.0, 1.0)*(a+l)+"
			"		0.9*sky_color(normalize(vertViewRefl));"
			"}"
		);
		shape_fs.Compile();
		shape_prog.AttachShader(shape_fs);

		FragmentShader sky_fs;
		sky_fs.Source(
			"#version 140\n"

			"const float WorldRadius = 6371000;"
			"const float AtmThickness = 50000;"
			"const vec3 AirColor = vec3(0.32, 0.36, 0.45);"
			"const vec3 LightColor = vec3(1.0, 1.0, 1.0);"

			"uniform vec3 SunPosition;"

			"uniform samplerCube EnvMap;"

			"float atm_intersection(vec3 v)"
			"{"
			"	const vec3 c = vec3(0.0, -WorldRadius, 0.0);"
			"	const float r = WorldRadius + AtmThickness;"
			"	const float c_c = dot(-c, -c);"
			"	float v_c = dot( v, -c);"
			"	return (-v_c + sqrt(v_c*v_c - c_c + r*r))/AtmThickness;"
			"}"

			"vec3 sky_color(vec3 vd)"
			"{"
			"	vec3 up = vec3(0.0, 1.0, 0.0);"
			"	vec3 ld = normalize(SunPosition);"
			"	vec4 cl = texture(EnvMap, vd);"
			"	float ai = atm_intersection(vd);"
			"	float al = max(dot(ld, up) + 0.12, 0.0);"
			"	float vl = max(dot(vd, ld), 0.0);"
			"	float ct = (1.0-cl.a)*cl.b;"
			"	vec3 ac = max(LightColor-AirColor*pow(ai, 0.33), vec3(0.0, 0.0, 0.0));"

			"	vec3 Sun = "
			"		ac*(vl>0.995+0.004*al ? 1.0:0.0);"

			"	vec3 Air = "
			"		min(AirColor*sqrt(pow(al,0.25)*ai), vec3(al, al, al)*1.5)+"
			"		ac*pow(min(vl+0.001*ai, 1.0), 1024.0/pow(ai, 2.0))+"
			"		ac*(vl/(1.0+pow(3.0*al, 8.0)))*pow(ai, 0.6)*0.5;"

			"	vec3 Clouds ="
			"		ac*pow(min(vl*(cl.g+cl.b), 1.015), 64.0)*pow(ct, 2.0)+"
			"		ac*pow(min(vl*cl.g+cl.b, 1.020), 32.0)*ct+"
			"		ac*pow(min(vl*cl.g*cl.b, 1.010), 16.0)*pow(ct, 0.5)+"
			"		ac*0.7*min(cl.g + cl.b*0.5, 1.0)*al+"
			"		ac*(cl.g*(1.0-cl.b*0.2)*5.0)*pow(1.0-al, 2.0)*(al)+"
			"		LightColor*0.5*min(al + cl.g*0.4+cl.b*0.1, 1.0)*sqrt(al);"

			"	return mix(Air, Clouds, cl.a*(1.0-cl.r*0.8))+Sun*(1.0-cl.a);"
			"}"
		);
		sky_fs.Compile();
		sky_box_prog.AttachShader(sky_fs);
		shape_prog.AttachShader(sky_fs);

		sky_box_prog.Link();
		sky_box.UseInProgram(sky_box_prog);

		shape_prog.Link();
		shape.UseInProgram(shape_prog);

		{
			ProgramUniformSampler(sky_box_prog, "EnvMap").Set(0);
			ProgramUniformSampler(shape_prog, "EnvMap").Set(0);
			Texture::Active(0);

			gl.Bound(Texture::Target::CubeMap, env_map)
				.MinFilter(TextureMinFilter::Linear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::ClampToEdge)
				.WrapT(TextureWrap::ClampToEdge)
				.WrapR(TextureWrap::ClampToEdge);

			Texture::ImageCM(0, images::LoadTexture("clouds01-cm_0", false, false));
			Texture::ImageCM(1, images::LoadTexture("clouds01-cm_1", false, false));
			Texture::ImageCM(2, images::LoadTexture("clouds01-cm_2", false, false));
			Texture::ImageCM(3, images::LoadTexture("clouds01-cm_3", false, false));
			Texture::ImageCM(4, images::LoadTexture("clouds01-cm_4", false, false));
			Texture::ImageCM(5, images::LoadTexture("clouds01-cm_5", false, false));
		}

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		auto projection = CamMatrixf::PerspectiveX(
			Degrees(70),
			float(width)/height,
			1, 200
		);
		sky_box_projection_matrix.Set(projection);
		shape_projection_matrix.Set(projection);
	}

	void Render(double time)
	{
		const double day_duration = 67.0;
		auto sun =
			Vec3f( 0.000, 1.000, 0.000)*1e10*SineWave(time / day_duration) +
			Vec3f( 0.000, 0.000,-1.000)*1e10*CosineWave(time / day_duration);


		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			5.0,
			FullCircles(-0.10-time / 27.0),
			Degrees(-20 - SineWave(time / 17.0)*30)
		);

		auto model = ModelMatrixf::RotationA(
			Vec3f(1.0, 1.0, 1.0),
			FullCircles(time / 13.0)
		);

		gl.Clear().ColorBuffer().DepthBuffer();

		sky_box.Use();
		sky_box_prog.Use();
		sky_box_camera_matrix.Set(camera);
		sky_box_sun_position.Set(sun);
		sky_box.Draw();

		shape.Use();
		shape_prog.Use();
		shape_model_matrix.Set(model);
		shape_camera_matrix.Set(camera);
		shape_camera_position.Set(camera.Position());
		shape_sun_position.Set(sun);
		shape.Draw();
	}

	bool Continue(double time)
	{
		return time < 180.0;
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
	return std::unique_ptr<Example>(new SkyBoxExample);
}

} // namespace oglplus
