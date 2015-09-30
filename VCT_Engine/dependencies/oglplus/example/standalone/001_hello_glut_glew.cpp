/**
 *  @example standalone/001_hello_glut_glew.cpp
 *  @brief Shows the basic usage of OGLplus with GLUT and GLEW.
 *
 *  See the @ref oglplus_tut_001_glut_glew tutorial for a detailed explanation
 *  of the source code of this example.
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <oglplus/all.hpp>

#include <oglplus/opt/resources.hpp>
#include <oglplus/text/font2d.hpp>

class Example
{
private:
	GLint width, height;

	oglplus::ResourceFile font_file;
	oglplus::text::Font2D font;

	oglplus::Context gl;

	oglplus::Texture tex;
	oglplus::Framebuffer fbo;
public:
	Example(GLint w, GLint h)
	 : width(w)
	 , height(h)
	 , font_file("fonts", "FreeSans", ".ttf")
	 , font(font_file)
	{
		using namespace oglplus;

		tex.Bind(TextureTarget::Rectangle);
		fbo.Bind(FramebufferTarget::Read);

		PrepareText();
	}

	void PrepareText(void)
	{
		using namespace oglplus;

		text::CodePoints code_points = text::UTF8ToCodePoints("Hello World!");
		text::Font2D::Layout layout = font.MakeLayout(code_points);

		std::size_t font_size = height-1;
		std::vector<unsigned char> buffer(width*height, 0x00);

		font.Render(font_size, layout, buffer.data(), width, height);

		Texture::Image2D(
			TextureTarget::Rectangle,
			0,
			PixelDataInternalFormat::Red,
			width, height,
			0,
			PixelDataFormat::Red,
			PixelDataType::UnsignedByte,
			buffer.data()
		);

		Framebuffer::AttachTexture(
			FramebufferTarget::Read,
			FramebufferAttachment::Color,
			tex,
			0
		);
	}

	void Display(void)
	{
		using namespace oglplus;

		gl.BlitFramebuffer(
			0, 0, width, height,
			0, height, width, 0,
			BufferSelectBit::ColorBuffer,
			BlitFilter::Nearest
		);
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

	SingleExample(GLint width, GLint height)
	{
		assert(!SingleInstance());
		SingleInstance() = new Example(width, height);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	GLint width = 800;
	GLint height = 150;

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OGLplus+GLUT+GLEW");

	if(glewInit() == GLEW_OK) try
	{
		glGetError();

		SingleExample example(width, height);
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

