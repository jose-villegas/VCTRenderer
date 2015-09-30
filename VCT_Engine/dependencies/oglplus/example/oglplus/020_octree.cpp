/**
 *  @example oglplus/020_octree.cpp
 *  @brief Shows Z-sorting of complete octree leaves for proper blending
 *
 *  @oglplus_screenshot{020_octree}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_transform_feedback2}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <cassert>

#include "example.hpp"

namespace oglplus {

class SortProg : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"in vec4 PosAndSize;"
			"void main(void)"
			"{"
			"	gl_Position = PosAndSize;"
			"}"
		).Compile();

		GeometryShader gs;
		gs.Source(
			"#version 150\n"
			"layout (points) in;"
			"layout (points, max_vertices=8) out;"

			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"mat4 Matrix = ProjectionMatrix*CameraMatrix;"

			"out vec4 xfbPosAndSize;"

			"int Index[8] = int[8](0,1,2,3,4,5,6,7);"

			"const vec3 Offset[8] = vec3[8]("
			"	vec3(-0.5,-0.5,-0.5),"
			"	vec3(+0.5,-0.5,-0.5),"
			"	vec3(-0.5,+0.5,-0.5),"
			"	vec3(+0.5,+0.5,-0.5),"
			"	vec3(-0.5,-0.5,+0.5),"
			"	vec3(+0.5,-0.5,+0.5),"
			"	vec3(-0.5,+0.5,+0.5),"
			"	vec3(+0.5,+0.5,+0.5) "
			");"

			"vec4 Child[8], ChildVS[8];"

			"void MakeChild(int i)"
			"{"
			"	vec4 Node = gl_in[0].gl_Position;"
			"	Child[i] = vec4(Node.xyz+Node.w*Offset[i],Node.w*0.5);"
			"	ChildVS[i] = Matrix*vec4(Child[i].xyz, 1.0);"
			"}"

			"ivec2 SortingNW8[19] = ivec2[19]("
			"	ivec2(0,7), ivec2(1,6), ivec2(2,5), ivec2(3,4),"
			"	ivec2(0,3), ivec2(4,7), ivec2(1,2), ivec2(5,6),"
			"	ivec2(0,1), ivec2(2,3), ivec2(4,5), ivec2(6,7),"
			"	ivec2(2,4), ivec2(3,5),"
			"	ivec2(1,2), ivec2(3,4), ivec2(5,6),"
			"	ivec2(2,3), ivec2(4,5)"
			");"

			"void SortSwapIdx(ivec2 pair)"
			"{"
			"	float zx = ChildVS[Index[pair.x]].z;"
			"	float zy = ChildVS[Index[pair.y]].z;"
			"	if(zx < zy)"
			"	{"
			"		int tmp = Index[pair.y];"
			"		Index[pair.y] = Index[pair.x];"
			"		Index[pair.x] = tmp;"
			"	}"
			"}"

			"void main(void)"
			"{"
			"	for(int c=0; c!=8; ++c)"
			"		MakeChild(c);"

			"	for(int s=0; s!=19; ++s)"
			"		SortSwapIdx(SortingNW8[s]);"

			"	for(int c=0; c!=8; ++c)"
			"	{"
			"		xfbPosAndSize = Child[Index[c]];"
			"		EmitVertex();"
			"		EndPrimitive();"
			"	}"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(gs);
		prog.TransformFeedbackVarying("xfbPosAndSize");
		prog.Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix;
	Uniform<Mat4f> camera_matrix;

	SortProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	{ }
};

class DrawProg : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"mat4 Matrix = ProjectionMatrix*CameraMatrix;"
			"in vec4 PosAndSize;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	gl_Position = Matrix * vec4(PosAndSize.xyz, 1.0);"
			"	gl_PointSize = 9 * gl_Position.w / gl_Position.z;"
			"	vertColor = normalize(vec3(1, 1, 1) - normalize(PosAndSize.xyz));"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 150\n"
			"in vec3 vertColor;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(vertColor, 0.5);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs);
		prog.Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix;
	Uniform<Mat4f> camera_matrix;

	DrawProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	{ }
};

class Octree
{
private:
	Context gl;
	const GLuint levels;
	std::vector<GLuint> node_count;

	Array<Buffer> pos_and_size;
	Array<VertexArray> vao;
	Array<TransformFeedback> xfb;

	DefaultTransformFeedback dxfb;

public:
	SortProg sort_prog;

	Octree(GLuint depth, GLfloat size)
	 : gl()
	 , levels(depth)
	 , node_count(levels)
	 , pos_and_size(levels)
	 , vao(levels-1)
	 , xfb(levels-1)
	{
		assert(levels != 0);

		for(GLuint l=0; l!=levels; ++l)
		{
			node_count[l] = 1;
			for(GLuint p=0; p!=l;++p) node_count[l] *= 8;

			if(l+1 != levels)
			{
				vao[l].Bind();
			}
			pos_and_size[l].Bind(Buffer::Target::Array);

			if(l == 0)
			{
				GLfloat root_data[4] = {0.0f, 0.0f, 0.0f, size};
				Buffer::Data(Buffer::Target::Array, root_data);
			}
			else
			{
				std::vector<GLfloat> data(node_count[l]*4);
				Buffer::Data(Buffer::Target::Array, data);
			}

			if(l+1 != levels)
			{
				VertexArrayAttrib vert_attr(sort_prog, "PosAndSize");
				vert_attr.Setup<Vec4f>().Enable();
				NoVertexArray().Bind();
			}

			if(l != 0)
			{
				xfb[l-1].Bind();
				pos_and_size[l].Bind(Buffer::Target::TransformFeedback);
				pos_and_size[l].BindBase(
					Buffer::IndexedTarget::TransformFeedback,
					0
				);
				dxfb.Bind();
			}
		}
	}

	VertexArray VAOForProgram(const Program& prog)
	{
		prog.Use();
		VertexArray pvao;
		pvao.Bind();

		pos_and_size[levels-1].Bind(Buffer::Target::Array);
		{
			VertexArrayAttrib vert_attr(prog, "PosAndSize");
			vert_attr.Setup<Vec4f>().Enable();
		}
		NoVertexArray().Bind();

		return pvao;
	}

	void Sort(void)
	{
		gl.Enable(Capability::RasterizerDiscard);
		for(GLuint l=0; l!=levels-1; ++l)
		{
			vao[l].Bind();
			xfb[l].Bind();
			TransformFeedback::Activator xfba(TransformFeedbackPrimitiveType::Points);
			gl.DrawArrays(PrimitiveType::Points, 0, node_count[l]);
			xfba.Finish();
		}
		gl.Disable(Capability::RasterizerDiscard);
	}

	void Draw(void)
	{
		gl.DrawArrays(PrimitiveType::Points, 0, node_count.back());
	}
};

class OctreeExample : public Example
{
private:
	Context gl;

	Octree octree;

	DrawProg draw_prog;

	VertexArray draw_vao;
public:
	OctreeExample(void)
	 : gl()
	 , octree(6, 3.0f)
	 , draw_vao(octree.VAOForProgram(draw_prog))
	{
		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::ProgramPointSize);
		gl.Enable(Capability::Blend);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);

		auto projection =
			CamMatrixf::PerspectiveX(
				Degrees(70),
				float(width)/height,
				1, 200
			);

		octree.sort_prog.Use();
		octree.sort_prog.projection_matrix.Set(projection);

		draw_prog.Use();
		draw_prog.projection_matrix.Set(projection);
	}

	void Render(double time)
	{
		auto camera =
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(8.0 + SineWave(time / 31.0)),
				FullCircles(time / 17.0),
				Degrees(SineWave(time / 21.0) * 90)
			);

		octree.sort_prog.Use();
		octree.sort_prog.camera_matrix.Set(camera);

		octree.Sort();

		gl.Clear().ColorBuffer().DepthBuffer();

		draw_prog.Use();
		draw_prog.camera_matrix.Set(camera);
		draw_vao.Bind();
		octree.Draw();
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
	return std::unique_ptr<Example>(new OctreeExample);
}

} // namespace oglplus
