/**
 *  @example oglplus/022_xyz_planes.cpp
 *  @brief Shows how to use clipping for proper blending of overlapping objects
 *
 *  @oglplus_screenshot{022_xyz_planes}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 *  @oglplus_example_uses_gl{GL_ARB_shading_language_include}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/twisted_torus.hpp>
#include <oglplus/shapes/plane.hpp>

#include <sstream>

#include "example.hpp"

namespace oglplus {

class TorusExample : public Example
{
private:
	// helper object building torus vertex attributes
	shapes::TwistedTorus make_torus;
	// helper object encapsulating torus drawing instructions
	shapes::DrawingInstructions torus_instr;
	// indices pointing to torus primitive elements
	shapes::TwistedTorus::IndexArray torus_indices;

	// similar helpers for the X-,Y- and Z-plane
	std::vector<shapes::Plane> make_plane;
	shapes::DrawingInstructions plane_instr;
	shapes::Plane::IndexArray plane_indices;

	static std::vector<shapes::Plane> make_plane_builders(void)
	{
		std::vector<shapes::Plane> result;
		result.push_back(shapes::Plane(
			Vec3f( 0.0f,  2.0f,  0.0f),
			Vec3f( 0.0f,  0.0f,  2.0f)
		));
		result.push_back(shapes::Plane(
			Vec3f( 2.0f,  0.0f,  0.0f),
			Vec3f( 0.0f,  0.0f, -2.0f)
		));
		result.push_back(shapes::Plane(
			Vec3f( 2.0f,  0.0f,  0.0f),
			Vec3f( 0.0f,  2.0f,  0.0f)
		));
		return result;
	}

	// wrapper around the current OpenGL context
	Context gl;

	Shader torus_vs, plane_vs;
	Shader torus_fs, plane_fs;

	Program torus_prog, plane_prog;

	// Uniform references
	std::vector<ProgramUniform<GLfloat>> torus_clip_signs;
	std::vector<ProgramUniform<GLfloat>> plane_clip_signs;

	Lazy<ProgramUniform<Vec3f>> plane_normal;
	Lazy<ProgramUniform<Mat4f>>
		plane_camera_matrix,
		torus_camera_matrix,
		torus_model_matrix;

	// A vertex array objects for the rendered torus and planes
	VertexArray torus;
	Array<VertexArray> plane;

	// VBOs for the torus's vertex attributes
	Buffer torus_positions, torus_texcoords;
	// VBOs for plane vertex positions
	Array<Buffer> plane_positions;
public:
	TorusExample(void)
	 : make_torus()
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , make_plane(make_plane_builders())
	 , plane_instr(make_plane[0].Instructions())
	 , plane_indices(make_plane[0].Indices())
	 , torus_vs(ShaderType::Vertex, ObjectDesc("Torus vertex"))
	 , plane_vs(ShaderType::Vertex, ObjectDesc("Plane vertex"))
	 , torus_fs(ShaderType::Fragment, ObjectDesc("Torus fragment"))
	 , plane_fs(ShaderType::Fragment, ObjectDesc("Plane fragment"))
	 , torus_prog(ObjectDesc("Torus"))
	 , plane_prog(ObjectDesc("Plane"))
	 , plane_normal(plane_prog, "Normal")
	 , plane_camera_matrix(plane_prog, "CameraMatrix")
	 , torus_camera_matrix(torus_prog, "CameraMatrix")
	 , torus_model_matrix(torus_prog, "ModelMatrix")
	 , plane(make_plane.size())
	 , plane_positions(make_plane.size())
	{
		std::stringstream config;
		config << "#define PlaneCount " << plane.size() << '\n';

		ShaderInclude config_glsl("/config.glsl", config.str());

		torus_vs.Source(
			"#version 140\n"
			"#extension GL_ARB_shading_language_include : enable\n"
			"#include <config.glsl>\n"

			"uniform mat4 ProjectionMatrix, ModelMatrix, CameraMatrix;"
			"uniform float ClipSign[PlaneCount];"
			"uniform vec4 ClipPlane[PlaneCount];"
			"in vec4 Position;"
			"in vec2 TexCoord;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = "
			"		ModelMatrix *"
			"		Position;"
			"	for(int p=0; p!=PlaneCount; ++p)"
			"	{"
			"		gl_ClipDistance[p] = "
			"			ClipSign[p]* "
			"			dot(ClipPlane[p], gl_Position);"
			"	}"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		gl_Position;"
			"}"
		);

		torus_fs.Source(
			"#version 140\n"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float i = ("
			"		int(vertTexCoord.x*36) % 2+"
			"		int(vertTexCoord.y*24) % 2"
			"	) % 2;"
			"	fragColor = vec4(1-i/2, 1-i/2, 1-i/2, 1.0);"
			"}"
		);

		torus_prog.AttachShader(torus_vs);
		torus_prog.AttachShader(torus_fs);
		torus_prog.BuildInclude("/");
		torus_prog.Use();

		// bind the VAO for the torus
		torus.Bind();

		// bind the VBO for the torus vertices
		torus_positions.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(torus_prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the torus UV-coordinates
		torus_texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.TexCoordinates(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(torus_prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		plane_vs.Source(
			"#version 140\n"
			"#extension GL_ARB_shading_language_include : enable\n"
			"#include <config.glsl>\n"

			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"uniform float ClipSign[PlaneCount];"
			"uniform vec4 ClipPlane[PlaneCount];"
			"uniform vec3 Normal;"
			"in vec4 Position;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	for(int p=0; p!=PlaneCount; ++p)"
			"	{"
			"		gl_ClipDistance[p] = "
			"			ClipSign[p]* "
			"			dot(ClipPlane[p], gl_Position);"
			"	}"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		gl_Position;"
			"	vertColor = normalize("
			"		abs(Normal) + "
			"		0.4*Position.xyz"
			"	);"
			"}"
		).CompileInclude("/");

		plane_fs.Source(
			"#version 140\n"
			"in vec3 vertColor;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(vertColor, 0.7);"
			"}"
		).Compile();

		plane_prog.AttachShader(plane_vs);
		plane_prog.AttachShader(plane_fs);
		plane_prog.Link();
		plane_prog.Use();


		ProgramUniform<Vec4f> torus_clip_plane(torus_prog, "ClipPlane");
		ProgramUniform<Vec4f> plane_clip_plane(plane_prog, "ClipPlane");
		ProgramUniform<GLfloat> torus_clip_sign(torus_prog, "ClipSign");
		ProgramUniform<GLfloat> plane_clip_sign(plane_prog, "ClipSign");
		for(std::size_t p=0; p!=plane.size(); ++p)
		{
			plane[p].Bind();

			plane_positions[p].Bind(Buffer::Target::Array);
			{
				std::vector<GLfloat> data;
				GLuint n = make_plane[p].Positions(data);
				// upload the data
				Buffer::Data(Buffer::Target::Array, data);
				VertexArrayAttrib attr(plane_prog, "Position");
				attr.Setup<GLfloat>(n);
				attr.Enable();
			}
			{
				auto eq = make_plane[p].Equation();
				torus_clip_plane[p].Set(eq);
				plane_clip_plane[p].Set(eq);
			}
			{
				torus_clip_signs.push_back(torus_clip_sign[p]);
				plane_clip_signs.push_back(plane_clip_sign[p]);
			}
		}

		//
		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.FrontFace(make_torus.FaceWinding());
		gl.Enable(Capability::DepthTest);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		auto proj = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 30
		);
		ProgramUniform<Mat4f>(torus_prog, "ProjectionMatrix").Set(proj);
		ProgramUniform<Mat4f>(plane_prog, "ProjectionMatrix").Set(proj);
	}

	void RenderTorus(void)
	{
		torus_prog.Use();
		torus.Bind();
		torus_instr.Draw(torus_indices);
	}

	void RenderPlane(std::size_t p)
	{
		gl.Enable(Capability::Blend);
		plane_prog.Use();
		plane_normal.Set(make_plane[p].Normal());
		plane[p].Bind();
		plane_instr.Draw(plane_indices);
		gl.Disable(Capability::Blend);
	}

	void BSP(const Mat4f& camera, GLuint p)
	{
		assert(p < std::size_t(plane.size()));
		// the normal vector of the plane
		Vec4f normal(make_plane[p].Normal(), 0.0);
		// check if we are seeing the front or the back face
		GLfloat sign = ((camera*normal).z() >= 0.0f)? 1.0f: -1.0f;
		bool at_leaf = p+1 == plane.size();

		+(Functionality::ClipDistance|p);
		torus_clip_signs[p].Set(-sign);
		plane_clip_signs[p].Set(-sign);
		if(at_leaf) RenderTorus();
		else BSP(camera, p+1);
		-(Functionality::ClipDistance|p);

		RenderPlane(p);

		+(Functionality::ClipDistance|p);
		torus_clip_signs[p].Set(+sign);
		plane_clip_signs[p].Set(+sign);
		if(at_leaf) RenderTorus();
		else BSP(camera, p+1);
		-(Functionality::ClipDistance|p);

	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			6.0,
			FullCircles(time / 10.0),
			Degrees(45.0 + SineWave(time / 7.0)*30.0)
		);
		auto model = ModelMatrixf::RotationX(FullCircles(time / 12.0));

		plane_camera_matrix.Set(camera);
		torus_camera_matrix.Set(camera);
		torus_model_matrix.Set(model);

		BSP(camera, 0);
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
	return std::unique_ptr<Example>(new TorusExample);
}

} // namespace oglplus
