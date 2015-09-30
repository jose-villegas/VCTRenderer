/*
 *  .file advanced/spectra/main_frame.cpp
 *  .brief Implements the main frame
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "main_frame.hpp"
#include "coroutine.hpp"
#include "document_frame.hpp"
#include "document.hpp"
#include "default_renderer.hpp"
#include "xsection_renderer.hpp"
#include "visualisation.hpp"

#include <wx/stockitem.h>
#include <wx/filedlg.h>
#include <wx/aboutdlg.h>

// -- SpectraMainFrameDocumentLoader --
class SpectraMainFrameDocumentLoader
 : public SpectraCoroutine
{
private:
	SpectraMainFrame* main_frame;
	std::shared_ptr<SpectraDocument> document;
public:
	SpectraMainFrameDocumentLoader(
		SpectraMainFrame* frame,
		const std::shared_ptr<SpectraDocument>& doc
	);

	wxString Description(void) const;

	void Cancel(void);

	bool DoWork(void);

	int PercentDone(void) const;
};

SpectraMainFrameDocumentLoader::SpectraMainFrameDocumentLoader(
	SpectraMainFrame* frame,
	const std::shared_ptr<SpectraDocument>& doc
): main_frame(frame)
 , document(doc)
{
	assert(main_frame);
	assert(document);
}

wxString SpectraMainFrameDocumentLoader::Description(void) const
{
	return wxGetTranslation(wxT("Loading document"));
}

void SpectraMainFrameDocumentLoader::Cancel(void)
{
}

bool SpectraMainFrameDocumentLoader::DoWork(void)
{
	if(document->FinishLoading())
	{
		main_frame->OpenLoadedDocument(document);
		return true;
	}
	else return false;
}

int SpectraMainFrameDocumentLoader::PercentDone(void) const
{
	return document->PercentLoaded();
}


//-- SpectraMainFrame --

static const
int SpectraMainFrameID_MenuFile = wxID_HIGHEST+1;
static const
int SpectraMainFrameID_GenerateDoc = SpectraMainFrameID_MenuFile+1;

static const
int SpectraMainFrameID_MenuView = wxID_HIGHEST+100;
static const
int SpectraMainFrameID_DefaultRenderer = SpectraMainFrameID_MenuView+1;
static const
int SpectraMainFrameID_XSectionRenderer = SpectraMainFrameID_MenuView+2;

void SpectraMainFrame::SetStatus(const wxString& status_text)
{
	status_bar->SetStatusText(status_text);
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
	sizer->Add(value_label, 3, wxEXPAND);
	return value_label;
}

void SpectraMainFrame::InitMainMenu(void)
{
	wxMenu* file_menu = new wxMenu();
	file_menu->Append(wxID_OPEN);
	file_menu->Append(
		SpectraMainFrameID_GenerateDoc,
		wxGetTranslation(wxT("&Generate\tCtrl-Shift-G"), wxT("Document"))
	);
	file_menu->AppendSeparator();
	file_menu->Append(wxID_EXIT);

	main_menu->Append(
		file_menu,
		wxGetStockLabel(wxID_FILE)
	);

	wxMenu* view_menu = new wxMenu();

	renderer_menu = new wxMenu();
	renderer_menu->AppendRadioItem(
		SpectraMainFrameID_DefaultRenderer,
		wxGetTranslation(wxT("&Default renderer"), wxT("Menu"))
	);

	renderer_menu->AppendRadioItem(
		SpectraMainFrameID_XSectionRenderer,
		wxGetTranslation(wxT("&Cross-section renderer"), wxT("Menu"))
	);

	view_menu->AppendSubMenu(
		renderer_menu,
		wxGetTranslation(wxT("&Renderer"), wxT("Menu"))
	);

	main_menu->Append(
		view_menu,
		wxGetTranslation(wxT("&View"), wxT("Menu"))
	);

	wxMenu* help_menu = new wxMenu();
	help_menu->Append(wxID_ABOUT);

	main_menu->Append(
		help_menu,
		wxGetStockLabel(wxID_HELP)
	);

	SetMenuBar(main_menu);
}

void SpectraMainFrame::ConnectEventHandlers(void)
{
	Connect(
		wxID_EXIT,
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(SpectraMainFrame::DoClose)
	);
	Connect(
		wxID_OPEN,
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(SpectraMainFrame::DoOpenDocument)
	);
	Connect(
		SpectraMainFrameID_GenerateDoc,
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(SpectraMainFrame::DoGenerateDocument)
	);
	Connect(
		wxID_ABOUT,
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(SpectraMainFrame::DoShowAboutDialog)
	);
	Connect(
		wxEVT_CLOSE_WINDOW,
		wxCloseEventHandler(SpectraMainFrame::OnClose)
	);
	SetExtraStyle(wxWS_EX_PROCESS_IDLE);
	wxIdleEvent::SetMode(wxIDLE_PROCESS_SPECIFIED);
	Connect(
		wxEVT_IDLE,
		wxIdleEventHandler(SpectraMainFrame::OnIdle)
	);
}

void SpectraMainFrame::InitComponents(void)
{
	const int em = 5;
	const int em_2 = em/2;

	const wxString n_a(wxGetTranslation(wxT("N/A")));

	SetMinSize(wxSize(200, 300));

	wxBoxSizer* td_sizer = new wxBoxSizer(wxVERTICAL);
	td_sizer->AddSpacer(em);
	wxBoxSizer* lr_sizer = new wxBoxSizer(wxHORIZONTAL);
	lr_sizer->AddSpacer(em);

	wxPanel* main_panel = new wxPanel(this);
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* info_panel = new wxPanel(main_panel);
	wxFlexGridSizer* info_sizer = new wxFlexGridSizer(0, 2, em_2, em);

	gl_vendor = AddKeyValue(
		info_panel,
		info_sizer,
		wxGetTranslation(wxT("GL Vendor")),
		wxGetTranslation(wxT("The name of the GL implementation vendor")),
		n_a
	);

	gl_renderer = AddKeyValue(
		info_panel,
		info_sizer,
		wxGetTranslation(wxT("GL Renderer")),
		wxGetTranslation(wxT("The name of the GL renderer")),
		n_a
	);

	gl_version = AddKeyValue(
		info_panel,
		info_sizer,
		wxGetTranslation(wxT("GL Version")),
		wxGetTranslation(wxT("The version of GL supported by the renderer")),
		n_a
	);

	sl_version = AddKeyValue(
		info_panel,
		info_sizer,
		wxGetTranslation(wxT("SL Version")),
		wxGetTranslation(wxT("The version of GLSL supported by the renderer")),
		n_a
	);


	info_panel->SetSizer(info_sizer);

	main_sizer->Add(info_panel, 0, wxEXPAND);
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

void SpectraMainFrame::OnIdle(wxIdleEvent& event)
{
	if(!coroutine_exec->DoWork())
		event.RequestMore();
	for(auto* doc_frame : doc_frames)
		doc_frame->OnIdle(event);
}

void SpectraMainFrame::DoClose(wxCommandEvent&)
{
	Close();
}

void SpectraMainFrame::OnClose(wxCloseEvent& event)
{
	if(!doc_frames.empty())
	{
		for(auto& frame : doc_frames)
		{
			frame->Close();
		}
		if(event.CanVeto())
		{
			event.Veto();
		}
		else Destroy();
	}
	Destroy();
}

void SpectraMainFrame::DoShowAboutDialog(wxCommandEvent&)
{
    wxAboutDialogInfo info;
    info.SetName(wxGetTranslation(wxT("OGLplus spectra")));
    info.SetVersion(wxGetTranslation(wxT("0.0.1 Alpha")));
    info.SetDescription(wxGetTranslation(wxT("3D viewer of signal Fourier spectra.")));
    info.SetCopyright(wxGetTranslation(wxT("(C) 2013 Matus Chochlik")));
    info.SetWebSite(wxGetTranslation(wxT("http://oglplus.org/")));
    info.AddDeveloper(wxGetTranslation(wxT("Matus Chochlik")));

    wxAboutBox(info);
}

void SpectraMainFrame::DoOpenDocument(wxCommandEvent&)
{
	wxString doc_path = wxLoadFileSelector(
		wxGetTranslation(wxT("Select file to load"), wxT("Dialog")),
		wxT(".wav"),
		(const wxChar*)0,
		this
	); // TODO a customized dialog
	try
	{
		StartCoroutine(
			std::make_shared<SpectraMainFrameDocumentLoader>(
				this,
				SpectraLoadDocFromFile(doc_path)
			),
			false
		);
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err); }
	catch(const std::exception& se) { parent_app.HandleError(se); }
}

void SpectraMainFrame::DoGenerateDocument(wxCommandEvent&)
{
	try
	{
		struct TestSignal
		{
			float operator()(float x) const
			{
				return float(
					sin(3.1415*10.0*(sin(x)+2.0+x*x))*
					std::rand()/float(RAND_MAX)*
					std::rand()/float(RAND_MAX)
				);
			}
		};

		StartCoroutine(
			std::make_shared<SpectraMainFrameDocumentLoader>(
				this,
				SpectraOpenTestDoc(
					TestSignal(),
					11000,
					4.71f
				)
			),
			false
		);
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err); }
	catch(const std::exception& se) { parent_app.HandleError(se); }
}

void SpectraMainFrame::StartCoroutine(const std::shared_ptr<SpectraCoroutine>& coroutine)
{
	coroutine_exec->Start(coroutine, false); //TODO: get value of quiet from settings
}

void SpectraMainFrame::StartCoroutine(
	const std::shared_ptr<SpectraCoroutine>& coroutine,
	bool quiet
)
{
	coroutine_exec->Start(coroutine, quiet);
}

void SpectraMainFrame::OpenLoadedDocument(const std::shared_ptr<SpectraDocument>& document)
{
	struct DocVisMaker
	{
		std::shared_ptr<SpectraCalculator> calc;
		std::shared_ptr<SpectraDocument> doc;

		std::shared_ptr<SpectraVisualisation> operator()(
			SpectraApp& /*parent_app*/,
			SpectraMainFrame* main_frame,
			wxGLCanvas* canvas,
			wxGLContext* parent_context
		) const
		{
			return std::make_shared<SpectraVisualisation>(
				main_frame,
				canvas,
				parent_context,
				calc,
				doc
			);
		}
	} doc_vis_maker = {
		// let the use pick the sliding window size
		// or spectrum with and the spectrum calculator
		shared_objects->SpectrumCalculator(128),
		document
	};

	RegisterDocumentFrame(
		new SpectraDocumentFrame(
			this->parent_app,
			this,
			&this->gl_context,
			doc_vis_maker,
			LazyRendererPicker()
		)
	);
}

std::shared_ptr<SpectraRenderer> SpectraMainFrame::PickRenderer(
	SpectraApp& app,
	SpectraMainFrame* main_frame,
	const std::shared_ptr<SpectraVisualisation>& doc_vis,
	wxGLCanvas* canvas
)
{
	assert(main_frame == this);
	std::shared_ptr<SpectraRenderer> result;
	if(renderer_menu->IsChecked(SpectraMainFrameID_XSectionRenderer))
	{
		result = SpectraMakeXSectionRenderer(
			app,
			main_frame->shared_objects,
			doc_vis,
			canvas
		);
	}
	else
	{
		result = SpectraMakeDefaultRenderer(
			app,
			main_frame->shared_objects,
			doc_vis,
			canvas
		);
	}
	// TODO support for other renderers
	return result;
}

SpectraMainFrame::RendererGetter SpectraMainFrame::LazyRendererPicker(void)
{
	struct RendererPicker
	{
		std::shared_ptr<SpectraRenderer> operator()(
			SpectraApp& main_app,
			SpectraMainFrame* main_frame,
			const std::shared_ptr<SpectraVisualisation>& doc_vis,
			wxGLCanvas* canvas
		)
		{
			assert(main_frame);
			return main_frame->PickRenderer(
				main_app,
				main_frame,
				doc_vis,
				canvas
			);
		}
	};

	return RendererPicker();
}

void SpectraMainFrame::RegisterDocumentFrame(SpectraDocumentFrame* doc_frame)
{
	doc_frames.insert(doc_frame);
}

void SpectraMainFrame::ForgetDocumentFrame(SpectraDocumentFrame* doc_frame)
{
	doc_frames.erase(doc_frame);
}

int* SpectraGLConfig(void)
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

SpectraMainFrame::SpectraMainFrame(SpectraApp& app)
 : wxFrame(
	(wxWindow*)nullptr,
	wxID_ANY,
	wxT("OGLplus Spectra"),
	wxDefaultPosition,
	wxSize(300, 400)
), parent_app(app)
 , main_menu(new wxMenuBar())
 , status_bar(new wxStatusBar(this))
 , gl_canvas(
	new wxGLCanvas(
		(wxWindow*)this,
		wxID_ANY,
		SpectraGLConfig(),
		wxDefaultPosition,
		wxDefaultSize
	)
), gl_context(gl_canvas)
 , gl_api_init()
 , shared_objects()
 , coroutine_exec()
{
	InitMainMenu();
	InitComponents();
	Show();

	coroutine_exec = std::make_shared<SpectraCoroutineExecutor>(
		parent_app,
		this //TODO progress panel + sizer
	);

	SetStatus(wxGetTranslation(wxT("Initializing GL"), wxT("Status")));

	gl_context.SetCurrent(*gl_canvas);
	gl_api_init.reset(new oglplus::GLAPIInitializer());
	gl_canvas->Hide();

	gl_vendor->SetLabel(wxString(
		(const char*)glGetString(GL_VENDOR),
		wxConvUTF8
	));
	gl_renderer->SetLabel(wxString(
		(const char*)glGetString(GL_RENDERER),
		wxConvUTF8
	));
	gl_version->SetLabel(wxString(
		(const char*)glGetString(GL_VERSION),
		wxConvUTF8
	));
	sl_version->SetLabel(wxString(
		(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),
		wxConvUTF8
	));

	SetStatus(wxGetTranslation(wxT("Creating shared objects"), wxT("Status")));

	shared_objects = std::make_shared<SpectraSharedObjects>(&gl_context, gl_canvas);

	SetStatus(wxGetTranslation(wxT("Ready"), wxT("Status")));

	ConnectEventHandlers();

	SetStatus(wxGetTranslation(wxT("Running"), wxT("Status")));
}

SpectraMainFrame::~SpectraMainFrame(void)
{
	gl_context.SetCurrent(*gl_canvas);
	shared_objects.reset();
}
