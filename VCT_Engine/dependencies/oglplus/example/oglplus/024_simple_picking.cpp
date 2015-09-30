/**
 *  @example oglplus/024_simple_picking.cpp
 *  @brief Shows how to do picking/hiliting of objects by mouse
 *
 *  @oglplus_screenshot{024_simple_picking}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_transform_feedback2}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>

#include <map>
#include <cmath>

#include "example.hpp"

namespace oglplus {

class PickingExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions cube_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray cube_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex, geometry and fragment shader
	Shader vs, gs, fs;

	// Drawing and picking programs
	Program draw_prog, pick_prog;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertices
	Buffer verts;

	// VBO for the values returned by transform feedback
	Buffer picked_instances;

	// The query object used to get count of picked objects
	Query count_query;

	// the pair of picked instance depth and id
	typedef std::pair<const GLfloat, GLint> DepthAndID;
public:
	PickingExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , vs(ShaderType::Vertex, ObjectDesc("Vertex"))
	 , gs(ShaderType::Geometry, ObjectDesc("Geometry"))
	 , fs(ShaderType::Fragment, ObjectDesc("Fragment"))
	{
		// Set the vertex shader source
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"in vec4 Position;"
			"out vec3 vertColor;"
			"flat out int vertInstanceID;"
			"void main(void)"
			"{"
			"	float x = gl_InstanceID % 6 - 2.5;"
			"	float y = gl_InstanceID / 6 - 2.5;"
			"	mat4 ModelMatrix = mat4("
			"		 1.0, 0.0, 0.0, 0.0,"
			"		 0.0, 1.0, 0.0, 0.0,"
			"		 0.0, 0.0, 1.0, 0.0,"
			"		 2*x, 2*y, 0.0, 1.0 "
			"	);"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		Position;"
			"	vertColor = vec3("
			"		abs(normalize((ModelMatrix*Position).xy)),"
			"		0.1"
			"	);"
			"	vertInstanceID = gl_InstanceID;"
			"}"
		);
		vs.Compile();

		// Set the geometry shader source
		gs.Source(
			"#version 150\n"
			"layout(triangles) in;"
			"layout(points, max_vertices = 1) out;"

			"flat in int vertInstanceID[];"
			"uniform vec2 MousePos;"

			"out int geomInstanceID;"
			"out float geomDepth;"

			"vec2 barycentric_coords(vec4 a, vec4 b, vec4 c)"
			"{"
			// we'll need normalized device coordinates
			// of the triangle vertices
			"	vec2 ad = vec2(a.x/a.w, a.y/a.w);"
			"	vec2 bd = vec2(b.x/b.w, b.y/b.w);"
			"	vec2 cd = vec2(c.x/c.w, c.y/c.w);"
			"	vec2 u = cd - ad;"
			"	vec2 v = bd - ad;"
			"	vec2 r = MousePos - ad;"
			"	float d00 = dot(u, u);"
			"	float d01 = dot(u, v);"
			"	float d02 = dot(u, r);"
			"	float d11 = dot(v, v);"
			"	float d12 = dot(v, r);"
			"	float id = 1.0 / (d00 * d11 - d01 * d01);"
			"	float ut = (d11 * d02 - d01 * d12) * id;"
			"	float vt = (d00 * d12 - d01 * d02) * id;"
			"	return vec2(ut, vt);"
			"}"

			"vec3 intersection(vec3 a, vec3 b, vec3 c, vec2 bc)"
			"{"
			"	return (c - a)*bc.x + (b - a)*bc.y;"
			"}"

			"bool inside_triangle(vec2 b)"
			"{"
			"	return ("
			"		(b.x >= 0.0) &&"
			"		(b.y >= 0.0) &&"
			"		(b.x + b.y <= 1.0)"
			"	);"
			"}"

			"void main(void)"
			"{"
			"	vec2 bc = barycentric_coords("
			"		gl_in[0].gl_Position,"
			"		gl_in[1].gl_Position,"
			"		gl_in[2].gl_Position "
			"	);"
			"	if(inside_triangle(bc))"
			"	{"
			"		gl_Position = vec4(intersection("
			"			gl_in[0].gl_Position.xyz,"
			"			gl_in[1].gl_Position.xyz,"
			"			gl_in[2].gl_Position.xyz,"
			"			bc"
			"		), 1.0);"
			"		geomDepth = gl_Position.z;"
			"		geomInstanceID = vertInstanceID[0];"
			"		EmitVertex();"
			"		EndPrimitive();"
			"	}"
			"}"
		);
		gs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 150\n"
			"flat in int vertInstanceID;"
			"in vec3 vertColor;"
			"uniform int Picked;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	if(vertInstanceID == Picked)"
			"		fragColor = vec4(1.0, 1.0, 1.0, 1.0);"
			"	else fragColor = vec4(vertColor, 1.0);"
			"}"
		);
		fs.Compile();

		// attach the shaders to the picking program
		pick_prog.AttachShader(vs);
		pick_prog.AttachShader(gs);

		const GLchar* var_names[2] = {"geomDepth", "geomInstanceID"};
		pick_prog.TransformFeedbackVaryings(
			2, var_names,
			TransformFeedbackMode::InterleavedAttribs
		);
		pick_prog.Link();

		// attach the shaders to the drawing program
		draw_prog.AttachShader(vs);
		draw_prog.AttachShader(fs);
		draw_prog.Link();

		// bind the VAO for the cube
		cube.Bind();

		// buffer for the picked instance depths and IDs
		picked_instances.Bind(Buffer::Target::TransformFeedback);
		picked_instances.BindBase(
			Buffer::IndexedTarget::TransformFeedback,
			0
		);
		{
			std::vector<DepthAndID> data(36, DepthAndID(0.0, 0));
			Buffer::Resize(
				BufferTarget::TransformFeedback,
				BufferSize::Of<DepthAndID>(36)
			);
		}

		// bind the VBO for the cube vertices
		verts.Bind(Buffer::Target::Array);
		{
			// make and upload the vertex data
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);

			// setup the vertex attribs array for the vertices
			VertexArrayAttrib draw_attr(draw_prog, "Position");
			draw_attr.Setup<GLfloat>(n_per_vertex);
			draw_attr.Enable();
		}

		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		gl.FrontFace(make_cube.FaceWinding());
		gl.CullFace(Face::Back);
		gl.Enable(Capability::CullFace);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		auto perspective = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 80
		);
		draw_prog.Use();
		Uniform<Mat4f>(draw_prog, "ProjectionMatrix").Set(perspective);
		pick_prog.Use();
		Uniform<Mat4f>(pick_prog, "ProjectionMatrix").Set(perspective);
	}

	// called when the mouse pointer is moved; x and y are between -1 and 1
	void MouseMoveNormalized(float x, float y, float /*aspect*/)
	{
		pick_prog.Use();
		Uniform<Vec2f>(pick_prog, "MousePos").Set(x, y);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			16.0,
			FullCircles(time * 0.1),
			Degrees(SineWave(time / 20.0) * 30)
		);

		// use the picking program
		pick_prog.Use();
		Uniform<Mat4f>(pick_prog, "CameraMatrix").Set(camera);

		// query the number of values written to the feedabck buffer
		GLuint picked_count = 0;
		{
			QueryExecution<GLuint> query_exec(
				count_query,
				Query::Target::
				TransformFeedbackPrimitivesWritten,
				picked_count
			);
			TransformFeedback::Activator xfb_act(
				TransformFeedbackPrimitiveType::Points
			);
			// draw 36 instances of the cube
			// the vertex shader will take care of their placement
			cube_instr.Draw(cube_indices, 36);
			//
			xfb_act.Finish();
			query_exec.WaitForResult();
		}

		std::map<GLfloat, GLint> picked_objs;
		{
			picked_instances.Bind(Buffer::Target::TransformFeedback);
			BufferTypedMap<DepthAndID> picked_instances_map(
				Buffer::Target::TransformFeedback,
				BufferMapAccess::Read
			);
			picked_objs.insert(
				picked_instances_map.Data(),
				picked_instances_map.Data()+picked_count
			);
		}

		draw_prog.Use();

		Uniform<GLint> picked(draw_prog, "Picked");
		if(picked_objs.empty()) picked = -1;
		else picked = picked_objs.begin()->second;
		Uniform<Mat4f>(draw_prog, "CameraMatrix").Set(camera);

		// draw 36 instances of the cube
		// the vertex shader will take care of their placement
		cube_instr.Draw(cube_indices, 36);
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
	return std::unique_ptr<Example>(new PickingExample);
}

} // namespace oglplus
