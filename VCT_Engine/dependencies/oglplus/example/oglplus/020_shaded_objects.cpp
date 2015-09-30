/**
 *  @example oglplus/020_shaded_objects.cpp
 *  @brief Shows how to draw several differently shaded geometric shapes
 *
 *  @oglplus_screenshot{020_shaded_objects}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/sphere.hpp>
#include <oglplus/shapes/torus.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

// Renderable geometric shape built by the ShapeMaker class
template <class ShapeMaker>
class Shape
{
private:
	// helper object building shape's vertex attributes
	ShapeMaker make_shape;
	// helper object encapsulating the drawing instructions
	shapes::DrawingInstructions shape_instr;
	// indices pointing to the shape's primitive elements
	typename ShapeMaker::IndexArray shape_indices;

	// Shading program
	Program prog;

	static Program make(const Shader& vs, const Shader& fs)
	{
		Program prog;
		prog.AttachShader(vs).AttachShader(fs).Link().Use();
		return std::move(prog);
	}

	// Uniform variables
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	Uniform<Vec3f> light_pos;

	// A vertex array object for the rendered shape
	VertexArray shape;

	// VBOs for the shape's vertices, normals and tex-coordinates
	Buffer verts, normals, texcoords;
public:
	Shape(const Shader& vs, const Shader& fs)
	 : shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , prog(make(vs, fs))
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , light_pos(prog, "LightPos")
	{

		// bind the VAO for the shape
		shape.Bind();

		const GLuint n_attr = 3;
		// pointers to the vertex attribute data build functions
		typedef GLuint (ShapeMaker::*Func)(std::vector<GLfloat>&) const;
		Func func[n_attr] = {
			&ShapeMaker::Positions,
			&ShapeMaker::Normals,
			&ShapeMaker::TexCoordinates
		};
		// managed references to the VBOs
		Reference<Buffer> vbo[n_attr] = {verts, normals, texcoords};
		// vertex attribute identifiers from the shaders
		const GLchar* ident[n_attr] = {"Position", "Normal", "TexCoord"};

		for(GLuint i=0; i!=n_attr; ++i)
		{
			// bind the VBO
			vbo[i].Bind(Buffer::Target::Array);
			// make the data
			std::vector<GLfloat> data;
			GLuint n_per_vertex = (make_shape.*func[i])(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attrib
			VertexArrayAttrib attr(prog, ident[i]);
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}
	}

	void SetProjection(const Mat4f& projection)
	{
		prog.Use();
		projection_matrix.Set(projection);
	}

	void Render(
		const Vec3f& light,
		const Mat4f& camera,
		const Mat4f& model
	)
	{
		// use the shading program
		prog.Use();
		// set the uniforms
		light_pos.Set(light);
		camera_matrix.Set(camera);
		model_matrix.Set(model);
		// bind the VAO
		shape.Bind();
		// use the instructions to draw the shape
		// (this basically calls glDrawArrays* or glDrawElements*)
		shape_instr.Draw(shape_indices);
	}
};

class ShapeExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// The vertex shader is shader by the shapes
	VertexShader vs;

	// Makes the common shared vertex shader
	static VertexShader make_vs(void)
	{
		VertexShader shader;

		shader.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"
			"out vec2 vertTexCoord;"
			"out vec3 vertNormal;"
			"out vec3 vertLight;"
			"uniform vec3 LightPos;"
			"void main(void)"
			"{"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertLight = LightPos - gl_Position.xyz;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);

		shader.Compile();
		return shader;
	}

	// The common first part of all fragment shader sources
	static const GLchar* fs_prologue(void)
	{
		return
		"#version 140\n"
		"in vec2 vertTexCoord;"
		"in vec3 vertNormal;"
		"in vec3 vertLight;"
		"out vec4 fragColor;"
		"void main(void)"
		"{"
		"	float Len = dot(vertLight, vertLight);"
		"	float Dot = Len > 0.0 ? dot("
		"		vertNormal, "
		"		normalize(vertLight)"
		"	) / Len : 0.0;"
		"	float Intensity = 0.2 + max(Dot, 0.0) * 4.0;";
	}

	// The common last part of all fragment shader sources
	static const GLchar* fs_epilogue(void)
	{
		return
		"	fragColor = vec4(Color * Intensity, 1.0);"
		"}";
	}

	// The part calculating the color for the black/white checker shader
	static const GLchar* fs_bw_checker(void)
	{
		return
		"	float c = ("
		"		1 +"
		"		int(vertTexCoord.x*8) % 2+"
		"		int(vertTexCoord.y*8) % 2"
		"	) % 2;"
		"	vec3 Color = vec3(c, c, c);";
	}

	// The part calculating the color for the yellow/black strips shader
	static const GLchar* fs_yb_strips(void)
	{
		return
		"	float m = int((vertTexCoord.x+vertTexCoord.y)*16) % 2;"
		"	vec3 Color = mix("
		"		vec3(0.0, 0.0, 0.0),"
		"		vec3(1.0, 1.0, 0.0),"
		"		m"
		"	);";
	}

	// The part calculating the color for the white/orange strips shader
	static const GLchar* fs_wo_vstrips(void)
	{
		return
		"	float m = int(vertTexCoord.x*8) % 2;"
		"	vec3 Color = mix("
		"		vec3(1.0, 0.6, 0.1),"
		"		vec3(1.0, 0.9, 0.8),"
		"		m"
		"	);";
	}

	// The part calculating the color for the blue/red circles shader
	static const GLchar* fs_br_circles(void)
	{
		return
		"	vec2  center = vertTexCoord - vec2(0.5, 0.5);"
		"	float m = int(sqrt(length(center))*16) % 2;"
		"	vec3 Color = mix("
		"		vec3(1.0, 0.0, 0.0),"
		"		vec3(0.0, 0.0, 1.0),"
		"		m"
		"	);";
	}

	// The part calculating the color for the white/green spiral shader
	static const GLchar* fs_wg_spirals(void)
	{
		return
		"	vec2  center = (vertTexCoord - vec2(0.5, 0.5)) * 16.0;"
		"	float l = length(center);"
		"	float t = atan(center.y, center.x)/(2.0*asin(1.0));"
		"	float m = int(l+t) % 2;"
		"	vec3 Color = mix("
		"		vec3(0.0, 1.0, 0.0),"
		"		vec3(1.0, 1.0, 1.0),"
		"		m"
		"	);";
	}

	// makes a fragment shader from the prologe, custom part and epilogue
	static Shader make_fs(const char* color_fs, const char* desc)
	{
		Shader shader(ShaderType::Fragment, ObjectDesc(desc));
		const GLchar* src[3] = {fs_prologue(), color_fs, fs_epilogue()};
		shader.Source(src);
		shader.Compile();
		return shader;
	}

	// The shapes that will be rendered
	// a stationary sphere with unit radius at the origin
	Shape<shapes::Sphere> sphere;
	// three rotating unit cubes at the X, Y and Z axes
	Shape<shapes::Cube> cubeX, cubeY, cubeZ;
	// a torus rotating around the (1,1,1) vector
	Shape<shapes::Torus> torus;
public:
	ShapeExample(void)
	 : vs(make_vs())
	 , sphere(vs,make_fs(fs_yb_strips(), "Y/B strips"))
	 , cubeX(vs, make_fs(fs_bw_checker(), "B/W checker"))
	 , cubeY(vs, make_fs(fs_br_circles(), "B/R circles"))
	 , cubeZ(vs, make_fs(fs_wg_spirals(), "W/G spirals"))
	 , torus(vs, make_fs(fs_wo_vstrips(), "W/O vstrips"))
	{
		gl.ClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		auto projection = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 50
		);
		sphere.SetProjection(projection);
		cubeX.SetProjection(projection);
		cubeY.SetProjection(projection);
		cubeZ.SetProjection(projection);
		torus.SetProjection(projection);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		// make the light position vector
		auto light = Vec3f(2.0f, 2.0f, 2.0f);
		// make the matrix for camera orbiting the origin
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			6.0,
			Degrees(time * 15),
			Degrees(SineWave(time / 6.0) * 45)
		);
		// render the shapes
		sphere.Render(light, camera, Mat4f());
		cubeX.Render(
			light,
			camera,
			ModelMatrixf::Translation(2.0f, 0.0f, 0.0f) *
			ModelMatrixf::RotationX(Degrees(time * 45))
		);
		cubeY.Render(
			light,
			camera,
			ModelMatrixf::Translation(0.0f, 2.0f, 0.0f) *
			ModelMatrixf::RotationY(Degrees(time * 90))
		);
		cubeZ.Render(
			light,
			camera,
			ModelMatrixf::Translation(0.0f, 0.0f, 2.0f) *
			ModelMatrixf::RotationZ(Degrees(time * 135))
		);
		torus.Render(
			light,
			camera,
			ModelMatrixf::Translation(-1.0f, -1.0f, -1.0f) *
			ModelMatrixf::RotationA(
				Vec3f(1.0f, 1.0f, 1.0f),
				Degrees(time * 45)
			) *
			ModelMatrixf::RotationY(Degrees(45)) *
			ModelMatrixf::RotationX(Degrees(45))
		);
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
	return std::unique_ptr<Example>(new ShapeExample);
}

} // namespace oglplus
