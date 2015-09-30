/**
 *  @example standalone/001_triangle_wxgl_glew.cpp
 *  @brief Shows the basic usage of OGLplus with wxWidgets and GLEW.
 *
 *  @oglplus_screenshot{001_triangle}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <GL/glew.h>
#include <oglplus/all.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

class Example
{
private:
	oglplus::Context gl;

	// Vertex shader
	oglplus::VertexShader vs;

	// Fragment shader
	oglplus::FragmentShader fs;

	// Program
	oglplus::Program prog;

	// A vertex array object for the rendered triangle
	oglplus::VertexArray triangle;
	// VBO for the triangle's vertices
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
		Buffer::Data(Buffer::Target::Array, 9, triangle_verts);
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

	void Reshape(GLuint w, GLuint h)
	{
		gl.Viewport(w, h);
	}
};

class ExampleFrame : public wxFrame
{
private:
	wxPanel* oglPanel;
	wxGLCanvas* oglCanvas;
	wxGLContext oglContext;

	static int* get_gl_config(void)
	{
		static int conf[] = {
			WX_GL_RGBA,
			WX_GL_DOUBLEBUFFER,
			WX_GL_DEPTH_SIZE, 24,
			0
		};
		return conf;
	}

	Example* oglExample;

	void Update(void)
	{
		if(oglExample) oglExample->Display();
		oglCanvas->SwapBuffers();
	}

	void OnIdle(wxIdleEvent& event)
	{
		Update();
		wxYield();
		wxMilliSleep(1);
		if(oglExample) event.RequestMore();
	}

	void OnResize(wxSizeEvent& event)
	{
		if(oglExample) oglExample->Reshape(GetSize().x, GetSize().y);
		event.Skip();
	}
public:
	ExampleFrame(void)
	 : wxFrame(
		(wxWindow*)0,
		wxID_ANY,
		wxT("OGLplus + wxGL"),
		wxDefaultPosition,
		wxSize(800, 600)
	), oglPanel(
		new wxPanel(
			(wxWindow*)this,
			wxID_ANY,
			wxDefaultPosition,
			wxDefaultSize
		)
	), oglCanvas(
		new wxGLCanvas(
			(wxWindow*)oglPanel,
			wxID_ANY,
			get_gl_config(),
			wxDefaultPosition,
			wxDefaultSize
		)
	), oglContext(oglCanvas)
	 , oglExample(0)
	{
		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
		mainSizer->Add(oglPanel, 1, wxEXPAND);
		SetSizer(mainSizer);
		Show();
		//
		wxBoxSizer* oglSizer = new wxBoxSizer(wxVERTICAL);
		oglSizer->Add(oglCanvas, 1, wxEXPAND);
		oglPanel->SetSizer(oglSizer);
		//
		oglContext.SetCurrent(*oglCanvas);
		//
		if(glewInit() != GLEW_OK) throw std::runtime_error("GLEW init error");
		glGetError();
		//
		oglExample = new Example();
		//
		Connect(
			wxEVT_SIZE,
			wxSizeEventHandler(ExampleFrame::OnResize)
		);
		SetExtraStyle(wxWS_EX_PROCESS_IDLE);
		wxIdleEvent::SetMode(wxIDLE_PROCESS_SPECIFIED);
		Connect(
			wxEVT_IDLE,
			wxIdleEventHandler(ExampleFrame::OnIdle)
		);
		Layout();
		oglExample->Reshape(GetSize().x, GetSize().y);
	}

	~ExampleFrame(void)
	{
		Disconnect(wxEVT_SIZE);
		Disconnect(wxEVT_IDLE);
		wxIdleEvent::SetMode(wxIDLE_PROCESS_ALL);
		wxYield();
		wxSafeYield();
		wxYield();
		delete oglExample;
		oglExample = nullptr;
	}
};

class ExampleApp : public wxApp
{
public:
	bool OnInit(void)
	{
		bool result = false;
		try
		{
			SetTopWindow(new ExampleFrame());
			result = true;
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
		catch(const std::exception& se)
		{
			std::cerr << "Unspecified error: " << se.what() << std::endl;
		}
		return result;
	}
};

IMPLEMENT_APP(ExampleApp)
