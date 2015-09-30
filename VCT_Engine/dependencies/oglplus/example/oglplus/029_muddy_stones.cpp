/**
 *  @example oglplus/029_muddy_stones.cpp
 *  @brief Shows shell and fins-based parallax mapping
 *
 *  @oglplus_screenshot{029_muddy_stones}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 *  @oglplus_example_uses_texture{stones_color_hmap}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/plane.hpp>

#include <oglplus/images/load.hpp>
#include <oglplus/images/normal_map.hpp>

#include <oglplus/opt/list_init.hpp>

#include "example.hpp"

namespace oglplus {

class ParallaxExample : public Example
{
private:
	Context gl;

	Program prog;

	static Program make_prog(void)
	{
		Program prog;
		VertexShader vs;
		vs.Source(StrCRef(
			"#version 150\n"
			"in vec3 Position;"
			"in vec2 TexCoord;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = vec4(Position, 1.0);"
			"	vertTexCoord = TexCoord;"
			"}"
		)).Compile();
		prog.AttachShader(vs);

		GeometryShader gs;
		gs.Source(StrCRef(
			"#version 150\n"
			"#extension GL_ARB_gpu_shader5 : enable\n"
			"layout(triangles, invocations = 7) in;"
			"layout(triangle_strip, max_vertices = 21) out;"

			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"mat4 Matrix = ProjectionMatrix * CameraMatrix;"
			"uniform vec3 CameraPosition;"

			"in vec2 vertTexCoord[3];"

			"out gl_PerVertex {"
			"	vec4 gl_Position;"
			"	float gl_ClipDistance[3];"
			"};"
			"flat out mat3 geomPositionFront;"
			"flat out mat3 geomTexCoordFront;"
			"flat out vec3 geomWFront;"
			"noperspective out vec3 geomBarycentric;"
			"out vec3 geomPosition;"
			"out vec3 geomTexCoord;"

			"void main(void)"
			"{"
			"	vec4 world_pos[8*3];"
			"	vec3 tex_coord[8*3];"

			"	vec4 view_pos[8*3];"
			"	vec3 screen_pos[8*3];"
			"	bool front_facing[8];"

			"	int ft = gl_InvocationID+1;"

			"	for(int pass=0; pass!=2; ++pass)"
			"	{"
			"		bool first = pass == 0;"
			"		if(((ft == 0) && first) || (((ft != 0) && !first)))"
			"		{"
			"			for(int v=0; v!=3; ++v)"
			"			{"
			"				int w = 2-v;"
			"				world_pos[0+v] = gl_in[w].gl_Position;"
			"				tex_coord[0+v] = vec3(vertTexCoord[w], 0.0);"
			"			}"
			"		}"

			"		vec4 n = vec4(-0.15 * normalize(cross("
			"			gl_in[1].gl_Position.xyz-gl_in[0].gl_Position.xyz,"
			"			gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz "
			"		)), 0.0);"

			"		if(((ft == 1) && first) || (((ft != 1) && !first)))"
			"		{"
			"			for(int v=0; v!=3; ++v)"
			"			{"
			"				world_pos[3+v] = gl_in[v].gl_Position + n;"
			"				tex_coord[3+v] = vec3(vertTexCoord[v], 1.0);"
			"			}"
			"		}"

			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			int w = (v+1)%3;"
			"			int k = 2+2*v;"
			"			if(((ft == k) && first) || (((ft != k) && !first)))"
			"			{"
			"				world_pos[6+0+v*6] = gl_in[v].gl_Position;"
			"				tex_coord[6+0+v*6] = vec3(vertTexCoord[v], 0.0);"
			"				world_pos[6+1+v*6] = gl_in[w].gl_Position;"
			"				tex_coord[6+1+v*6] = vec3(vertTexCoord[w], 0.0);"
			"				world_pos[6+2+v*6] = gl_in[v].gl_Position + n;"
			"				tex_coord[6+2+v*6] = vec3(vertTexCoord[v], 1.0);"
			"			}"

			"			k = 3+2*v;"
			"			if(((ft == k) && first) || (((ft != k) && !first)))"
			"			{"
			"				world_pos[6+3+v*6] = gl_in[w].gl_Position;"
			"				tex_coord[6+3+v*6] = vec3(vertTexCoord[w], 0.0);"
			"				world_pos[6+4+v*6] = gl_in[w].gl_Position + n;"
			"				tex_coord[6+4+v*6] = vec3(vertTexCoord[w], 1.0);"
			"				world_pos[6+5+v*6] = gl_in[v].gl_Position + n;"
			"				tex_coord[6+5+v*6] = vec3(vertTexCoord[v], 1.0);"
			"			}"
			"		}"

			"		for(int t=first?ft:0; t!=8; ++t)"
			"		{"
			"			if(!first && (t == ft)) continue;"
			"			int o = t*3;"
			"			for(int v=0; v!=3; ++v)"
			"			{"
			"				int w = o+v;"
			"				view_pos[w] = Matrix * world_pos[w];"
			"				screen_pos[w] = view_pos[w].xyz/view_pos[w].w;"
			"			}"
			"			front_facing[t] = cross("
			"				screen_pos[o+1]-screen_pos[o+0],"
			"				screen_pos[o+2]-screen_pos[o+0] "
			"			).z < 0.0;"
			"			if(first) break;"
			"		}"
			"		if(first && !front_facing[ft]) return;"
			"	}"

			"	int o = ft*3;"
			"	vec4 clip_plane[3];"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		int w = (v+1)%3;"
			"		vec3 p0 = world_pos[o+v].xyz;"
			"		vec3 p1 = world_pos[o+w].xyz;"
			"		vec3 p2 = CameraPosition;"
			"		vec3 pv = normalize(cross(p1-p0, p2-p0));"
			"		clip_plane[v] = vec4(pv, -dot(pv, p0));"
			"	}"
			"	vec3 lo = CameraPosition;"
			"	vec3 p0 = world_pos[o+0].xyz;"
			"	vec3 pu = world_pos[o+1].xyz-p0;"
			"	vec3 pv = world_pos[o+2].xyz-p0;"
			"	vec3 lp = lo-p0;"

			"	float w0 = view_pos[o+0].w;"
			"	float wu = view_pos[o+1].w-w0;"
			"	float wv = view_pos[o+2].w-w0;"

			"	vec3 t0 = tex_coord[o+0];"
			"	vec3 tu = tex_coord[o+1]-t0;"
			"	vec3 tv = tex_coord[o+2]-t0;"

			"	for(int bt=0; bt!=8; ++bt)"
			"	{"
			"		int k = bt*3;"
			"		if((ft != bt) && !front_facing[bt])"
			"		{"
			"			for(int v=0; v!=3; ++v)"
			"			{"
			"				vec3 lt = world_pos[k+v].xyz;"
			"				mat3 im = mat3(lo-lt, pu, pv);"
			"				vec3 ic = inverse(im)*lp;"
			"				float s = ic.y;"
			"				float t = ic.z;"

			"				geomPositionFront[v] = p0+pu*s+pv*t;"
			"				geomTexCoordFront[v] = t0+tu*s+tv*t;"
			"				geomWFront[v] = w0+wu*s+wv*t;"
			"			}"
			"			for(int v=0; v!=3; ++v)"
			"			{"
			"				int w = k+v;"
			"				gl_Position = view_pos[w];"
			"				for(int c=0; c!=3; ++c)"
			"				{"
			"					gl_ClipDistance[c] = dot("
			"						clip_plane[c],"
			"						world_pos[w]"
			"					);"
			"				}"
			"				geomPosition = world_pos[w].xyz;"
			"				geomTexCoord = tex_coord[w];"
			"				geomBarycentric = vec3(0.0);"
			"				geomBarycentric[v] = 1.0;"

			"				EmitVertex();"
			"			}"
			"			EndPrimitive();"
			"		}"
			"	}"
			"}"
		)).Compile();
		prog.AttachShader(gs);

		FragmentShader fs;
		fs.Source(StrCRef(
			"#version 150\n"

			"uniform float Time;"
			"uniform sampler2D ColorMap;"
			"uniform sampler2D BumpMap;"
			"uniform vec3 LightPosition;"

			"flat in mat3 geomPositionFront;"
			"flat in mat3 geomTexCoordFront;"
			"flat in vec3 geomWFront;"
			"noperspective in vec3 geomBarycentric;"
			"in vec3 geomPosition;"
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

			"	vec3 p0 = geomPosition;"
			"	vec3 p1 = (geomPositionFront*bzfv)/dot(one,bzfv);"
			"	vec3 tc0 = geomTexCoord;"
			"	vec3 tc1 = (geomTexCoordFront*bzfv)/dot(one,bzfv);"
			"	ivec2 ts = textureSize(BumpMap, 0);"
			"	int mts = max(ts.x, ts.y);"
			"	vec2 dtc = tc1.xy - tc0.xy;"
			"	float mdtc = max(abs(dtc.x), abs(dtc.y));"

			"	int nsam = max(min(int(mdtc*mts), mts/2), 1);"
			"	float step = 1.0 / nsam;"
			"	for(int s=0; s<=nsam; ++s)"
			"	{"
			"		vec3 tc = mix(tc1, tc0, s*step);"
			"		vec4 bm = texture(BumpMap, tc.xy);"
			"		if(tc.z <= bm.w)"
			"		{"
			"			vec3 p = mix(p1, p0, s*step);"
			"			vec3 ldir = normalize(LightPosition - p);"
			"			float l = max(dot(ldir, bm.xzy), 0.0)*1.3;"
			"			fragColor = texture(ColorMap, tc.xy).rgb*l;"
			"			return;"
			"		}"
			"	}"
			"	discard;"
			"}"
		)).Compile();
		prog.AttachShader(fs);

		prog.Link();
		prog.Use();

		return prog;
	}

	Uniform<Mat4f> projection_matrix, camera_matrix;
	Uniform<Vec3f> camera_position, light_position;

	shapes::ShapeWrapper shape;

	Texture color_tex, bump_tex;
public:
	ParallaxExample(void)
	 : prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , camera_position(prog, "CameraPosition")
	 , light_position(prog, "LightPosition")
	 , shape(
		List("Position")("TexCoord").Get(),
		shapes::Plane(
			Vec3f(),
			Vec3f(1.0f, 0.0f, 0.0f),
			Vec3f(0.0f, 0.0f,-1.0f),
			32, 32
		)
	)
	{
		shape.UseInProgram(prog);

		auto tex_image = images::LoadTexture("stones_color_hmap");

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


		(Capability::DepthTest)<< true;
		(Capability::CullFace) << false;

		(Functionality::ClipDistance|0) << true;
		(Functionality::ClipDistance|1) << true;
		(Functionality::ClipDistance|2) << true;
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				width, height,
				1, 20
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			2.9f,
			FullCircles(time / 17.0),
			Degrees(45 + SineWave(time / 20.0) * 40)
		);

		camera_matrix.Set(camera);
		camera_position.Set(camera.Position());

		auto langle = FullCircles(time / 31.0);
		light_position.Set(
			GLfloat(Cos(langle)*20.0f),
			GLfloat((1.2+Sin(langle))*15.0f),
			GLfloat(Sin(langle)*20.0f)
		);

		shape.Draw();
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
	return std::unique_ptr<Example>(new ParallaxExample);
}

} // namespace oglplus
