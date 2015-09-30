/**
 *  @example standalone/001_triangle_glut_gles3.cpp
 *  @brief Shows the basic usage of OGLplus with GLUT and GLES3.
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>

#include <GLES3/gl3.h>

#include <oglplus/all.hpp>

#define GL_GLEXT_LEGACY 1
#include <GL/glut.h>

class Example
{
private:
	oglplus::Context gl;

	oglplus::VertexShader vs;

	oglplus::FragmentShader fs;

	oglplus::Program prog;

	oglplus::VertexArray triangle;

	oglplus::Buffer verts;
public:
	Example(void)
	{
		using namespace oglplus;

		// Set the vertex shader source
		vs.Source(
			"#ifdef GL_ES\n"
			"#version 300 es\n"
			"#else\n"
			"#version 330\n"
			"#endif\n"
			"in vec3 Position;"
			"void main(void)"
			"{"
			"	gl_Position = vec4(Position, 1.0);"
			"}"
		);
		// compile it
		vs.Compile();

		// set the fragment shader source
		fs.Source(
			"#ifdef GL_ES\n"
			"#version 300 es\n"
			"#else\n"
			"#version 330\n"
			"#endif\n"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(0.0, 1.0, 0.0, 1.0);"
			"}"
		);
		// compile it
		fs.Compile();

		// release the shader compiler
		Shader::ReleaseCompiler();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the triangle
		triangle.Bind();

		GLfloat triangle_verts[9] = {
			-1.0f,-1.0f, 0.0f,
			 1.0f,-1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f
		};
		// bind the VBO for the triangle vertices
		verts.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(
			Buffer::Target::Array,
			9,
			triangle_verts
		);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib vert_attr(prog, "Position");
		vert_attr.Setup<GLfloat>(3);
		vert_attr.Enable();

		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
	}

	void Display(void)
	{
		using namespace oglplus;

		gl.Clear().ColorBuffer().DepthBuffer();

		gl.DrawArrays(PrimitiveType::Triangles, 0, 3);
	}
};

class SingleExample
{
private:
	static Example*& SingleInstance(void)
	{
		static Example* test = nullptr;
		return test;
	}

	SingleExample(const SingleExample&);
public:

	SingleExample(void)
	{
		assert(!SingleInstance());
		SingleInstance() = new Example();
	}

	~SingleExample(void)
	{
		assert(SingleInstance());
		delete SingleInstance();
		SingleInstance() = nullptr;
	}

	static void Display(void)
	{
		assert(SingleInstance());
		SingleInstance()->Display();
		glutSwapBuffers();
	}
};

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OGLplus+GLUT+GLES3");

	try
	{
		SingleExample example;
		glutDisplayFunc(&SingleExample::Display);
		glutMainLoop();
		return 0;
	}
	catch(oglplus::Error& err)
	{
		std::cerr
			<< "Error (in "
			<< err.GLFunc()
			<< "'): "
			<< err.what()
			<< " ["
			<< err.SourceFile()
			<< ":"
			<< err.SourceLine()
			<< "] "
			<< std::endl;
	}
	return 1;
}

