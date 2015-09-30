/**
 *  @example oglplus/030_pool_tiles.cpp
 *  @brief Shows a multitextured bump-mapped reflective plane
 *
 *  @oglplus_screenshot{030_pool_tiles}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_texture{pool_pictogram}
 *  @oglplus_example_uses_texture{small_tile}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/spiral_sphere.hpp>

#include <oglplus/dsa/ext/buffer.hpp>
#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/ext/framebuffer.hpp>
#include <oglplus/dsa/ext/renderbuffer.hpp>
#include <oglplus/dsa/ext/vertex_array.hpp>
#include <oglplus/dsa/ext/vertex_attrib.hpp>

#include <oglplus/images/load.hpp>
#include <oglplus/images/random.hpp>
#include <oglplus/images/normal_map.hpp>
#include <oglplus/images/transformed.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class PoolTilesExample : public Example
{
private:
	// helper object building and storing plane drawing instructions
	shapes::Plane make_plane;
	shapes::DrawingInstructions plane_instr;
	shapes::Plane::IndexArray plane_indices;


	// helper object building and storing shape drawing instructions
	typedef shapes::SpiralSphere Shape;
	Shape make_shape;
	shapes::DrawingInstructions shape_instr;
	Shape::IndexArray shape_indices;

	Context gl;

	VertexShader plane_vs, shape_vs;

	FragmentShader plane_fs, shape_fs;

	Program plane_prog, shape_prog;

	Lazy<Uniform<Mat4f>> plane_camera_matrix, shape_camera_matrix;
	Lazy<Uniform<Vec3f>> plane_camera_position;

	DSAVertexArrayEXT plane, shape;

	DSABufferEXT plane_verts, plane_texcoords;
	DSABufferEXT shape_verts, shape_normals;

	// plane textures
	DSATextureEXT rand_tex, pict_tex, tile_tex, norm_tex;
	// Texture user for the simulation of reflection
	DSATextureEXT reflect_tex;

	DSAFramebufferEXT fbo;
	DSARenderbufferEXT rbo;

	GLuint width, height, refl_tex_side, tile_tex_side;

public:
	PoolTilesExample(void)
	 : make_plane(
		Vec3f(),
		Vec3f(7.0f, 0.0f, 0.0f),
		Vec3f(0.0f, 0.0f,-7.0f),
		48,
		48
	), plane_instr(make_plane.Instructions())
	 , plane_indices(make_plane.Indices())
	 , make_shape()
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , plane_vs(ObjectDesc("Plane vertex"))
	 , shape_vs(ObjectDesc("Shape vertex"))
	 , plane_fs(ObjectDesc("Plane fragment"))
	 , shape_fs(ObjectDesc("Shape fragment"))
	 , plane_camera_matrix(plane_prog, "CameraMatrix")
	 , shape_camera_matrix(shape_prog, "CameraMatrix")
	 , plane_camera_position(plane_prog, "CameraPosition")
	 , width(800)
	 , height(600)
	 , refl_tex_side(width > height ? height : width)
	 , tile_tex_side(64)
	{
		gl.RequireAtLeast(LimitQuery::MaxCombinedTextureImageUnits, 5);

		plane_vs.Source(
			"#version 140\n"
			"uniform vec3 LightPosition;"
			"uniform vec3 CameraPosition;"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"in vec2 TexCoord;"
			"out vec3 vertLightDir;"
			"out vec3 vertViewDir;"
			"out vec4 vertReflTexCoord;"
			"out vec2 vertTileTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix* Position;"
			"	vertLightDir = normalize(LightPosition - gl_Position.xyz);"
			"	vertViewDir = normalize(CameraPosition - gl_Position.xyz);"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"	vertReflTexCoord = gl_Position;"
			"	vertTileTexCoord = TexCoord;"
			"}"
		);
		plane_vs.Compile();

		plane_fs.Source(
			"#version 140\n"
			"uniform sampler2D RandTex, PictTex, TileTex, NormTex;"
			"uniform sampler2D ReflectTex;"
			"uniform uint TileCount;"
			"uniform float Aspect;"
			"in vec3 vertLightDir;"
			"in vec3 vertViewDir;"
			"in vec4 vertReflTexCoord;"
			"in vec2 vertTileTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec3 Normal = texture("
			"		NormTex, "
			"		vertTileTexCoord * TileCount"
			"	).rgb;"
			"	vec3 LightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		normalize(Normal)"
			"	);"
			"	float Diffuse = max(dot("
			"		Normal, "
			"		vertLightDir"
			"	), 0.0);"
			"	float Specular = max(dot("
			"		LightRefl,"
			"		vertViewDir"
			"	), 0.0);"
			"	float PlasterLight = 0.3 + max(Diffuse, 0.0);"
			"	float TileLight = 0.3 + pow(Diffuse, 2.0)*0.9 + pow(Specular, 4.0)*2.5;"
			"	vec2 ReflCoord = vertReflTexCoord.xy;"
			"	ReflCoord /= vertReflTexCoord.w;"
			"	ReflCoord *= 0.5;"
			"	ReflCoord += vec2(Aspect*0.5, 0.5);"
			"	ReflCoord += vec2(Normal.x, Normal.z)*0.5;"
			"	vec3 ReflColor = texture("
			"		ReflectTex, "
			"		ReflCoord"
			"	).rgb;"
			"	vec3 TileProps = texture("
			"		TileTex, "
			"		vertTileTexCoord * TileCount"
			"	).rgb;"
			"	float Pict = texture(PictTex, vertTileTexCoord).r;"
			"	float Rand = texture(RandTex, vertTileTexCoord).r;"
			"	float LightVsDark = "
			"		mix( 0.1, 0.9, Pict)+"
			"		mix(-0.1, 0.1, Rand);"
			"	vec3 TileColor = mix("
			"		vec3(0.1, 0.1, 0.5),"
			"		vec3(0.4, 0.4, 0.9),"
			"		LightVsDark "
			"	);"
			"	vec3 PlasterColor = vec3(0.9, 0.9, 0.9);"
			"	fragColor = vec4("
			"		mix("
			"			PlasterColor * PlasterLight,"
			"			TileColor * TileLight, "
			"			TileProps.b"
			"		) +"
			"		ReflColor * TileProps.g * 0.6,"
			"		1.0"
			"	);"
			"}"
		);
		plane_fs.Compile();

		plane_prog.AttachShader(plane_vs);
		plane_prog.AttachShader(plane_fs);
		plane_prog.Link();
		plane_prog.Use();

		Vec3f lightPos(3.0f, 2.5f, 2.0f);
		Uniform<Vec3f>(plane_prog, "LightPosition").Set(lightPos);
		Uniform<GLuint>(plane_prog, "TileCount").Set(tile_tex_side);
		Uniform<Mat4f>(plane_prog, "ModelMatrix").Set(
			ModelMatrixf::Translation(0.0f, -0.5f, 0.0f)
		);

		std::vector<GLfloat> data;
		GLuint n_per_vertex;

		n_per_vertex = make_plane.Positions(data);
		plane_verts.Data(data);
		DSAVertexArrayAttribEXT(plane, plane_prog, "Position")
			.Setup<GLfloat>(plane_verts, n_per_vertex)
			.Enable();

		n_per_vertex = make_plane.TexCoordinates(data);
		plane_texcoords.Data(data);
		DSAVertexArrayAttribEXT(plane, plane_prog, "TexCoord")
			.Setup<GLfloat>(plane_texcoords, n_per_vertex)
			.Enable();

		//
		rand_tex.target = Texture::Target::_2D;
		rand_tex.Image2D(
			images::RandomRedUByte(
				tile_tex_side,
				tile_tex_side
			)
		);
		rand_tex.Filter(TextureFilter::Nearest);
		rand_tex.Wrap(TextureWrap::Repeat);
		Texture::Active(0);
		UniformSampler(plane_prog, "RandTex").Set(0);
		rand_tex.Bind();

		//
		pict_tex.target = Texture::Target::_2D;
		pict_tex.Image2D(images::LoadTexture("pool_pictogram"));
		pict_tex.Filter(TextureFilter::Linear);
		pict_tex.Wrap(TextureWrap::Repeat);
		Texture::Active(1);
		UniformSampler(plane_prog, "PictTex").Set(1);
		pict_tex.Bind();
		//
		auto tile_image = images::LoadTexture("small_tile");
		//
		tile_tex.target = Texture::Target::_2D;
		tile_tex.Image2D(tile_image);
		tile_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
		tile_tex.MagFilter(TextureMagFilter::Linear);
		tile_tex.Wrap(TextureWrap::Repeat);
		tile_tex.GenerateMipmap();
		Texture::Active(2);
		UniformSampler(plane_prog, "TileTex").Set(2);
		tile_tex.Bind();
		//
		norm_tex.target =  Texture::Target::_2D;
		norm_tex.Image2D(
			images::TransformComponents<GLfloat, 3>(
				images::NormalMap(tile_image),
				Mat4d(
					Vec4d(1.0, 0.0, 0.0, 0.0),
					Vec4d(0.0, 0.0, 1.0, 0.0),
					Vec4d(0.0,-1.0, 0.0, 0.0),
					Vec4d(0.0, 0.0, 0.0, 1.0)
				)
			)
		);
		norm_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
		norm_tex.MagFilter(TextureMagFilter::Linear);
		norm_tex.Wrap(TextureWrap::Repeat);
		norm_tex.GenerateMipmap();
		Texture::Active(3);
		UniformSampler(plane_prog, "NormTex").Set(3);
		norm_tex.Bind();
		//
		reflect_tex.target = Texture::Target::_2D;
		reflect_tex.Image2D(
			0,
			PixelDataInternalFormat::RGB,
			refl_tex_side, refl_tex_side,
			0,
			PixelDataFormat::RGB,
			PixelDataType::UnsignedByte,
			nullptr
		);
		reflect_tex.Filter(TextureFilter::Linear);
		reflect_tex.Wrap(TextureWrap::ClampToEdge);
		Texture::Active(4);
		UniformSampler(plane_prog, "ReflectTex").Set(4);
		reflect_tex.Bind();

		rbo.Storage(
			PixelDataInternalFormat::DepthComponent,
			refl_tex_side,
			refl_tex_side
		);
		fbo.target = Framebuffer::Target::Draw;
		fbo.AttachTexture(
			FramebufferAttachment::Color,
			reflect_tex,
			0
		);
		fbo.AttachRenderbuffer(
			FramebufferAttachment::Depth,
			rbo
		);

		shape_vs.Source(
			"#version 140\n"
			"uniform vec3 LightPosition;"
			"uniform mat4 ProjectionMatrix, ModelMatrix, CameraMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"out vec3 vertNormal;"
			"out vec3 vertLightDir;"
			"out vec3 vertLightRefl;"
			"out vec3 vertViewDir;"
			"out vec3 vertViewRefl;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	gl_Position = "
			"		ModelMatrix *"
			"		Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertLightRefl = reflect("
			"		-normalize(vertLightDir),"
			"		normalize(vertNormal)"
			"	);"
			"	vertViewDir = ("
			"		vec4(0.0, 0.0, 1.0, 1.0)*"
			"		CameraMatrix"
			"	).xyz;"
			"	vertViewRefl = reflect("
			"		-normalize(vertViewDir),"
			"		normalize(vertNormal)"
			"	);"
			"	vertColor = vec3(0.3, 0.3, 0.7);"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		gl_Position;"
			"}"
		);
		shape_vs.Compile();

		shape_fs.Source(
			"#version 140\n"
			"uniform sampler2D PictTex, TileTex;"
			"uniform uint TileCount;"
			"in vec3 vertNormal;"
			"in vec3 vertLightDir;"
			"in vec3 vertLightRefl;"
			"in vec3 vertViewDir;"
			"in vec3 vertViewRefl;"
			"in vec3 vertColor;"
			"out vec4 fragColor;"

			"void main(void)"
			"{"
			"	float LtDist = length(vertLightDir);"
			"	float Diffuse = dot("
			"		normalize(vertNormal), "
			"		normalize(vertLightDir)"
			"	) / LtDist;"
			"	float Specular = dot("
			"		normalize(vertLightRefl),"
			"		normalize(vertViewDir)"
			"	);"
			"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"
			"	vec2 ReflTexCoord = -vec2("
			"		vertViewRefl.x,"
			"		vertViewRefl.z "
			"	);"
			"	ReflTexCoord *= 0.25;"
			"	ReflTexCoord += vec2(0.5, 0.5);"
			"	float Pict = texture(PictTex, ReflTexCoord).r;"
			"	float LightVsDark = mix( 0.1, 0.9, Pict);"
			"	vec3 TileColor = mix("
			"		vec3(0.2, 0.2, 0.6),"
			"		vec3(0.5, 0.5, 0.9),"
			"		LightVsDark"
			"	);"
			"	vec3 PlasterColor = vec3(0.7, 0.7, 0.7);"
			"	vec3 FloorColor = mix("
			"		PlasterColor, "
			"		TileColor, "
			"		texture(TileTex, ReflTexCoord*TileCount).b"
			"	);"
			"	vec3 ReflColor = mix("
			"		vec3(0.5, 0.5, 0.4), "
			"		FloorColor, "
			"		pow(max((-vertViewRefl.y-0.5)*2.0, 0.0), 2.0)"
			"	);"
			"	fragColor = vec4("
			"		vertColor * 0.4 + "
			"		ReflColor * 0.3 + "
			"		(LightColor + vertColor)*pow(max(2.5*Diffuse, 0.0), 3) + "
			"		LightColor * pow(max(Specular, 0.0), 64), "
			"		1.0"
			"	);"
			"}"
		);
		shape_fs.Compile();

		shape_prog.AttachShader(shape_vs);
		shape_prog.AttachShader(shape_fs);
		shape_prog.Link();
		shape_prog.Use();

		Uniform<Vec3f>(shape_prog, "LightPosition").Set(lightPos);
		Uniform<Mat4f>(shape_prog, "ModelMatrix").Set(
			ModelMatrixf::Translation(0.0f, 0.6f, 0.0f)
		);
		UniformSampler(shape_prog, "PictTex").Set(0);
		UniformSampler(shape_prog, "TileTex").Set(1);
		Uniform<GLuint>(shape_prog, "TileCount").Set(tile_tex_side);


		n_per_vertex = make_shape.Positions(data);
		shape_verts.Data(data);
		DSAVertexArrayAttribEXT(shape, shape_prog, "Position")
			.Setup<GLfloat>(shape_verts, n_per_vertex)
			.Enable();

		n_per_vertex = make_shape.Normals(data);
		shape_normals.Data(data);
		DSAVertexArrayAttribEXT(shape, shape_prog, "Normal")
			.Setup<GLfloat>(shape_normals, n_per_vertex)
			.Enable();
		//
		gl.ClearColor(0.5f, 0.5f, 0.4f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
		refl_tex_side = width > height ? height : width;

		reflect_tex.Image2D(
			0,
			PixelDataInternalFormat::RGB,
			refl_tex_side, refl_tex_side,
			0,
			PixelDataFormat::RGB,
			PixelDataType::UnsignedByte,
			nullptr
		);
		rbo.Storage(
			PixelDataInternalFormat::DepthComponent,
			refl_tex_side, refl_tex_side
		);

		float aspect = float(width)/height;
		ProgramUniform<GLfloat>(plane_prog, "Aspect").Set(aspect);

		auto projection = CamMatrixf::PerspectiveX(
			Degrees(60), aspect, 1, 60
		);
		ProgramUniform<Mat4f>(plane_prog, "ProjectionMatrix").Set(projection);
		ProgramUniform<Mat4f>(shape_prog, "ProjectionMatrix").Set(projection);
	}

	void Render(double time)
	{
		static const Mat4f reflection(
			Vec4f( 1.0, 0.0, 0.0, 0.0),
			Vec4f( 0.0,-1.0, 0.0, 0.0),
			Vec4f( 0.0, 0.0, 1.0, 0.0),
			Vec4f( 0.0, 0.0, 0.0, 1.0)
		);

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(7.0 + SineWave(time / 12.0)*2.5),
			FullCircles(time / 10.0),
			Degrees(45.0 - SineWave(time / 7.0)*35.0)
		);

		shape_prog.Use();
		shape.Bind();

		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_shape.FaceWinding());

		// render into the off-screen framebuffer
		fbo.Bind(Framebuffer::Target::Draw);
		gl.Viewport(
			(width - refl_tex_side) / 2,
			(height - refl_tex_side) / 2,
			refl_tex_side, refl_tex_side
		);
		gl.Clear().ColorBuffer().DepthBuffer();

		shape_camera_matrix.Set(
			camera *
			ModelMatrixf::Translation(0.0f, -1.0f, 0.0f) *
			reflection
		);

		gl.CullFace(Face::Front);
		shape_instr.Draw(shape_indices);

		gl.Bind(Framebuffer::Target::Draw, DefaultFramebuffer());
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();

		shape_camera_matrix.Set(camera);

		gl.CullFace(Face::Back);
		shape_instr.Draw(shape_indices);

		gl.Disable(Capability::CullFace);

		// Render the plane
		plane_prog.Use();
		plane.Bind();

		plane_camera_matrix.Set(camera);
		plane_camera_position.Set(camera.Position());

		plane_instr.Draw(plane_indices);
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
	return std::unique_ptr<Example>(new PoolTilesExample);
}

} // namespace oglplus
