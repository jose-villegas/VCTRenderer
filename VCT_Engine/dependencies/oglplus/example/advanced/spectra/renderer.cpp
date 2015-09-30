/*
 *  .file advanced/spectra/renderer.cpp
 *  .brief Implements the document renderer abstract class.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "renderer.hpp"

SpectraSharedObjects& SpectraRenderer::Common(void)
{
	assert(shared_objects);
	return *shared_objects;
}

SpectraVisualisation& SpectraRenderer::DocVis(void)
{
	assert(document_vis);
	return *document_vis;
}

SpectraRenderer::SpectraRenderer(
	SpectraApp& app,
	const std::shared_ptr<SpectraSharedObjects>& sh_obj,
	const std::shared_ptr<SpectraVisualisation>& doc_vis,
	wxGLCanvas* canvas
): parent_app(app)
 , shared_objects(sh_obj)
 , document_vis(doc_vis)
{
	assert(canvas);
	canvas->SetCurrent(DocVis().GLContext());
}

void SpectraRenderer::ReinitStyle(void)
{
}

