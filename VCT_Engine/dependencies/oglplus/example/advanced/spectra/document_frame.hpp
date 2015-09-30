/*
 *  .file advanced/spectra/document_frame.hpp
 *  .brief Declares the document GL frame.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_FRAME_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_FRAME_HPP

#include <oglplus/gl.hpp>

#include "spectra_app.hpp"
#include "document_canvas.hpp"
#include "document_view.hpp"

#include <wx/wx.h>

#include <memory>
#include <functional>

struct SpectraDocument;
class SpectraMainFrame;
class SpectraVisualisation;
class SpectraRenderer;

class SpectraDocumentFrame
 : public wxFrame
{
private:
	SpectraApp& parent_app;
	SpectraMainFrame* main_frame;

	wxMenuBar* main_menu;
	wxMenuItem* play_menu_item;

	wxPanel* main_panel;

	wxStatusBar* status_bar;
	void SetStatus(const wxString& status_text);

	wxGLContext* parent_context;
	SpectraDocumentCanvas* gl_canvas;

	std::shared_ptr<SpectraVisualisation> document_vis;
	std::shared_ptr<SpectraRenderer> renderer;

	SpectraDocumentView document_view;

	void InitMainMenu(void);
	void InitComponents(void);
	void ConnectEventHandlers(void);

	void DoClose(wxCommandEvent&);
	void OnClose(wxCloseEvent&);

	void DoDuplicate(wxCommandEvent&);

	void HandleResize(void);
	void OnResize(wxSizeEvent& event);

	void HandleSysColorChange(void);
	void OnSysColorChange(wxSysColourChangedEvent&);

	void DoPlay(wxCommandEvent&);

	wxPoint old_mouse_position;
	GLint ClampMouseCoord(GLint c, GLint m);
	void HandleMouseMotion(const wxMouseEvent& event);
	void OnMouseMotionEvent(wxMouseEvent& event);
	void HandleMouseWheel(const wxMouseEvent& event);
	void OnMouseWheelEvent(wxMouseEvent& event);

	int idle_call_count;
	void Update(void);
public:
	void OnIdle(wxIdleEvent& event);

	typedef std::function<
		std::shared_ptr<SpectraVisualisation> (
			SpectraApp&,
			SpectraMainFrame*,
			wxGLCanvas*,
			wxGLContext*
		)
	> VisualisationGetter;

	typedef std::function<
		std::shared_ptr<SpectraRenderer> (
			SpectraApp&,
			SpectraMainFrame*,
			const std::shared_ptr<SpectraVisualisation>&,
			wxGLCanvas*
		)
	> RendererGetter;

	SpectraDocumentFrame(
		SpectraApp& app,
		SpectraMainFrame* parent,
		wxGLContext* parent_ctxt,
		const VisualisationGetter& get_doc_vis,
		const RendererGetter& get_renderer
	);

	~SpectraDocumentFrame(void);
};

#endif // include guard
