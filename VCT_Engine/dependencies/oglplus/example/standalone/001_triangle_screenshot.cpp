/**
 *  @example standalone/001_triangle_screenshot.cpp
 *  @brief Shows the basic usage of OGLplus with EGLplus
 *
 *  @oglplus_screenshot{001_triangle}
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>
#include <fstream>

#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <eglplus/egl.hpp>
#define EGL_CONTEXT_MAJOR_VERSION 0x3098
#define EGL_CONTEXT_MINOR_VERSION 0x30FB
#define EGL_CONTEXT_FLAGS                   0x30FC
#define EGL_CONTEXT_OPENGL_PROFILE_MASK     0x30FD
#define EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY  0x31BD
#define EGL_OPENGL_ES3_BIT                  0x0040
#define EGL_NO_RESET_NOTIFICATION           0x31BE
#define EGL_LOSE_CONTEXT_ON_RESET           0x31BF
#define EGL_CONTEXT_OPENGL_DEBUG_BIT               0x00000001
#define EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT  0x00000002
#define EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT       0x00000004
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT          0x00000001
#define EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT 0x00000002

#include <eglplus/all.hpp>

void render_frame(void)
{
	using namespace oglplus;

	Context gl;

	VertexShader vs;
	vs.Source(" \
		#version 120\n \
		attribute vec3 Position; \
		void main(void) \
		{ \
			gl_Position = vec4(Position, 1.0); \
		} \
	");
	vs.Compile();

	FragmentShader fs;
	fs.Source(" \
		#version 120\n \
		void main(void) \
		{ \
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); \
		} \
	");
	fs.Compile();

	Program prog;
	prog.AttachShader(vs);
	prog.AttachShader(fs);
	prog.Link();
	prog.Use();

	VertexArray triangle;
	triangle.Bind();

	GLfloat triangle_verts[9] = {
		 0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f
	};

	oglplus::Buffer verts;
	verts.Bind(Buffer::Target::Array);
	Buffer::Data(
		Buffer::Target::Array,
		9,
		triangle_verts
	);

	VertexArrayAttrib vert_attr(prog, "Position");
	vert_attr.Setup<Vec3f>();
	vert_attr.Enable();

	gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gl.Clear().ColorBuffer();

	gl.DrawArrays(PrimitiveType::Triangles, 0, 3);
}

void save_frame(unsigned width, unsigned height, const char* screenshot_path)
{
	std::vector<char> pixels(width * height * 3);
	glReadPixels(
		0, 0,
		width,
		height,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		pixels.data()
	);
	std::ofstream output(screenshot_path);
	output.write(pixels.data(), pixels.size());
	std::cout << screenshot_path << std::endl;
}

void make_screenshot(unsigned width, unsigned height, const char* screenshot_path)
{
	using namespace eglplus;

	eglplus::Display display;
	LibEGL egl(display);

	Configs configs(
		display,
		ConfigAttribs()
			.Add(ConfigAttrib::RedSize, 8)
			.Add(ConfigAttrib::GreenSize, 8)
			.Add(ConfigAttrib::BlueSize, 8)
			.Add(ConfigAttrib::DepthSize, 24)
			.Add(ConfigAttrib::StencilSize, 8)
			.Add(ColorBufferType::RGBBuffer)
			.Add(RenderableTypeBit::OpenGL)
			.Add(SurfaceTypeBit::Pbuffer)
			.Get()
	);

	Config config = configs.First();

	Surface surface = Surface::Pbuffer(
		display,
		config,
		SurfaceAttribs()
			.Add(SurfaceAttrib::Width, int(width))
			.Add(SurfaceAttrib::Height, int(height))
			.Get()
	);

	BindAPI(RenderingAPI::OpenGL);
	Context context(
		display,
		config,
		ContextAttribs()
			.Add(ContextAttrib::MajorVersion, 3)
			.Add(ContextAttrib::MinorVersion, 0)
			.Add(ContextFlag::OpenGLRobustAccess|ContextFlag::OpenGLDebug)
			.Add(OpenGLProfileBit::Core|OpenGLProfileBit::Compatibility)
			.Add(OpenGLResetNotificationStrategy::NoResetNotification)
			.Get()
	);

	context.MakeCurrent(surface);

	oglplus::GLAPIInitializer api_init;

	render_frame();

	context.WaitClient();

	save_frame(width, height, screenshot_path);
}

int main(int argc, char* argv[])
{
	try
	{
		glGetError();
		make_screenshot(
			800,
			600,
			(argc>1)?argv[1]:"screenshot.rgb"
		);
		return 0;
	}
	catch(oglplus::Error& oe)
	{
		std::cerr
			<< "OGLplus error (in "
			<< oe.GLFunc()
			<< "'): "
			<< oe.what()
			<< " ["
			<< oe.SourceFile()
			<< ":"
			<< oe.SourceLine()
			<< "] "
			<< std::endl;
	}
	catch(eglplus::Error& ee)
	{
		std::cerr
			<< "EGLplus error (in "
			<< ee.EGLFunc()
			<< ") "
			<< ee.what()
			<< " ["
			<< ee.SourceFile()
			<< ":"
			<< ee.SourceLine()
			<< "] "
			<< std::endl;
	}
	catch(std::exception& se)
	{
		std::cerr << "Error: " << se.what();
		std::cerr << std::endl;
	}
	return 1;
}

