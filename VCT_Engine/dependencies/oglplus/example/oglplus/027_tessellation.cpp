/**
 *  @example oglplus/027_tessellation.cpp
 *  @brief Shows tesellation of a bezier disc
 *
 *  @oglplus_screenshot{027_tessellation}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_tessellation_shader}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <vector>

#include "example.hpp"

namespace oglplus {

class TessellationExample : public Example
{
private:
	Context gl;

	Program prog;

	Buffer positions, next_cps, prev_cps, tess_levels;

	VertexArray vao;

	Lazy<Uniform<Mat4f>> projection_matrix, camera_matrix;
	Lazy<Uniform<Vec2f>> viewport_dimensions;

	GLuint segments;
	GLuint instances;
	GLfloat step;
public:
	TessellationExample(void)
	 : prog()
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , viewport_dimensions(prog, "ViewportDimensions")
	 , segments(8)
	 , instances(24)
	 , step(10.0f)
	{
		VertexShader vert(ObjectDesc("Vertex"));
		vert.Source(StrCRef(
			"#version 150\n"
			"uniform mat4 CameraMatrix;"

			"uniform float Step;"
			"uniform int InstCount;"

			"in vec4 Position;"
			"in vec3 PrevCP, NextCP;"
			"in vec2 TessLevel;"

			"out vec4 vertPosition;"
			"out vec3 vertPrevCP, vertNextCP;"
			"out vec3 vertColor;"
			"out vec2 vertTessLevel;"

			"void main(void)"
			"{"
			"	float XOffs = gl_InstanceID*Step-(InstCount*Step)*0.5;"
			"	vertColor = normalize(normalize(vec3(1,1,1))-normalize(Position.xyz));"
			"	vertPosition = CameraMatrix * (Position+vec4(XOffs, 0, 0, 0));"
			"	vertPrevCP = mat3(CameraMatrix)*PrevCP;"
			"	vertNextCP = mat3(CameraMatrix)*NextCP;"
			"	vertTessLevel = TessLevel;"
			"}"
		));
		vert.Compile();
		prog << vert;

		TessControlShader teco(ObjectDesc("TessControl"));
		teco.Source(StrCRef(
			"#version 150\n"
			"#extension ARB_tessellation_shader : enable\n"
			"#define ID gl_InvocationID\n"
			"layout(vertices = 4) in;"
			"layout(vertices = 4) out;"

			"in vec4 vertPosition[];"
			"in vec3 vertPrevCP[], vertNextCP[];"
			"in vec3 vertColor[];"
			"in vec2 vertTessLevel[];"

			"patch out vec3 tecoPosition[4], tecoPrevCP[4], tecoNextCP[4], tecoColor[4];"

			"float calc_tl(vec3 v)"
			"{"
			"	float d = pow(dot(vec3(0.0, 0.0,-1.0), normalize(v)), 3);"
			"	return clamp(d/pow(abs(v.z)*0.1+1.5, 1.5), 0.0, 1.0);"
			"}"

			"vec3 mid(int id1, int id2)"
			"{"
			"	return mix(vertPosition[id1].xyz, vertPosition[id2].xyz, 0.5);"
			"}"

			"void main(void)"
			"{"
			"	int NextID = (ID+2)%4;"
			"	int PrevID = (ID+3)%4;"
			"	if(ID == 0 || ID == 1)"
			"	{"
			"		gl_TessLevelInner[ID] = int("
			"			(vertTessLevel[ID].y+vertTessLevel[NextID].y)*"
			"			calc_tl(mix(mid(0,2), mid(1,3), 0.5))"
			"		);"
			"	}"
			"	gl_TessLevelOuter[ID] = 1+int(vertTessLevel[PrevID].x*calc_tl(mid(PrevID, ID)));"
			"	tecoPosition[ID] = vertPosition[ID].xyz;"
			"	tecoPrevCP[ID] = vertPrevCP[ID];"
			"	tecoNextCP[ID] = vertNextCP[ID];"
			"	tecoColor[ID] = vertColor[ID];"
			"}"
		));
		teco.Compile();
		prog << teco;

		TessEvaluationShader teev(ObjectDesc("TessEvaluation"));
		teev.Source(StrCRef(
			"#version 150\n"
			"#extension ARB_tessellation_shader : enable\n"
			"layout(quads, equal_spacing, ccw) in;"
			"uniform mat4 ProjectionMatrix;"

			"patch in vec3 tecoPosition[4], tecoPrevCP[4], tecoNextCP[4], tecoColor[4];"

			"out vec4 teevPatchDistance;"
			"out vec3 teevColor;"

			"vec3 teevPosition[16] = vec3[16]("
			"	tecoPosition[0],"
			"	tecoPosition[0]+tecoNextCP[0],"
			"	tecoPosition[1]+tecoPrevCP[1],"
			"	tecoPosition[1],"

			"	tecoPosition[0]+tecoPrevCP[0],"
			"	tecoPosition[0]+tecoPrevCP[0]+tecoNextCP[0],"
			"	tecoPosition[1]+tecoPrevCP[1]+tecoNextCP[1],"
			"	tecoPosition[1]+tecoNextCP[1],"

			"	tecoPosition[3]+tecoNextCP[3],"
			"	tecoPosition[3]+tecoPrevCP[3]+tecoNextCP[3],"
			"	tecoPosition[2]+tecoPrevCP[2]+tecoNextCP[2],"
			"	tecoPosition[2]+tecoPrevCP[2],"

			"	tecoPosition[3],"
			"	tecoPosition[3]+tecoPrevCP[3],"
			"	tecoPosition[2]+tecoNextCP[2],"
			"	tecoPosition[2] "
			");"

			"const mat4 B = mat4("
			"	-1, 3,-3, 1,"
			"	 3,-6, 3, 0,"
			"	-3, 3, 0, 0,"
			"	 1, 0, 0, 0 "
			");"

			"mat4 Px, Py, Pz;"

			"void main(void)"
			"{"
			"	float u = gl_TessCoord.x;"
			"	float v = gl_TessCoord.y;"

			"	for(int j=0; j!=4; ++j)"
			"	for(int i=0; i!=4; ++i)"
			"	{"
			"		int k = j*4+i;"
			"		Px[j][i] = teevPosition[k].x;"
			"		Py[j][i] = teevPosition[k].y;"
			"		Pz[j][i] = teevPosition[k].z;"
			"	}"

			"	mat4 Cx = B * Px * B;"
			"	mat4 Cy = B * Py * B;"
			"	mat4 Cz = B * Pz * B;"

			"	vec4 up = vec4(u*u*u, u*u, u, 1);"
			"	vec4 vp = vec4(v*v*v, v*v, v, 1);"

			"	vec4 tempPosition = vec4(dot(Cx * vp, up), dot(Cy * vp, up), dot(Cz * vp, up), 1.0);"

			"	gl_Position = ProjectionMatrix * tempPosition;"
			"	teevPatchDistance = vec4(u, v, 1.0-u, 1.0-v);"
			"	teevColor = mix("
			"		mix(tecoColor[0], tecoColor[1], u),"
			"		mix(tecoColor[3], tecoColor[2], u),"
			"		v"
			"	);"
			"}"
		));
		teev.Compile();
		prog << teev;

		GeometryShader geom(ObjectDesc("Geometry"));
		geom.Source(StrCRef(
			"#version 150\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 3) out;"

			"uniform vec2 ViewportDimensions;"

			"in vec4 teevPatchDistance[3];"
			"in vec3 teevColor[3];"

			"out vec3 geomEdge;"
			"out vec3 geomColor;"

			"void main(void)"
			"{"
			"	vec2 ScreenPos[3];"
			"	vec4 PatchDist[3];"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		ScreenPos[v] ="
			"			ViewportDimensions*"
			"			gl_in[v].gl_Position.xy/"
			"			gl_in[v].gl_Position.w;"
			"		PatchDist[v] = vec4("
			"			teevPatchDistance[v].x < 1.0 ? 0.0 : 1.0,"
			"			teevPatchDistance[v].y < 1.0 ? 0.0 : 1.0,"
			"			teevPatchDistance[v].z < 1.0 ? 0.0 : 1.0,"
			"			teevPatchDistance[v].w < 1.0 ? 0.0 : 1.0 "
			"		);"
			"	}"

			"	vec2 TempVect[3];"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		TempVect[v] ="
			"			ScreenPos[(v+2)%3]-"
			"			ScreenPos[(v+1)%3];"
			"	}"

			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		float Dist = abs("
			"			TempVect[(v+1)%3].x * TempVect[(v+2)%3].y-"
			"			TempVect[(v+1)%3].y * TempVect[(v+2)%3].x "
			"		) / length(TempVect[v]);"

			"		float Width = 0.6+dot(PatchDist[(v+1)%3], PatchDist[(v+2)%3])*1.4;"

			"		geomEdge = vec3(0, 0, 0);"
			"		geomEdge[v] = Dist/Width;"
			"		geomColor = teevColor[v];"
			"		gl_Position = gl_in[v].gl_Position;"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		));
		geom.Compile();
		prog << geom;


		FragmentShader frag(ObjectDesc("Fragment"));
		frag.Source(StrCRef(
			"#version 150\n"

			"in vec3 geomEdge;"
			"in vec3 geomColor;"

			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	float EdgeDist = min(min(geomEdge.x, geomEdge.y), geomEdge.z);"
			"	float EdgeAlpha = exp2(-pow(EdgeDist, 2.0));"

			"	vec3 EdgeColor = vec3(0.0, 0.0, 0.0);"

			"	fragColor = mix(geomColor, EdgeColor, EdgeAlpha);"
			"}"
		));
		frag.Compile();
		prog << frag;

		prog.Link();
		prog.Use();

		vao.Bind();

		GLfloat r_min = 2.0f;
		GLfloat r_max = 4.5f;

		GLfloat seg_step = 1.0f/segments;

		std::vector<GLfloat> pos_data(segments*4*3);
		std::vector<GLfloat> pcp_data(pos_data.size());
		std::vector<GLfloat> ncp_data(pos_data.size());
		std::vector<GLfloat> ptl_data(segments*4*2);

		GLfloat kpa = GLfloat((4.0f/segments)*(4.0f/3.0f)*(sqrt(2.0f)-1.0f)/sqrt(2.0f));

		GLuint k_pos=0, k_pcp=0, k_ncp=0, k_ptl=0;
		for(GLuint s=0; s!=segments;++s)
		{
			Angle<GLfloat> a0(FullCircles((s-1)*seg_step));
			Angle<GLfloat> a1(FullCircles((s+0)*seg_step));
			Angle<GLfloat> a2(FullCircles((s+1)*seg_step));
			Angle<GLfloat> a3(FullCircles((s+2)*seg_step));

			// 0
			pos_data[k_pos++] = +r_min*Cos(a1);
			pos_data[k_pos++] = 0.0f;
			pos_data[k_pos++] = -r_min*Sin(a1);

			pcp_data[k_pcp++] = (+r_min*Cos(a2)-r_min*Cos(a0))*kpa;
			pcp_data[k_pcp++] = 0.0f;
			pcp_data[k_pcp++] = (-r_min*Sin(a2)+r_min*Sin(a0))*kpa;

			ncp_data[k_ncp++] = (+r_max*Cos(a1)-r_min*Cos(a1))/3.0f;
			ncp_data[k_ncp++] = 0.0f;
			ncp_data[k_ncp++] = (-r_max*Sin(a1)+r_min*Sin(a1))/3.0f;

			ptl_data[k_ptl++] = 3.0;
			ptl_data[k_ptl++] = 3.0;

			// 1
			pos_data[k_pos++] = +r_max*Cos(a1);
			pos_data[k_pos++] = 0.0f;
			pos_data[k_pos++] = -r_max*Sin(a1);

			pcp_data[k_pcp++] = (+r_min*Cos(a1)-r_max*Cos(a1))/3.0f;
			pcp_data[k_pcp++] = 0.0f;
			pcp_data[k_pcp++] = (-r_min*Sin(a1)+r_max*Sin(a1))/3.0f;

			ncp_data[k_ncp++] = (+r_max*Cos(a2)-r_max*Cos(a0))*kpa;
			ncp_data[k_ncp++] = 0.0f;
			ncp_data[k_ncp++] = (-r_max*Sin(a2)+r_max*Sin(a0))*kpa;

			ptl_data[k_ptl++] = 20.0f*r_max/r_min;
			ptl_data[k_ptl++] = 10.0f*r_max/r_min;

			// 2
			pos_data[k_pos++] = +r_max*Cos(a2);
			pos_data[k_pos++] = 0.0f;
			pos_data[k_pos++] = -r_max*Sin(a2);

			pcp_data[k_pcp++] = (+r_max*Cos(a1)-r_max*Cos(a3))*kpa;
			pcp_data[k_pcp++] = 0.0f;
			pcp_data[k_pcp++] = (-r_max*Sin(a1)+r_max*Sin(a3))*kpa;

			ncp_data[k_ncp++] = (+r_min*Cos(a2)-r_max*Cos(a2))/3.0f;
			ncp_data[k_ncp++] = 0.0f;
			ncp_data[k_ncp++] = (-r_min*Sin(a2)+r_max*Sin(a2))/3.0f;

			ptl_data[k_ptl++] = 3.0;
			ptl_data[k_ptl++] = 3.0;

			// 3
			pos_data[k_pos++] = +r_min*Cos(a2);
			pos_data[k_pos++] = 0.0f;
			pos_data[k_pos++] = -r_min*Sin(a2);

			pcp_data[k_pcp++] = (+r_max*Cos(a2)-r_min*Cos(a2))/3.0f;
			pcp_data[k_pcp++] = 0.0f;
			pcp_data[k_pcp++] = (-r_max*Sin(a2)+r_min*Sin(a2))/3.0f;

			ncp_data[k_ncp++] = (+r_min*Cos(a1)-r_min*Cos(a3))*kpa;
			ncp_data[k_ncp++] = 0.0f;
			ncp_data[k_ncp++] = (-r_min*Sin(a1)+r_min*Sin(a3))*kpa;

			ptl_data[k_ptl++] = 20.0;
			ptl_data[k_ptl++] = 10.0;
		}

		positions.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, pos_data);
		VertexArrayAttrib position_attr(prog, "Position");
		position_attr.Setup<Vec3f>();
		position_attr.Enable();

		prev_cps.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, pcp_data);
		VertexArrayAttrib prev_cp_attr(prog, "PrevCP");
		prev_cp_attr.Setup<Vec3f>();
		prev_cp_attr.Enable();

		next_cps.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, ncp_data);
		VertexArrayAttrib next_cp_attr(prog, "NextCP");
		next_cp_attr.Setup<Vec3f>();
		next_cp_attr.Enable();

		tess_levels.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, ptl_data);
		VertexArrayAttrib tess_lvl_attr(prog, "TessLevel");
		tess_lvl_attr.Setup<Vec2f>();
		tess_lvl_attr.Enable();

		Uniform<GLfloat>(prog, "Step").Set(step);
		Uniform<GLint>(prog, "InstCount").Set(instances);

		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.PatchParameter(PatchParameter::PatchVertices, 4);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				float(width)/height,
				1, 200
			)
		);
		viewport_dimensions.Set(width, height);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		GLfloat a = GLfloat(std::pow(SineWave(time / 51.7f), 3.0f));
		GLfloat b = GLfloat(SineWave(time / 31.1));

		camera_matrix.Set(
			CamMatrixf::LookingAt(
				Vec3f(
					GLfloat(0.55f*a*instances*step),
					GLfloat(step*0.5f+std::pow(b, 4)*80.0f),
					GLfloat(SineWave(time / 7.1)*step*(1.0-std::pow(b, 2)*0.9))
				),
				Vec3f(
					GLfloat(0.45f*a*instances*step),
					GLfloat(-step+std::pow(b, 2)*30.0f),
					GLfloat(0.0f)
				)
			)
		);

		gl.DrawArraysInstanced(
			PrimitiveType::Patches,
			0,
			segments*4,
			instances
		);
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
	return std::unique_ptr<Example>(new TessellationExample);
}

} // namespace oglplus
