/*
 *  .file advanced/spectra/main_frame.hpp
 *  .brief Declares the main frame.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_MAIN_FRAME_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_MAIN_FRAME_HPP

#include <oglplus/gl.hpp>

#include "shared_objects.hpp"
#include "spectra_app.hpp"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <memory>
#include <set>

struct SpectraDocument;
struct SpectraCoroutine;
class SpectraDocumentFrame;
class SpectraCoroutineExecutor;
class SpectraMainFrameDocumentLoader;
class SpectraRenderer;
class SpectraVisualisation;

class SpectraMainFrame
 : public wxFrame
{
private:
	SpectraApp& parent_app;
	wxMenuBar* main_menu;

	wxMenu* renderer_menu;

	wxStatusBar* status_bar;
	void SetStatus(const wxString& status_text);

	wxGLCanvas* gl_canvas;
	wxGLContext gl_context;
	std::unique_ptr<oglplus::GLAPIInitializer> gl_api_init;

	std::shared_ptr<SpectraSharedObjects> shared_objects;

	std::set<SpectraDocumentFrame*> doc_frames;

	wxStaticText* gl_vendor;
	wxStaticText* gl_renderer;
	wxStaticText* gl_version;
	wxStaticText* sl_version;

	std::shared_ptr<SpectraCoroutineExecutor> coroutine_exec;

	void InitMainMenu(void);
	void InitComponents(void);
	void ConnectEventHandlers(void);

	void OnIdle(wxIdleEvent& event);

	void DoClose(wxCommandEvent&);
	void OnClose(wxCloseEvent&);

	void DoShowAboutDialog(wxCommandEvent&);

	void DoOpenDocument(wxCommandEvent&);
	void DoGenerateDocument(wxCommandEvent&);
public:
	SpectraMainFrame(SpectraApp& app);
	~SpectraMainFrame(void);

	void StartCoroutine(const std::shared_ptr<SpectraCoroutine>&, bool);
	void StartCoroutine(const std::shared_ptr<SpectraCoroutine>&);

	void OpenLoadedDocument(const std::shared_ptr<SpectraDocument>&);

	void RegisterDocumentFrame(SpectraDocumentFrame*);
	void ForgetDocumentFrame(SpectraDocumentFrame*);

	std::shared_ptr<SpectraRenderer> PickRenderer(
		SpectraApp& parent_app,
		SpectraMainFrame* main_frame,
		const std::shared_ptr<SpectraVisualisation>& doc_vis,
		wxGLCanvas* canvas
	);

	typedef std::function<
		std::shared_ptr<SpectraRenderer> (
			SpectraApp&,
			SpectraMainFrame*,
			const std::shared_ptr<SpectraVisualisation>&,
			wxGLCanvas*
		)
	> RendererGetter;

	RendererGetter LazyRendererPicker(void);
};

#endif // include guard
