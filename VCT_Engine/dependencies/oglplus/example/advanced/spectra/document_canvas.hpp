/*
 *  .file advanced/spectra/document_canvas.hpp
 *  .brief Declares the document GL canvas.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_CANVAS_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_CANVAS_HPP

#include <oglplus/gl.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

class SpectraDocumentCanvas
 : public wxGLCanvas
{
private:
	wxEvtHandler* evt_handler;

	void OnMouseEvent(wxMouseEvent& event);
public:
	SpectraDocumentCanvas(
		wxEvtHandler* handler,
		wxWindow* parent
	);
};

#endif // include guard
