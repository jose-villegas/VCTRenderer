/**
 *  @example standalone/001_triangle_sdl_glew.cpp
 *  @brief Shows the basic usage of OGLplus with SDL and GLEW.
 *
 *  @oglplus_screenshot{001_triangle}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <oglplus/all.hpp>

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
		vs.Source(" \
			#version 120\n \
			attribute vec3 Position; \
			void main(void) \
			{ \
				gl_Position = vec4(Position, 1.0); \
			} \
		");
		// compile it
		vs.Compile();

		// set the fragment shader source
		fs.Source(" \
			#version 120\n \
			void main(void) \
			{ \
				gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \
			} \
		");
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the triangle
		triangle.Bind();

		GLfloat triangle_verts[9] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
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

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void Display(void)
	{
		using namespace oglplus;

		gl.Clear().ColorBuffer();

		gl.DrawArrays(PrimitiveType::Triangles, 0, 3);
	}
};

class WrapSDL
{
private:
	bool done;
public:
	WrapSDL(const char* title)
	 : done(false)
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
			throw std::runtime_error(SDL_GetError());

		if(!SDL_SetVideoMode(
			800, 600, 32,
			SDL_HWSURFACE |
			SDL_GL_DOUBLEBUFFER |
			SDL_OPENGL
		))
		{
			std::runtime_error e(SDL_GetError());
			SDL_Quit();
			throw e;
		}
		SDL_WM_SetCaption(title, nullptr);
	}

	~WrapSDL(void)
	{
		SDL_Quit();
	}

	void ProcessEvents(void)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				done = true;
			}
		}
	}

	bool Done(void) const
	{
		return done;
	}

	void SwapBuffers(void)
	{
		SDL_GL_SwapBuffers();
	}
};

class WrapGLEW
{
public:
	WrapGLEW(void)
	{
		GLenum res = glewInit();
		if(res != GLEW_OK)
		{
			throw std::runtime_error(
				(const char*)glewGetErrorString(res)
			);
		}
		glGetError();
	}
};

int main(int argc, char* argv[])
{
	try
	{
		WrapSDL wrap_sdl("OGLplus+SDL+GLEW");
		WrapGLEW wrap_glew;
		Example example;

		do
		{
			example.Display();
			wrap_sdl.SwapBuffers();
			wrap_sdl.ProcessEvents();
		}
		while(!wrap_sdl.Done());

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
	catch(std::runtime_error& sre)
	{
		std::cerr << "Runtime error: " << sre.what() << std::endl;
	}
	return 1;
}

