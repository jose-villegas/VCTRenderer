/**
 *  @example oglplus/027_brain_scan_vis.cpp
 *  @brief Simple visualisation of volumetric brain scan data.
 *
 *  @oglplus_screenshot{027_brain_scan_vis}
 *
 *  This example requires a unsigned-byte 512x512x512 raw 3D texture
 *  file named brain_scan_512.raw. Such file can be converted for example
 *  from a medical scan stored in MINC format using the *nix minc-tools
 *  (mincinfo, mincreshape and minctoraw).
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{INITIALIZER_LISTS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/dsa/ext/buffer.hpp>
#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/ext/vertex_array.hpp>
#include <oglplus/dsa/ext/vertex_attrib.hpp>

#include <oglplus/images/gradient.hpp>
#include <oglplus/opt/resources.hpp>

#include "example.hpp"

namespace oglplus {

class RaytraceProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		VertexShader vs;
		vs.Source(
		"#version 150\n"

		"in vec4 Coord;"

		"out int vertFace;"

		"void main(void)"
		"{"
		"	gl_Position = Coord;"
		"	vertFace = gl_InstanceID;"
		"}"

		).Compile();

		GeometryShader gs;
		gs.Source(
		"#version 150\n"
		"layout (points) in;"
		"layout (triangle_strip, max_vertices=4) out;"

		"uniform mat4 ProjectionMatrix, CameraMatrix;"
		"mat4 ViewMatrix = ProjectionMatrix * CameraMatrix;"
		"uniform float CoordStep;"

		"in int vertFace[1];"

		"out vec3 geomCoord;"

		"flat out vec3 geomCO[6], geomCu[6], geomCv[6];"
		"flat out int geomFrontFace[6];"

		"const mat3 tmats[6] = mat3[6]("
		"	mat3( 0.0, 0.0, 1.0,  0.0,-1.0, 0.0,  1.0, 0.0, 0.0),"
		"	mat3( 0.0, 0.0,-1.0,  0.0,-1.0, 0.0, -1.0, 0.0, 0.0),"
		"	mat3(-1.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0, 0.0),"
		"	mat3(-1.0, 0.0, 0.0,  0.0, 0.0,-1.0,  0.0,-1.0, 0.0),"
		"	mat3( 1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0),"
		"	mat3(-1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0,-1.0) "
		");"

		"const mat3x4 mmat = mat3x4("
		"	-1.0,+1.0,-1.0,+1.0,"
		"	-1.0,-1.0,+1.0,+1.0,"
		"	+1.0,+1.0,+1.0,+1.0 "
		");"

		"const mat3x4 face_mat[6] = mat3x4[6]("
		"	mmat*tmats[0],"
		"	mmat*tmats[1],"
		"	mmat*tmats[2],"
		"	mmat*tmats[3],"
		"	mmat*tmats[4],"
		"	mmat*tmats[5] "
		");"

		"mat4x3 get_face_mat(int face)"
		"{"
		"	return transpose(face_mat[face]);"
		"}"

		"mat4 get_grid_coord(int face)"
		"{"
		"	mat4x3 fmat = get_face_mat(face)*CoordStep;"
		"	mat4 result;"
		"	for(int v=0; v!=4; ++v)"
		"	{"
		"		result[v] = vec4(gl_in[0].gl_Position.xyz+fmat[v], 1.0);"
		"	}"
		"	return result;"
		"}"

		"bool is_front_facing(mat4 ViewPos)"
		"{"
		"	vec3 ScreenPos[3] = vec3[3]("
		"		ViewPos[0].xyz/ViewPos[0].w,"
		"		ViewPos[1].xyz/ViewPos[1].w,"
		"		ViewPos[2].xyz/ViewPos[2].w "
		"	);"
		"	return cross("
		"		ScreenPos[2]-ScreenPos[0],"
		"		ScreenPos[1]-ScreenPos[0] "
		"	).z < 0.0;"
		"}"

		"void main(void)"
		"{"
		"	int face = vertFace[0];"
		"	mat4 Coord[6];"
		"	Coord[face] = get_grid_coord(face);"
		"	mat4 ViewPos = ViewMatrix * Coord[face];"
		"	if(is_front_facing(ViewPos))"
		"	{"
		"		for(int of=0; of!=6; ++of)"
		"		{"
		"			Coord[of] = get_grid_coord(of);"

		"			geomCO[of] = (Coord[of]*vec4(0.25, 0.25, 0.25, 0.25)).xyz;"
		"			geomCu[of] = (Coord[of][2].xyz-Coord[of][0].xyz)*0.5;"
		"			geomCv[of] = (Coord[of][1].xyz-Coord[of][0].xyz)*0.5;"
		"			geomFrontFace[of] = is_front_facing(ViewMatrix * Coord[of])?1:0;"
		"		}"

		"		for(int vert=0; vert!=4; ++vert)"
		"		{"
		"			gl_Position = ViewPos[vert];"
		"			geomCoord = Coord[face][vert].xyz;"
		"			EmitVertex();"
		"		}"
		"		EndPrimitive();"
		"	}"
		"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
		"#version 150\n"

		"uniform vec3 CameraPosition;"
		"uniform float CoordStep;"
		"uniform int VolumeTexSide;"
		"float SampleStep = 1.0/VolumeTexSide;"
		"uniform sampler3D VolumeTex;"
		"uniform sampler1D Palette;"
		"uniform vec4 CutoutCoord;"

		"in vec3 geomCoord;"

		"flat in vec3 geomCO[6], geomCu[6], geomCv[6];"
		"flat in int geomFrontFace[6];"

		"out vec3 fragColor;"

		"void main(void)"
		"{"
		"	float bt = 2.0;"
		"	for(int of=0; of!=6; ++of)"
		"	{"
		"		vec3 CPO = CameraPosition-geomCO[of];"
		"		vec3 VD = CameraPosition-geomCoord;"
		"		mat3 M = mat3(geomCu[of], geomCv[of], VD);"
		"		vec3 uvt = inverse(M)*CPO;"
		"		float u = abs(uvt.x);"
		"		float v = abs(uvt.y);"
		"		float t = abs(uvt.z);"
		"		if((geomFrontFace[of] == 0) && (u <= 2.0) && (v <= 2.0) && (bt > t))"
		"		{"
		"			bt = t;"
		"		}"
		"	}"
		"	if(bt <= 1.0)"
		"		bt = 1.0 + CoordStep / distance(CameraPosition, geomCoord);"
		"	vec3 backCoord = mix(CameraPosition, geomCoord, bt);"

		"	vec4 finalColor = vec4(0, 0, 0, 0);"
		"	for(int s=0; s<=VolumeTexSide; ++s)"
		"	{"
		"		vec3 Coord = mix(backCoord, geomCoord, s*SampleStep);"
		"		float Sample = texture(VolumeTex, Coord).r;"
		"		vec4 Color = texture(Palette, Sample);"
		"		float Cutout = distance(CutoutCoord.xyz, Coord)/CutoutCoord.w;"
		"		Cutout = min(floor(Cutout), 1.0);"
		"		float Enhance = 1.0+(1.0-Cutout)*0.003;"
		"		finalColor = mix(finalColor*Enhance, Color, Color.a*Cutout);"
		"	}"
		"	if(finalColor.a < 0.01)"
		"		discard;"
		"	else fragColor = finalColor.rgb;"
		"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(gs).AttachShader(fs).Link().Use();

		return std::move(prog);
	}

	Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<Vec3f> camera_position;
	ProgramUniform<Vec4f> cutout_coord;

	RaytraceProgram(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , camera_position(self(), "CameraPosition")
	 , cutout_coord(self(), "CutoutCoord")
	{ }
};

class RaytraceExample : public Example
{
private:
	Context gl;

	RaytraceProgram prog;

	DSAVertexArrayEXT grid;

	DSABufferEXT coords;

	DSATextureEXT palette, volume_tex;
	GLuint cube_side;

public:
	RaytraceExample(void)
	 : cube_side(512)
	{
		GLfloat grid_coords[3] = {0.5f, 0.5f, 0.5f};

		coords.Data(grid_coords);

		DSAVertexArrayAttribEXT(grid, prog, "Coord")
			.Setup<Vec3f>(coords)
			.Enable();

		grid.Bind();
		ProgramUniform<GLfloat>(prog, "CoordStep").Set(0.5f);

		// Volume Tex
		ProgramUniform<GLint>(prog, "VolumeTexSide").Set(cube_side);
		ProgramUniformSampler(prog, "VolumeTex").Set(0);

		volume_tex.target = Texture::Target::_3D;
		volume_tex.BindMulti(0, Texture::Target::_3D);

		std::ifstream image_file;
		OpenResourceFile(image_file, "textures", "brain_scan_512", ".raw");
		std::vector<GLubyte> image(cube_side*cube_side*cube_side);
		image_file.read(
			reinterpret_cast<char*>(image.data()),
			image.size()
		);
		volume_tex.Image3D(
			0,
			PixelDataInternalFormat::R8,
			cube_side, cube_side, cube_side,
			0,
			PixelDataFormat::Red,
			PixelDataType::UnsignedByte,
			image.data()
		);
		volume_tex.MinFilter(TextureMinFilter::Linear);
		volume_tex.MagFilter(TextureMagFilter::Linear);
		volume_tex.BorderColor(Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
		volume_tex.WrapS(TextureWrap::ClampToBorder);
		volume_tex.WrapT(TextureWrap::ClampToBorder);
		volume_tex.WrapR(TextureWrap::ClampToBorder);

		// Palette
		ProgramUniformSampler(prog, "Palette").Set(1);
		palette.target = Texture::Target::_1D;
		palette.BindMulti(1, Texture::Target::_1D);
		palette.Image1D(
			images::LinearGradient(
				256,
				Vec4f(0, 0, 0, 0),
				std::map<GLfloat, Vec4f>({
					{  0.f/256.f, Vec4f(0.0f, 0.0f, 0.0f, 0.000f)},
					{  1.f/256.f, Vec4f(0.0f, 0.0f, 0.0f, 0.000f)},
					{128.f/256.f, Vec4f(0.5f, 0.5f, 0.9f, 0.500f)},
					{196.f/256.f, Vec4f(0.5f, 1.4f, 0.6f, 0.900f)},
					{256.f/256.f, Vec4f(1.0f, 1.0f, 1.0f, 1.000f)}
				})
			)
		);
		palette.MinFilter(TextureMinFilter::Linear);
		palette.MagFilter(TextureMagFilter::Linear);
		palette.BorderColor(Vec4f(0, 0, 0, 0));
		palette.WrapS(TextureWrap::ClampToBorder);
		palette.WrapT(TextureWrap::ClampToBorder);

		gl.ClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		gl.Disable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.FrontFace(FaceOrientation::CCW);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(25),
				width, height,
				1, 20
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(0.5f, 0.5f, 0.5f),
			GLfloat(3.5 - SineWave(time / 23.0) * 0.5),
			FullCircles(time / 13.0),
			Degrees(SineWave(time / 31.0) * 85)
		);

		prog.camera_position.Set(camera.Position());
		prog.camera_matrix.Set(camera);

		Anglef phi = FullCircles(CosineWave(time / 41.0));
		Anglef the = RightAngles(CosineWave(time / 9.0));
		GLfloat rho = 0.30f;

		prog.cutout_coord.Set(
			GLfloat(0.5+rho*Sin(the)*Cos(phi)),
			GLfloat(0.5+rho*Cos(the)),
			GLfloat(0.5+rho*Sin(the)*Sin(phi)),
			GLfloat(0.20+SineWave(time / 11.0)*0.05)
		);
		gl.DrawArraysInstanced(PrimitiveType::Points, 0, 1, 6);
	}

	bool Continue(double time)
	{
		return time < 90.0;
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
	return std::unique_ptr<Example>(new RaytraceExample);
}

} // namespace oglplus
