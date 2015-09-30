/*
 *  .file advanced/spectra/xsection_renderer.hpp
 *  .brief Declares the spectrum cross-section renderer class.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_XSECTION_RENDERER_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_XSECTION_RENDERER_HPP

#include "renderer.hpp"

extern std::shared_ptr<SpectraRenderer> SpectraMakeXSectionRenderer(
	SpectraApp& app,
	const std::shared_ptr<SpectraSharedObjects>& sh_obj,
	const std::shared_ptr<SpectraVisualisation>& doc_vis,
	wxGLCanvas* canvas
);

#endif // include guard
