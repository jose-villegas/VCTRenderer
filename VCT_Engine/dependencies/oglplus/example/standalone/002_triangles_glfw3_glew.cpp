/**
 *  @example standalone/002_triangles_glfw3_glew.cpp
 *  @brief Shows the basic usage of OGLplus with multiple contexts in GLFW3 and GLEW.
 *
 *  Based on an example provided by `ndelb`.
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <oglplus/all.hpp>
using namespace oglplus;

#include <iostream>
#include <stdexcept>
#include <memory>

static void on_error(int /*code*/, const char* description)
{
	throw std::runtime_error(description);
}

static void on_key_press(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

class Example
{
private:
	Context gl;
	VertexShader vs;
	FragmentShader fs;
	Program prog;
	//VAO for the mesh (just 2 triangles)
	VertexArray triangles;
	//VBO for the vertices positions
	Buffer verts;
public:
	Example(const Vec3f& color)
	{
		vs.Source(" \
			#version 140\n \
			in vec3 Position; \
			void main(void) \
			{ \
				gl_Position = vec4(Position, 1.0); \
			} \
		");
		vs.Compile();

		fs.Source(" \
			#version 140\n \
			out vec3 fragColor; \
			uniform vec3 Color; \
			void main(void) \
			{ \
				fragColor = Color; \
			} \
		");
		fs.Compile();

		prog.AttachShader(vs);
		prog.AttachShader(fs);
		prog.Link();
		prog.Use();

		Uniform<Vec3f>(prog, "Color").Set(color);

		triangles.Bind();
		GLfloat positions[9] = {
			0.0f,	0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f
		};
		verts.Bind(Buffer::Target::Array);
		Buffer::Data( Buffer::Target::Array, 9, positions);
		VertexArrayAttrib(prog, "Position").Setup<GLfloat>(3).Enable();
		gl.ClearColor(0,0,0,0);
	}

	void display(int windowWidth, int windowHeight)
	{
		gl.Viewport(0, 0, windowWidth, windowHeight);
		gl.Clear().ColorBuffer();
		verts.Bind(Buffer::Target::Array);
		gl.DrawArrays(PrimitiveType::Triangles, 0, 3);
	}
};

void run(void)
{
	std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> window1(
		glfwCreateWindow(480, 480, "Window 1", NULL, NULL),
		glfwDestroyWindow
	);
	std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> window2(
		glfwCreateWindow(480, 480, "Window 2", NULL, NULL),
		glfwDestroyWindow
	);

	if (!window1 || !window2)
	{
		throw std::runtime_error("GLFW3 failed to create a window");
	}


	glfwSetKeyCallback(window1.get(), on_key_press);
	glfwSetKeyCallback(window2.get(), on_key_press);
	glfwSetWindowPos(window2.get(), 481, 0);

	glfwMakeContextCurrent(window1.get());

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		throw std::runtime_error((const char*)glewGetErrorString(err));
	}

	glfwMakeContextCurrent(window1.get());
	Example triangle1(Vec3f(1,0,0));

	glfwMakeContextCurrent(window2.get());
	glfwMakeContextCurrent(window2.get());
	Example triangle2(Vec3f(0,1,0));

	while(true)
	{
		if(glfwWindowShouldClose(window1.get()))
		{
			break;
		}
		if(glfwWindowShouldClose(window2.get()))
		{
			break;
		}

		glfwMakeContextCurrent(window1.get());
		{
			int width, height;
			glfwGetFramebufferSize(window1.get(), &width, &height);
			triangle1.display(width, height);
			glfwSwapBuffers(window1.get());
		}

		glfwMakeContextCurrent(window2.get());
		{
			int width, height;
			glfwGetFramebufferSize(window2.get(), &width, &height);
			triangle2.display(width, height);
			glfwSwapBuffers(window2.get());
		}

		glfwPollEvents();
	}
}

int main(void)
{
	try
	{
		glfwSetErrorCallback(on_error);
		glfwInit();
		run();
		glfwTerminate();
	}
	catch(std::exception& error)
	{
		glfwTerminate();
		std::cerr << "Error: " << error.what() << std::endl;
		return 1;
	}
	return 0;
}
