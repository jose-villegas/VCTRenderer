/*
 *  .file advanced/spectra/spectra_app.hpp
 *  .brief Declares the main application class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_APP_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_APP_HPP

#include <oglplus/gl.hpp>

#include <oglplus/error/basic.hpp>

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <stdexcept>

class SpectraApp
 : public wxGLApp
{
private:
	static wxString MakeCommonErrorMessage(oglplus::Error& err);
public:
	void HandleError(
		oglplus::Error& err,
		wxWindow* parent = nullptr
	);

	void HandleError(
		const std::exception& err,
		wxWindow* parent = nullptr
	);

	bool OnInit(void);
};

#endif // include guard
