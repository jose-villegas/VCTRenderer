/*
 *  .file advanced/spectra/document_canvas.cpp
 *  .brief Implements the document GL canvas
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "document_canvas.hpp"

extern int* SpectraGLConfig(void);

void SpectraDocumentCanvas::OnMouseEvent(wxMouseEvent& event)
{
	evt_handler->ProcessEvent(event);
}

SpectraDocumentCanvas::SpectraDocumentCanvas(
	wxEvtHandler* handler,
	wxWindow* parent
): wxGLCanvas(
	parent,
	wxID_ANY,
	SpectraGLConfig(),
	wxDefaultPosition,
	wxDefaultSize
), evt_handler(handler)
{
	Connect(
		wxEVT_MOTION,
		wxMouseEventHandler(SpectraDocumentCanvas::OnMouseEvent)
	);
	Connect(
		wxEVT_MOUSEWHEEL,
		wxMouseEventHandler(SpectraDocumentCanvas::OnMouseEvent)
	);
}

