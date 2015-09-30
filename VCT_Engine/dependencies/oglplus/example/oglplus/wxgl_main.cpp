/*
 *  .file example/oglplus/wxgl_main.cpp
 *  Implements wxGL-based program main function for running examples
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>

#include <oglplus/config/site.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/recguard.h>
#include <wx/glcanvas.h>

#include <cassert>
#include <iostream>
#include <iomanip>

#include <oglplus/error/basic.hpp>

#include <oglplus/query.hpp>

#include <oglplus/os/steady_clock.hpp>

#include "example.hpp"

// helper interface for displaying information about the example
struct ExampleInfoDisplay
{
	virtual ~ExampleInfoDisplay(void){ }

	virtual void ExampleFinish(bool ok) = 0;

	virtual void MouseMove(
		unsigned mouse_x,
		unsigned mouse_y
	) = 0;

	virtual void Resize(
		unsigned width,
		unsigned height
	) = 0;

	virtual void FrameUpdate(
		unsigned frame_no,
		double sim_time,
		double real_time
	) = 0;

	virtual void RenderStats(
		double fps,
		double pps,
		double ppf
	) = 0;
};

// configuration options for wxGL
static inline int* GLConfig(void)
{
	static int conf[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE, 24,
		WX_GL_STENCIL_SIZE, 8,
		0
	};
	return conf;
}

inline wxString MakeCommonErrorMessage(oglplus::Error& error)
{
	wxString message;
	message.Append(wxString::Format(
		wxT("Error: '%s'\r\n"),
		wxString(error.what(), wxConvLocal).c_str()
	));
	if(error.SourceFile())
	{
		message.Append(wxString::Format(
			wxT("Source file: '%s'\r\n"),
			wxString(error.SourceFile(), wxConvLocal).c_str()
		));
	}

	if(error.SourceLine())
	{
		message.Append(wxString::Format(
			wxT("Source line: '%d'\r\n"),
			error.SourceLine()
		));
	}

	if(error.SourceFunc())
	{
		message.Append(wxString::Format(
			wxT("Source function: '%s'\r\n"),
			wxString(error.SourceFunc(), wxConvLocal).c_str()
		));
	}
	if(error.GLFunc())
	{
		message.Append(wxString::Format(
			wxT("GL function: '%s'\r\n"),
			wxString(error.GLFunc(), wxConvLocal).c_str()
		));
	}

	if(error.EnumParam() || error.EnumParamName())
	{
		message.Append(wxT("GL constant: "));
		if(error.EnumParamName())
		{
			message.Append(wxString::Format(
				wxT("'%s'\r\n"),
				wxString(error.EnumParamName(), wxConvLocal).c_str()
			));
		}
		else
		{
			message.Append(wxString::Format(
				wxT("(0x%x)\r\n"),
				error.EnumParam()
			));
		}
	}

	if(error.BindTarget() || error.TargetName())
	{
		message.Append(wxT("Binding point: "));
		if(error.TargetName())
		{
			message.Append(wxString::Format(
				wxT("'%s'\r\n"),
				wxString(error.TargetName(), wxConvLocal).c_str()
			));
		}
		else
		{
			message.Append(wxString::Format(
				wxT("(0x%x)\r\n"),
				error.BindTarget()
			));
		}
	}

	if(error.ObjectTypeName() || error.ObjectType())
	{
		message.Append(wxT("Object type: "));
		if(error.ObjectTypeName())
		{
			message.Append(wxString::Format(
				wxT("'%s'\r\n"),
				wxString(error.ObjectTypeName(), wxConvLocal).c_str()
			));
		}
		else
		{
			message.Append(wxString::Format(
				wxT("(0x%x)\r\n"),
				error.ObjectType()
			));
		}
	}

	if((!error.ObjectDesc().empty()) || (error.ObjectName() >= 0))
	{
		message.Append(wxT("Object: "));
		if(!error.ObjectDesc().empty())
		{
			message.Append(wxString::Format(
				wxT("'%s'\r\n"),
				wxString(error.ObjectDesc().c_str(), wxConvLocal).c_str()
			));
		}
		else
		{
			message.Append(wxString::Format(
				wxT("(%d)\r\n"),
				error.ObjectName()
			));
		}
	}

	if(error.SubjectTypeName() || error.SubjectType())
	{
		message.Append(wxT("Subject type: "));
		if(error.SubjectTypeName())
		{
			message.Append(wxString::Format(
				wxT("'%s'\r\n"),
				wxString(error.SubjectTypeName(), wxConvLocal).c_str()
			));
		}
		else
		{
			message.Append(wxString::Format(
				wxT("(%d)\r\n"),
				error.SubjectType()
			));
		}
	}

	if((!error.SubjectDesc().empty()) || (error.SubjectName() >= 0))
	{
		message.Append(wxT("Subject: "));
		if(!error.SubjectDesc().empty())
		{
			message.Append(wxString::Format(
				wxT("'%s'\r\n"),
				wxString(error.SubjectDesc().c_str(), wxConvLocal).c_str()
			));
		}
		else
		{
			message.Append(wxString::Format(
				wxT("(%d)\r\n"),
				error.SubjectName()
			));
		}
	}

	return message;
}

inline void HandleError(
	oglplus::Error& error,
	wxWindow* parent = nullptr
)
{
	wxMessageBox(
		MakeCommonErrorMessage(error),
		wxT("GL Error"),
		wxOK | wxICON_ERROR,
		parent
	);
}

inline void HandleError(
	const std::exception& error,
	wxWindow* parent = nullptr
)
{
	wxMessageBox(
		wxString(error.what(), wxConvLocal),
		wxT("Unspecified error"),
		wxOK | wxICON_ERROR,
		parent
	);
}

// frame with the GL canvas where the rendering is done
class ExampleFrame
 : public wxFrame
{
private:
	ExampleInfoDisplay* info_display;
	wxPanel* main_panel;

	class ExampleGLCanvas
	 : public wxGLCanvas
	{
	private:
		wxEvtHandler* evt_handler;

		void OnMouseEvent(wxMouseEvent& event)
		{
			evt_handler->ProcessEvent(event);
		}
	public:
		ExampleGLCanvas(
			wxEvtHandler* handler,
			wxWindow* parent
		): wxGLCanvas(
			parent,
			wxID_ANY,
			GLConfig(),
			wxDefaultPosition,
			wxDefaultSize
		), evt_handler(handler)
		{
			Connect(
				wxEVT_MOTION,
				wxMouseEventHandler(ExampleGLCanvas::OnMouseEvent)
			);
		}
	}* gl_canvas;
	wxGLContext* gl_context;


	oglplus::os::steady_clock os_clock;
	oglplus::ExampleClock example_clock;
	std::unique_ptr<oglplus::Example> example;
	unsigned frame_no, fps_frame_no;
	double fps_time, prim_count;

	oglplus::Query primitive_query;

	void Update(void)
	{
		assert(example);
		assert(info_display);
		assert(gl_canvas);

		example_clock.Update(os_clock.seconds());
		example->Continue(example_clock);
		GLuint primitives_per_frame = 0;
		{
			auto query_exec = primitive_query.Execute(
				oglplus::Query::Target::PrimitivesGenerated,
				primitives_per_frame
			);
			example->Render(example_clock);
			gl_canvas->SwapBuffers();
		}
		prim_count += double(primitives_per_frame);

		const double frame_time = example_clock.Time();
		const double real_time = example_clock.RealTime().Seconds();
		const double fps_interval = 5.0;
		const double this_interval = real_time - fps_time;
		if(this_interval >= fps_interval)
		{
			fps_time = real_time;
			info_display->RenderStats(
				fps_frame_no/this_interval,
				prim_count/this_interval,
				prim_count/fps_frame_no
			);

			fps_frame_no = 0;
			prim_count = 0.0;
		}
		info_display->FrameUpdate(frame_no, frame_time, real_time);
		++frame_no;
		++fps_frame_no;
	}

	int idle_call_count;

	void OnIdle(wxIdleEvent& event)
	{
		try
		{
			if(idle_call_count == 0)
			{
				idle_call_count++;
				Update();
				event.RequestMore();
				idle_call_count--;
			}
			return;
		}
		catch(oglplus::Error& err) { HandleError(err); }
		catch(const std::exception& se) { HandleError(se); }

		Close();
	}

	void HandleResize(void)
	{
		assert(example);
		assert(info_display);

		wxSize size = gl_canvas->GetSize();
		example->Reshape(size.GetWidth(), size.GetHeight());
		info_display->Resize(size.GetWidth(), size.GetHeight());
	}

	void OnResize(wxSizeEvent& event)
	{
		try
		{
			HandleResize();
			event.Skip();
			return;
		}
		catch(oglplus::Error& err) { HandleError(err, this); }
		catch(const std::exception& se) { HandleError(se, this); }
	}

	void HandleMouseMove(wxPoint position)
	{
		assert(example);
		assert(info_display);

		wxSize size = gl_canvas->GetSize();
		if(position.x < 0) position.x = 0;
		if(position.y < 0) position.y = 0;
		if(position.x > size.GetWidth())  position.x = size.GetWidth();
		if(position.y > size.GetHeight()) position.y = size.GetHeight();
		example->MouseMove(
			position.x,
			size.GetHeight()-
			position.y,
			size.GetWidth(),
			size.GetHeight()
		);
		info_display->MouseMove(
			position.x,
			size.GetHeight()-
			position.y
		);
	}

	void OnMouseEvent(wxMouseEvent& event)
	{
		try
		{
			HandleMouseMove(event.GetPosition());
			event.Skip();
			return;
		}
		catch(oglplus::Error& err) { HandleError(err, this); }
		catch(const std::exception& se) { HandleError(se, this); }
	}

	void OnClose(wxCloseEvent&)
	{
		assert(info_display);

		Disconnect(wxEVT_IDLE);
		info_display->ExampleFinish(idle_call_count == 0);
		Destroy();
	}
public:
	ExampleFrame(
		wxApp& app,
		wxWindow* parent,
		const wxString& title,
		ExampleInfoDisplay* info_disp,
		wxGLContext* context
	): wxFrame(
		parent,
		wxID_ANY,
		title,
		wxDefaultPosition,
		wxDefaultSize
	), info_display(info_disp)
	 , main_panel(
		new wxPanel(
			this,
			wxID_ANY,
			wxDefaultPosition,
			wxDefaultSize
		)
	), gl_canvas(
		new ExampleGLCanvas(
			(wxEvtHandler*)this,
			(wxWindow*)main_panel
		)
	), gl_context(context)
	 , frame_no(0)
	 , fps_frame_no(0)
	 , fps_time(0.0)
	 , prim_count(0.0)
	 , idle_call_count(0)
	{
		assert(info_display);
		assert(gl_canvas);
		assert(gl_context);

		wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
		main_panel->SetClientSize(800, 600);
		main_sizer->Add(main_panel, 1, wxEXPAND);

		wxStatusBar* status_bar = new wxStatusBar(this);
		main_sizer->Add(status_bar, 0, wxEXPAND);

		wxBoxSizer* gl_sizer = new wxBoxSizer(wxVERTICAL);
		gl_sizer->Add(gl_canvas, 1, wxEXPAND);
		main_panel->SetSizer(gl_sizer);

		SetSize(
			main_panel->GetSize().GetWidth(),
			main_panel->GetSize().GetHeight()+
			status_bar->GetSize().GetHeight()
		);
		SetSizer(main_sizer);
		Layout();
		Show();

		gl_context->SetCurrent(*gl_canvas);

		// TODO: this won't work very well in "UNICODE" builds
		oglplus::ExampleParams params(app.argc, (char**)app.argv);
		example = oglplus::makeExample(params);
		os_clock.reset();

		HandleResize();

		Connect(
			wxEVT_CLOSE_WINDOW,
			wxCloseEventHandler(ExampleFrame::OnClose)
		);
		Connect(
			wxEVT_MOTION,
			wxMouseEventHandler(ExampleFrame::OnMouseEvent)
		);
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
	}

	~ExampleFrame(void)
	{
		wxIdleEvent::SetMode(wxIDLE_PROCESS_ALL);
	}
};

class MainFrame
 : public wxFrame
 , virtual ExampleInfoDisplay
{
private:
	wxStatusBar* status_bar;

	void SetStatus(const wxString& status_text)
	{
		status_bar->SetStatusText(status_text);
	}

	wxStaticText* mouse_x_label;
	wxStaticText* mouse_y_label;
	wxStaticText* vp_width_label;
	wxStaticText* vp_height_label;
	wxStaticText* vp_aspect_label;
	wxStaticText* vp_pixnum_label;
	wxStaticText* real_time_label;
	wxStaticText* frame_time_label;
	wxStaticText* frame_no_label;
	wxStaticText* fps_label;
	wxStaticText* pps_label;
	wxStaticText* ppf_label;

	void MouseMove(unsigned mouse_x, unsigned mouse_y)
	{
		mouse_x_label->SetLabel(wxString::Format(
			wxT("%u"),
			mouse_x
		));
		mouse_y_label->SetLabel(wxString::Format(
			wxT("%u"),
			mouse_y
		));
	}

	void Resize(unsigned width, unsigned height)
	{
		vp_width_label->SetLabel(wxString::Format(
			wxT("%u"),
			width
		));
		vp_height_label->SetLabel(wxString::Format(
			wxT("%u"),
			height
		));
		vp_aspect_label->SetLabel(wxString::Format(
			wxT("%.3f"),
			double(width)/double(height)
		));
		vp_pixnum_label->SetLabel(wxString::Format(
			wxT("%u"),
			width*height)
		);
	}

	void FrameUpdate(unsigned frame_no, double frame_time, double real_time)
	{
		frame_no_label->SetLabel(wxString::Format(
			wxT("%u"),
			frame_no
		));
		frame_time_label->SetLabel(wxString::Format(
			wxT("%.1f"),
			frame_time
		));
		real_time_label->SetLabel(wxString::Format(
			wxT("%.1f"),
			real_time
		));
	}

	void RenderStats(double fps, double pps, double ppf)
	{
		fps_label->SetLabel(wxString::Format(
			wxT("%.1f"),
			fps
		));
		pps_label->SetLabel(wxString::Format(
			wxT("%.1f"),
			pps
		));
		ppf_label->SetLabel(wxString::Format(
			wxT("%.1f"),
			ppf
		));
	}

	void ExampleFinish(bool ok)
	{
		example_frame = nullptr;
		if(ok) SetStatus(wxT("Finished"));
		else SetStatus(wxT("Error"));
	}

	wxStaticText* AddKeyValue(
		wxWindow* parent,
		wxSizer* sizer,
		const wxString& label,
		const wxString& tool_tip,
		const wxString& value
	)
	{
		wxStaticText* name_label = new wxStaticText(
			parent,
			wxID_ANY,
			label,
			wxDefaultPosition,
			wxDefaultSize,
			wxALIGN_LEFT
		);
		wxFont name_font = name_label->GetFont();
		name_font.SetWeight(wxFONTWEIGHT_BOLD);
		name_label->SetFont(name_font);

		if(!tool_tip.empty()) name_label->SetToolTip(tool_tip);
		sizer->Add(name_label, 1, wxEXPAND);

		wxStaticText* value_label = new wxStaticText(
			parent,
			wxID_ANY,
			value,
			wxDefaultPosition,
			wxDefaultSize,
			wxALIGN_RIGHT
		);
		sizer->Add(value_label, 1, wxEXPAND);
		return value_label;
	}

	wxGLCanvas* tmp_canvas;
	wxGLContext gl_context;

	void InitComponents(const wxString& example_name)
	{
		SetMinSize(wxSize(200, 300));

		const int em = 5; // todo
		const int em_2 = em/2;
		const wxString n_a(wxT("N/A"));


		wxBoxSizer* td_sizer = new wxBoxSizer(wxVERTICAL);
		td_sizer->AddSpacer(em);
		wxBoxSizer* lr_sizer = new wxBoxSizer(wxHORIZONTAL);
		lr_sizer->AddSpacer(em);

		wxPanel* main_panel = new wxPanel(this);
		wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

		wxPanel* example_info_panel = new wxPanel(main_panel);
		wxGridSizer* example_info_sizer = new wxGridSizer(2, em_2, em);

		AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Example name"),
			wxT("The name of the example"),
			example_name
		);
		mouse_x_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Mouse X"),
			wxT("Displays the X coordinate of the mouse over the OpenGL viewport"),
			n_a
		);
		mouse_y_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Mouse Y"),
			wxT("Displays the Y coordinate of the mouse over the OpenGL viewport"),
			n_a
		);
		vp_width_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Viewport width"),
			wxT("Displays the width of the OpenGL viewport"),
			n_a
		);
		vp_height_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Viewport height"),
			wxT("Displays the height of the OpenGL viewport"),
			n_a
		);
		vp_aspect_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Viewport aspect ratio"),
			wxT("Displays the aspect ratio of the OpenGL viewport"),
			n_a
		);
		vp_pixnum_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Viewport pixel count"),
			wxT("Displays the number of pixels in the OpenGL viewport"),
			n_a
		);
		real_time_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Real time"),
			wxT("Displays the elapsed real time in seconds"),
			n_a
		);
		frame_time_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Simulation time"),
			wxT("Displays the elapsed simulation time in seconds"),
			n_a
		);
		frame_no_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Frame number"),
			wxT("Displays the current frame number"),
			n_a
		);
		fps_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Frames per second"),
			wxT("Displays the average number of frames rendered per second"),
			n_a
		);
		pps_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Primitives per second"),
			wxT("Displays the average number of geometric primitives rendered per second"),
			n_a
		);
		ppf_label = AddKeyValue(
			example_info_panel,
			example_info_sizer,
			wxT("Primitives per frame"),
			wxT("Displays the average number of geometric primitives rendered per frame"),
			n_a
		);

		example_info_panel->SetSizer(example_info_sizer);

		main_sizer->Add(example_info_panel, 0, wxEXPAND);
		main_panel->SetSizer(main_sizer);

		lr_sizer->Add(main_panel, 1, wxEXPAND);
		lr_sizer->AddSpacer(em);
		td_sizer->Add(lr_sizer, 1, wxEXPAND);
		td_sizer->AddSpacer(em);
		td_sizer->Add(status_bar, 0, wxEXPAND);

		td_sizer->SetSizeHints(this);
		SetSizer(td_sizer);
		Layout();
	}

	oglplus::GLAPIInitializer *api_init;

	ExampleFrame *example_frame;

	void OnClose(wxCloseEvent& event)
	{
		if(example_frame)
		{
			example_frame->Close();
			if(event.CanVeto())
				event.Veto();
			else Destroy();
		}
		else Destroy();
	}
public:
	MainFrame(
		wxApp& app,
		const wxString& example_name
	): wxFrame(
		(wxWindow*)0,
		wxID_ANY,
		wxT("OGLplus example"),
		wxDefaultPosition,
		wxSize(300, 600)
	), status_bar(new wxStatusBar(this))
	 , tmp_canvas(
		new wxGLCanvas(
			(wxWindow*)this,
			wxID_ANY,
			GLConfig(),
			wxDefaultPosition,
			wxDefaultSize
		)
	), gl_context(tmp_canvas)
	 , api_init(nullptr)
	 , example_frame(nullptr)
	{
		InitComponents(example_name);
		Show();
		SetStatus(wxT("Initializing GL"));
		gl_context.SetCurrent(*tmp_canvas);

		api_init = new oglplus::GLAPIInitializer();

		delete tmp_canvas;
		SetStatus(wxT("Ready"));

		Connect(
			wxEVT_CLOSE_WINDOW,
			wxCloseEventHandler(MainFrame::OnClose)
		);

		example_frame = new ExampleFrame(
			app,
			(wxWindow*)this,
			example_name,
			(ExampleInfoDisplay*)this,
			&gl_context
		);

		SetStatus(wxT("Running"));
	}

	~MainFrame(void)
	{
		delete api_init;
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
			SetTopWindow(new MainFrame(*this, GetAppName()));
			result = true;
		}
		catch(oglplus::Error& err) { HandleError(err); }
		catch(const std::exception& se) { HandleError(se); }
		return result;
	}
};

IMPLEMENT_APP(ExampleApp)

