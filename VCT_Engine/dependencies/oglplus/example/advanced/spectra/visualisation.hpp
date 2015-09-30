/*
 *  .file advanced/spectra/visualisation.hpp
 *  .brief Declares the class handling document visualization.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_VISUALISATION_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_VISUALISATION_HPP

#include <oglplus/gl.hpp>
#include <oglplus/buffer.hpp>
#include <oglplus/texture.hpp>

#include "spectra_app.hpp"
#include "document.hpp"
#include "calculator.hpp"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <memory>
#include <set>

class SpectraMainFrame;
class SpectraVisDataUploader;

class SpectraGLContext
 : public wxGLContext
{
public:
	SpectraGLContext(
		wxGLCanvas* canvas,
		wxGLContext* parent_ctxt
	);
};

class SpectraVisualisation
{
private:
	SpectraMainFrame* main_frame;

	float selected_time, selection_begin, selection_end;

	SpectraGLContext gl_context;
	std::shared_ptr<std::set<wxGLCanvas*>> gl_canvases;

	std::shared_ptr<SpectraDocument> document;

	oglplus::Buffer spectrum_data;
	oglplus::Texture spectrum_tex;

	std::size_t spectrum_size;
	std::size_t signal_samples_per_grid;

	std::weak_ptr<SpectraVisDataUploader> uploader_ref;
public:
	SpectraVisualisation(
		SpectraMainFrame* frame,
		wxGLCanvas* canvas,
		wxGLContext* parent_ctxt,
		const std::shared_ptr<SpectraCalculator>& calc,
		const std::shared_ptr<SpectraDocument>& doc
	);

	~SpectraVisualisation(void);

	void AddCanvas(wxGLCanvas*);
	void RemoveCanvas(wxGLCanvas*);

	bool FinishLoading(void);

	wxGLContext& GLContext(void);

	oglplus::Reference<oglplus::Texture> SpectrumTex(void);

	wxString Name(void);

	const SpectraDocument& Document(void) const;
	void Play(void);

	std::size_t SignalSpectrumSize(void) const;

	std::size_t SignalSamplesPerGrid(void) const;

	std::size_t SignalSamplesPerGridPatch(void) const;

	std::size_t GridSamples(void) const;

	void SelectedTime(float time);

	float SelectedTime(void) const;

	void DragSelection(float time, float delta);

	float SelectionBegin(void) const;
	float SelectionEnd(void) const;
};

#endif // include guard
