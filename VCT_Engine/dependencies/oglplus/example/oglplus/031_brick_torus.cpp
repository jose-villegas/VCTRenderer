/**
 *  @example oglplus/031_brick_torus.cpp
 *  @brief Shows shell and fins-based parallax mapping
 *
 *  @oglplus_screenshot{031_brick_torus}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 *  @oglplus_example_uses_texture{bricks_color_hmap}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/torus.hpp>

#include <oglplus/images/load.hpp>
#include <oglplus/images/normal_map.hpp>

#include <oglplus/opt/list_init.hpp>

#include "example.hpp"

namespace oglplus {

class ParallaxMapExample : public Example
{
private:
	Context gl;

	Program prog;
	static Program make(void)
	{
		Program prog;
		StrCRef vs_src(
			"#version 150\n"

			"uniform mat4 ModelMatrix;"

			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec3 Tangent;"
			"in vec2 TexCoord;"

			"out vec3 vertNormal;"
			"out vec3 vertTangent;"
			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = mat3(ModelMatrix) * Normal;"
			"	vertTangent = mat3(ModelMatrix) * Tangent;"
			"	vertTexCoord = vec2(4.0*TexCoord.x,2.0*TexCoord.y+TexCoord.x);"
			"}"
		);
		VertexShader vs;
		vs.Source(vs_src).Compile();
		prog.AttachShader(vs);

		StrCRef gs_src(
			"#version 150\n"
			"#extension GL_ARB_gpu_shader5 : enable\n"
			"layout(triangles, invocations = 28) in;"
			"layout(triangle_strip, max_vertices = 6) out;"

			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"mat4 Matrix = ProjectionMatrix * CameraMatrix;"
			"uniform vec3 CameraPosition;"

			"const float WindingDirection = -1;"
			"const float ShellHeight = 0.1;"

			"in vec3 vertNormal[3];"
			"in vec3 vertTangent[3];"
			"in vec2 vertTexCoord[3];"

			"out gl_PerVertex {"
			"	vec4 gl_Position;"
			"	float gl_ClipDistance[3];"
			"};"
			"flat out mat3 geomPositionFront;"
			"flat out mat3 geomNormalFront;"
			"flat out mat3 geomTangentFront;"
			"flat out mat3 geomTexCoordFront;"
			"flat out vec3 geomWFront;"

			"noperspective out vec3 geomBarycentric;"

			"out vec3 geomPosition;"
			"out vec3 geomNormal;"
			"out vec3 geomTangent;"
			"out vec3 geomTexCoord;"

			"mat3x4 get_world_pos(int id)"
			"{"
			"	mat3x4 result;"
			"	id -= 2;"
			"	if(id == -2)"
			"	{"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			result[v] = gl_in[2-v].gl_Position;"
			"		}"
			"	}"
			"	else if(id == -1)"
			"	{"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			result[v] = gl_in[v].gl_Position+"
			"				vec4(ShellHeight*vertNormal[v], 0.0);"
			"		}"
			"	}"
			"	else"
			"	{"
			"		int v = id/2;"
			"		int w = (v+1)%3;"
			"		if(id % 2 == 0)"
			"		{"
			"			result[0] = gl_in[v].gl_Position;"
			"			result[1] = gl_in[w].gl_Position;"
			"			result[2] = gl_in[v].gl_Position+"
			"				vec4(ShellHeight*vertNormal[v], 0.0);"
			"		}"
			"		else"
			"		{"
			"			result[0] = gl_in[w].gl_Position;"
			"			result[1] = gl_in[w].gl_Position+"
			"				vec4(ShellHeight*vertNormal[w], 0.0);"
			"			result[2] = gl_in[v].gl_Position+"
			"				vec4(ShellHeight*vertNormal[v], 0.0);"
			"		}"
			"	}"
			"	return result;"
			"}"

			"mat3x4 get_view_pos(mat3x4 pos)"
			"{"
			"	for(int v=0; v!=3; ++v)"
			"		pos[v] = Matrix * pos[v];"
			"	return pos;"
			"}"

			"mat3 get_screen_pos(mat3x4 pos)"
			"{"
			"	mat3 res;"
			"	for(int v=0; v!=3; ++v)"
			"		res[v] = pos[v].xyz/pos[v].w;"
			"	return res;"
			"}"

			"bool is_front_facing(mat3x4 view_pos)"
			"{"
			"	mat3 screen_pos = get_screen_pos(view_pos);"
			"	return WindingDirection * cross("
			"		screen_pos[1]-screen_pos[0],"
			"		screen_pos[2]-screen_pos[0] "
			"	).z < 0.0;"
			"}"

			"mat3 get_tex_coords(int id)"
			"{"
			"	mat3 result;"
			"	id -= 2;"
			"	if(id == -2)"
			"	{"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			result[v] = vec3(vertTexCoord[2-v], 0.0);"
			"		}"
			"	}"
			"	else if(id == -1)"
			"	{"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			result[v] = vec3(vertTexCoord[v], 1.0);"
			"		}"
			"	}"
			"	else"
			"	{"
			"		int v = id/2;"
			"		int w = (v+1)%3;"
			"		if(id % 2 == 0)"
			"		{"
			"			result[0] = vec3(vertTexCoord[v], 0.0);"
			"			result[1] = vec3(vertTexCoord[w], 0.0);"
			"			result[2] = vec3(vertTexCoord[v], 1.0);"
			"		}"
			"		else"
			"		{"
			"			result[0] = vec3(vertTexCoord[w], 0.0);"
			"			result[1] = vec3(vertTexCoord[w], 1.0);"
			"			result[2] = vec3(vertTexCoord[v], 1.0);"
			"		}"
			"	}"
			"	return result;"
			"}"

			"mat3 get_vectors(int id, vec3 attrib[3])"
			"{"
			"	mat3 result;"
			"	id -= 2;"
			"	if(id == -2)"
			"	{"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			result[v] = attrib[2-v];"
			"		}"
			"	}"
			"	else if(id == -1)"
			"	{"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			result[v] = attrib[v];"
			"		}"
			"	}"
			"	else"
			"	{"
			"		int v = id/2;"
			"		int w = (v+1)%3;"
			"		if(id % 2 == 0)"
			"		{"
			"			result[0] = attrib[v];"
			"			result[1] = attrib[w];"
			"			result[2] = attrib[v];"
			"		}"
			"		else"
			"		{"
			"			result[0] = attrib[w];"
			"			result[1] = attrib[w];"
			"			result[2] = attrib[v];"
			"		}"
			"	}"
			"	return result;"
			"}"

			"void main(void)"
			"{"
			"	int ft = gl_InvocationID/4+1;"

			"	mat3x4 world_pos_f = get_world_pos(ft);"

			"	mat3x4 view_pos_f = get_view_pos(world_pos_f);"

			"	if(!is_front_facing(view_pos_f)) return;"

			"	int bt[2];"
			"	bt[0] = (gl_InvocationID%4)*2;"
			"	bt[1] = bt[0]+1;"

			"	mat3x4 world_pos[2];"
			"	mat3x4 view_pos[2];"
			"	bool front_facing[2];"

			"	for(int b=0; b!=2; ++b)"
			"	{"
			"		if(ft == bt[b])"
			"		{"
			"			front_facing[b] = true;"
			"		}"
			"		else"
			"		{"
			"			world_pos[b] = get_world_pos(bt[b]);"
			"			view_pos[b] = get_view_pos(world_pos[b]);"
			"			front_facing[b] = is_front_facing(view_pos[b]);"
			"		}"
			"	}"

			"	if(front_facing[0] && front_facing[1]) return;"

			"	vec4 clip_plane[3];"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		int w = (v+1)%3;"
			"		vec3 p0 = world_pos_f[v].xyz;"
			"		vec3 p1 = world_pos_f[w].xyz;"
			"		vec3 p2 = CameraPosition;"
			"		vec3 pv = WindingDirection*normalize(cross(p1-p0, p2-p0));"
			"		clip_plane[v] = vec4(pv, -dot(pv, p0));"
			"	}"

			"	vec3 lo = CameraPosition;"
			"	vec3 p0 = world_pos_f[0].xyz;"
			"	vec3 pu = world_pos_f[1].xyz-p0;"
			"	vec3 pv = world_pos_f[2].xyz-p0;"
			"	vec3 lp = lo-p0;"

			"	float w0 = view_pos_f[0].w;"
			"	float wu = view_pos_f[1].w-w0;"
			"	float wv = view_pos_f[2].w-w0;"

			"	mat3 normal_f = get_vectors(ft, vertNormal);"
			"	vec3 n0 = normal_f[0];"
			"	vec3 nu = normal_f[1]-n0;"
			"	vec3 nv = normal_f[2]-n0;"

			"	mat3 tangent_f = get_vectors(ft, vertTangent);"
			"	vec3 t0 = tangent_f[0];"
			"	vec3 tu = tangent_f[1]-t0;"
			"	vec3 tv = tangent_f[2]-t0;"

			"	mat3 tex_coord_f = get_tex_coords(ft);"
			"	vec3 tc0 = tex_coord_f[0];"
			"	vec3 tcu = tex_coord_f[1]-tc0;"
			"	vec3 tcv = tex_coord_f[2]-tc0;"

			"	for(int b=0; b!=2; ++b)"
			"	{"
			"		if(front_facing[b]) continue;"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			vec3 lt = world_pos[b][v].xyz;"
			"			mat3 im = mat3(lo-lt, pu, pv);"
			"			vec3 ic = inverse(im)*lp;"
			"			float s = ic.y;"
			"			float t = ic.z;"

			"			geomPositionFront[v] = p0+pu*s+pv*t;"
			"			geomNormalFront[v] = n0+nu*s+nv*t;"
			"			geomTangentFront[v] = t0+tu*s+tv*t;"
			"			geomTexCoordFront[v] = tc0+tcu*s+tcv*t;"
			"			geomWFront[v] = w0+wu*s+wv*t;"
			"		}"
			"		mat3 normal = get_vectors(bt[b], vertNormal);"
			"		mat3 tangent = get_vectors(bt[b], vertTangent);"
			"		mat3 tex_coord = get_tex_coords(bt[b]);"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			gl_Position = view_pos[b][v];"
			"			for(int c=0; c!=3; ++c)"
			"			{"
			"				gl_ClipDistance[c] = dot("
			"					clip_plane[c],"
			"					world_pos[b][v]"
			"				);"
			"			}"
			"			geomPosition = world_pos[b][v].xyz;"
			"			geomNormal = normal[v];"
			"			geomTangent = tangent[v];"
			"			geomTexCoord = tex_coord[v];"
			"			geomBarycentric = vec3(0.0);"
			"			geomBarycentric[v] = 1.0;"

			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"	}"
			"}"
		);
		GeometryShader gs;
		gs.Source(gs_src).Compile();
		prog.AttachShader(gs);

		StrCRef fs_src(
			"#version 150\n"
			"#extension GL_EXT_gpu_shader4_1 : enable\n"

			"uniform float Time;"
			"uniform sampler2D ColorMap;"
			"uniform sampler2D BumpMap;"
			"uniform vec3 LightPosition;"

			"flat in mat3 geomPositionFront;"
			"flat in mat3 geomNormalFront;"
			"flat in mat3 geomTangentFront;"
			"flat in mat3 geomTexCoordFront;"
			"flat in vec3 geomWFront;"

			"noperspective in vec3 geomBarycentric;"

			"in vec3 geomPosition;"
			"in vec3 geomNormal;"
			"in vec3 geomTangent;"
			"in vec3 geomTexCoord;"

			"out vec3 fragColor;"

			"vec3 vcdiv(vec3 a, vec3 b)"
			"{"
			"	return vec3(a.x/b.x, a.y/b.y, a.z/b.z);"
			"}"

			"void main(void)"
			"{"
			"	const vec3 one = vec3(1.0, 1.0, 1.0);"

			"	vec3 bzfv = vcdiv(geomBarycentric,geomWFront);"
			"	float idobzfv = 1.0/dot(one,bzfv);"

			"	vec3 p0 = geomPosition;"
			"	vec3 p1 = (geomPositionFront*bzfv)*idobzfv;"

			"	vec3 n0 = geomNormal;"
			"	vec3 n1 = (geomNormalFront*bzfv)*idobzfv;"

			"	vec3 t0 = geomTangent;"
			"	vec3 t1 = (geomTangentFront*bzfv)*idobzfv;"

			"	vec3 tc0 = geomTexCoord;"
			"	vec3 tc1 = (geomTexCoordFront*bzfv)*idobzfv;"

			"	float tl = textureQueryLod(BumpMap, tc1.xy).x;"
			"	ivec2 ts = textureSize(BumpMap, int(tl));"
			"	int mts = max(ts.x, ts.y);"
			"	vec2 dtc = tc1.xy - tc0.xy;"
			"	float mdtc = max(abs(dtc.x), abs(dtc.y));"

			"	int nsam = max(min(int(mdtc*mts), mts), 1);"
			"	float step = 1.0 / nsam;"
			"	for(int s=0; s<=nsam; ++s)"
			"	{"
			"		vec3 tc = mix(tc1, tc0, s*step);"
			"		vec4 bm = texture(BumpMap, tc.xy);"
			"		if(tc.z <= bm.w+0.01)"
			"		{"
			"			vec3 p = mix(p1, p0, s*step);"
			"			vec3 n = mix(n1, n0, s*step);"
			"			vec3 t = mix(t1, t0, s*step);"
			"			vec3 b = cross(n, t);"

			"			vec3 ldir = normalize(LightPosition - p);"
			"			vec3 nml = normalize(t*bm.x+b*bm.y+n*bm.z);"
			"			float l = max(dot(ldir, nml), 0.0)*max(dot(ldir, n)+0.3, 0.0)+0.2;"
			"			fragColor = texture(ColorMap, tc.xy).rgb*l;"
			"			return;"
			"		}"
			"	}"
			"	discard;"
			"}"
		);
		FragmentShader fs;
		fs.Source(fs_src).Compile();
		prog.AttachShader(fs);

		prog.Link();
		prog.Use();

		return prog;
	}

	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	Uniform<Vec3f> camera_position, light_position;

	shapes::ShapeWrapper shape;

	Texture color_tex, bump_tex;
public:
	ParallaxMapExample(void)
	 : prog(make())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , camera_position(prog, "CameraPosition")
	 , light_position(prog, "LightPosition")
	 , shape(
		List("Position")("Normal")("Tangent")("TexCoord").Get(),
		shapes::Torus(1.0f, 0.5, 72, 48)
	)
	{
		shape.UseInProgram(prog);

		auto tex_image = images::LoadTexture("bricks_color_hmap");

		Texture::Active(0);
		try
		{
			UniformSampler(prog, "ColorMap").Set(0);
			gl.Bound(Texture::Target::_2D, color_tex)
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::Repeat)
				.Image2D(tex_image)
				.GenerateMipmap();
		}
		catch(Error&){ }

		Texture::Active(1);
		try
		{
			UniformSampler(prog, "BumpMap").Set(1);
			gl.Bound(Texture::Target::_2D, bump_tex)
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::Repeat)
				.Image2D(
					images::NormalMap(
						tex_image,
						images::NormalMap::FromAlpha()
					)
				).GenerateMipmap();
		}
		catch(Error&){ }

		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Disable(Capability::CullFace);

		gl.Enable(Functionality::ClipDistance, 0);
		gl.Enable(Functionality::ClipDistance, 1);
		gl.Enable(Functionality::ClipDistance, 2);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				float(width)/height,
				1, 20
			)
		);
	}

	void Render(ExampleClock& clock)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		double time = clock.Now().Seconds();

		auto langle = FullCircles(time / 23.0);
		light_position.Set(
			GLfloat(Cos(langle)*20.0),
			GLfloat((1.2+Sin(langle))*15.0),
			GLfloat(Sin(langle)*20.0)
		);

		double x = SineWave(time / 13.0);
		if(x+0.93 < 0.0) clock.Pace(0.2);
		else clock.Pace(1.0);

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(9.5 + x*5.1),
			FullCircles(time / 17.0),
			Degrees(SineWave(time / 20.0) * 89)
		);

		camera_matrix.Set(camera);
		camera_position.Set(camera.Position());

		model_matrix.Set(
			ModelMatrixf::TranslationX(+2.0f)*
			ModelMatrixf::RotationX(FullCircles(time / 13.0))
		);
		shape.Draw();

		model_matrix.Set(
			ModelMatrixf::TranslationX(-2.0f)*
			ModelMatrixf::RotationZ(FullCircles(time / 11.0))
		);
		shape.Draw();
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}

	double ScreenshotTime(void) const
	{
		return 9.0;
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
	return std::unique_ptr<Example>(new ParallaxMapExample);
}

} // namespace oglplus
