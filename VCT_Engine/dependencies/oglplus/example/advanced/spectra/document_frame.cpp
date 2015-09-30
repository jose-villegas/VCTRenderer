/*
 *  .file advanced/spectra/document_frame.cpp
 *  .brief Implements the document frame
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "spectra_app.hpp"
#include "main_frame.hpp"
#include "document_frame.hpp"
#include "document.hpp"
#include "renderer.hpp"

#include <wx/stockitem.h>

void SpectraDocumentFrame::SetStatus(const wxString& status_text)
{
	status_bar->SetStatusText(status_text);
}

static const
int SpectraDocFrameID_MenuFile = wxID_HIGHEST+1001;
static const
int SpectraDocFrameID_Play = SpectraDocFrameID_MenuFile+1;

void SpectraDocumentFrame::InitMainMenu(void)
{
	wxMenu* file_menu = new wxMenu();
	file_menu->Append(wxID_ADD, wxGetTranslation(wxT("New &view\tCtrl-N")));
	file_menu->AppendSeparator();
	play_menu_item = file_menu->Append(
		SpectraDocFrameID_Play,
		wxGetTranslation(wxT("&Play\tCtrl-P"))
	);
	file_menu->AppendSeparator();
	file_menu->Append(wxID_CLOSE);

	main_menu->Append(file_menu, wxGetStockLabel(wxID_FILE));

	SetMenuBar(main_menu);
}

void SpectraDocumentFrame::ConnectEventHandlers(void)
{
	Connect(
		wxID_CLOSE,
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(SpectraDocumentFrame::DoClose)
	);
	Connect(
		wxID_ADD,
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(SpectraDocumentFrame::DoDuplicate)
	);
	Connect(
		wxEVT_CLOSE_WINDOW,
		wxCloseEventHandler(SpectraDocumentFrame::OnClose)
	);
	Connect(
		wxEVT_SYS_COLOUR_CHANGED,
		wxSysColourChangedEventHandler(SpectraDocumentFrame::OnSysColorChange)
	);
	Connect(
		wxEVT_MOTION,
		wxMouseEventHandler(SpectraDocumentFrame::OnMouseMotionEvent)
	);
	Connect(
		wxEVT_MOUSEWHEEL,
		wxMouseEventHandler(SpectraDocumentFrame::OnMouseWheelEvent)
	);
	Connect(
		wxEVT_SIZE,
		wxSizeEventHandler(SpectraDocumentFrame::OnResize)
	);

	Connect(
		SpectraDocFrameID_Play,
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(SpectraDocumentFrame::DoPlay)
	);
}

void SpectraDocumentFrame::InitComponents(void)
{
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	main_panel->SetClientSize(640, 480);
	main_sizer->Add(main_panel, 1, wxEXPAND);

	main_sizer->Add(status_bar, 0, wxEXPAND);

	wxBoxSizer* gl_sizer = new wxBoxSizer(wxVERTICAL);
	gl_sizer->Add(gl_canvas, 1, wxEXPAND);
	main_panel->SetSizer(gl_sizer);

	SetSize(
		main_panel->GetSize().GetWidth(),
		main_menu->GetSize().GetHeight()+1+
		main_panel->GetSize().GetHeight()+1+
		status_bar->GetSize().GetHeight()+1
	);
	SetSizer(main_sizer);
	Layout();
}

void SpectraDocumentFrame::DoClose(wxCommandEvent&)
{
	Close();
}

void SpectraDocumentFrame::OnClose(wxCloseEvent&)
{
	assert(main_frame);
	main_frame->ForgetDocumentFrame(this);
	Destroy();
}

void SpectraDocumentFrame::DoDuplicate(wxCommandEvent&)
{
	struct DocVisGetter
	{
		std::shared_ptr<SpectraVisualisation> doc_vis;

		std::shared_ptr<SpectraVisualisation> operator()(
			SpectraApp&,
			SpectraMainFrame*,
			wxGLCanvas*,
			wxGLContext*
		) const
		{
			return doc_vis;
		}
	} doc_vis_getter = { document_vis };

	main_frame->RegisterDocumentFrame(
		new SpectraDocumentFrame(
			parent_app,
			main_frame,
			parent_context,
			doc_vis_getter,
			main_frame->LazyRendererPicker()
		)
	);
}

void SpectraDocumentFrame::HandleResize(void)
{
	wxSize size = gl_canvas->GetSize();
	document_view.Reshape(size.GetWidth(), size.GetHeight());
}

void SpectraDocumentFrame::OnResize(wxSizeEvent& event)
{
	try
	{
		HandleResize();
		Update();
		event.Skip();
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err, this); }
	catch(const std::exception& se) { parent_app.HandleError(se, this); }
}

void SpectraDocumentFrame::HandleSysColorChange(void)
{
	renderer->ReinitStyle();
}

void SpectraDocumentFrame::OnSysColorChange(wxSysColourChangedEvent& event)
{
	try
	{
		HandleSysColorChange();
		Update();
		event.Skip();
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err, this); }
	catch(const std::exception& se) { parent_app.HandleError(se, this); }
}

void SpectraDocumentFrame::DoPlay(wxCommandEvent&)
{
	try
	{
		assert(document_vis);
		document_vis->Play();
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err, this); }
	catch(const std::exception& se) { parent_app.HandleError(se, this); }
}

GLint SpectraDocumentFrame::ClampMouseCoord(GLint c, GLint m)
{
	if(c < 0) return 0;
	if(c > m) return m;
	return c;
}

void SpectraDocumentFrame::HandleMouseMotion(const wxMouseEvent& event)
{
	wxSize size = gl_canvas->GetSize();
	wxPoint new_mouse_position = event.GetPosition();

	int new_x = ClampMouseCoord(new_mouse_position.x, size.GetWidth());
	int new_y = ClampMouseCoord(new_mouse_position.y, size.GetHeight());
	if(event.Dragging())
	{
		int old_x = ClampMouseCoord(old_mouse_position.x, size.GetWidth());
		int old_y = ClampMouseCoord(old_mouse_position.y, size.GetHeight());

		if(event.ShiftDown())
		{
			if(event.LeftIsDown())
			{
				float new_t = document_view.PickOnGround(new_x, new_y).x();
				float old_t = document_view.PickOnGround(old_x, old_y).x();
				new_t /= document_view.TimeStretch();
				old_t /= document_view.TimeStretch();
				document_vis->DragSelection(new_t, new_t-old_t);
				SetStatus(
					wxString::Format(
						wxGetTranslation(wxT("Selection: <%f, %f> [s]")),
						document_vis->SelectionBegin(),
						document_vis->SelectionEnd()
					)
				);
			}
		}
		else if(event.CmdDown())
		{
			if(event.LeftIsDown())
			{
				if(event.AltDown())
				{
					document_view.StretchRange(new_x, new_y, old_x, old_y);
				}
				else
				{
					document_view.StretchDomain(new_x, new_y, old_x, old_y);
				}
			}
		}
		else
		{
			if(event.LeftIsDown())
			{
				if(event.AltDown())
				{
					document_view.Elevate(new_x, new_y, old_x, old_y);
				}
				else
				{
					document_view.Slide(new_x, new_y, old_x, old_y);
				}
			}
			else if(event.MiddleIsDown())
			{
				document_view.Scale(new_x, new_y, old_x, old_y);
			}
			else if(event.RightIsDown())
			{
				document_view.Orbit(new_x, new_y, old_x, old_y);
			}
		}
	}
	else
	{
		float t = 0.0f;
		if(event.AltDown())
		{
			t = document_view.PickOnWall(new_x, new_y).x();
		}
		else
		{
			t = document_view.PickOnGround(new_x, new_y).x();
		}
		t /= document_view.TimeStretch();
		SetStatus(wxString::Format(wxGetTranslation(wxT("Time: %f [s]")), t));
		document_vis->SelectedTime(t);
	}
}

void SpectraDocumentFrame::OnMouseMotionEvent(wxMouseEvent& event)
{
	try
	{
		if(renderer->Interactive())
			HandleMouseMotion(event);
		event.Skip();
		Update();
		old_mouse_position = event.GetPosition();
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err, this); }
	catch(const std::exception& se) { parent_app.HandleError(se, this); }
}

void SpectraDocumentFrame::HandleMouseWheel(const wxMouseEvent& event)
{
	document_view.Zoom(
		GLfloat(event.GetLinesPerAction())*
		GLfloat(event.GetWheelRotation())/
		GLfloat(event.GetWheelDelta())
	);
}

void SpectraDocumentFrame::OnMouseWheelEvent(wxMouseEvent& event)
{
	try
	{
		HandleMouseWheel(event);
		event.Skip();
		Update();
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err, this); }
	catch(const std::exception& se) { parent_app.HandleError(se, this); }
}


void SpectraDocumentFrame::Update(void)
{
	renderer->Render(document_view, gl_canvas);

	gl_canvas->SwapBuffers();
}

void SpectraDocumentFrame::OnIdle(wxIdleEvent&)
{
	try
	{
		if(idle_call_count == 0)
		{
			idle_call_count++;
			Update();
			idle_call_count--;
		}
		return;
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err); }
	catch(const std::exception& se) { parent_app.HandleError(se); }

	Close();
}


SpectraDocumentFrame::SpectraDocumentFrame(
	SpectraApp& app,
	SpectraMainFrame* parent,
	wxGLContext* parent_ctxt,
	const SpectraDocumentFrame::VisualisationGetter& get_doc_vis,
	const SpectraDocumentFrame::RendererGetter& get_renderer
): wxFrame(
	(wxWindow*)parent,
	wxID_ANY,
	wxGetTranslation(wxT("Opening document")),
	wxDefaultPosition,
	wxDefaultSize
), parent_app(app)
 , main_frame(parent)
 , main_menu(new wxMenuBar())
 , main_panel(
	new wxPanel(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize
	)
), status_bar(new wxStatusBar(this))
 , parent_context(parent_ctxt)
 , gl_canvas(
	new SpectraDocumentCanvas(
		(wxEvtHandler*)this,
		(wxWindow*)main_panel
	)
), document_vis()
 , renderer()
 , idle_call_count(0)
{
	assert(gl_canvas);

	InitMainMenu();
	InitComponents();
	Show();

	document_vis = get_doc_vis(
		parent_app,
		main_frame,
		gl_canvas,
		parent_context
	);
	assert(document_vis);
	document_vis->AddCanvas(gl_canvas);
	assert(play_menu_item);
	play_menu_item->Enable(document_vis->Document().CanPlay());

	renderer = get_renderer(
		parent_app,
		main_frame,
		document_vis,
		gl_canvas
	);
	assert(renderer);

	document_view.SetMaxTime(document_vis->Document().MaxTime());

	ConnectEventHandlers();

	HandleResize();

	SetTitle(document_vis->Name());
	SetStatus(wxT("TODO: short document info"));
}

SpectraDocumentFrame::~SpectraDocumentFrame(void)
{
	// this ensures that the GL objects from the context
	// are cleaned up properly in case this is the
	// last reference to this instance of doc. vis.
	gl_canvas->SetCurrent(document_vis->GLContext());
	document_vis->RemoveCanvas(gl_canvas);
}
