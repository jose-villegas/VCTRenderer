/*
 *  .file advanced/spectra/spectra_app.cpp
 *  .brief Implements the main application class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>
#include <oglplus/opt/application.hpp>
#include <oglplus/lib/oglplus.hpp>
#include <oglplus/error/glfunc.hpp>

#include <oalplus/al.hpp>
#include <oalplus/lib.hpp>

#include "spectra_app.hpp"
#include "main_frame.hpp"

#include <wx/stdpaths.h>

IMPLEMENT_APP(SpectraApp)

wxString SpectraApp::MakeCommonErrorMessage(oglplus::Error& err)
{
	const wxString newline(wxT("\r\n"));
	wxString message;
	message.Append(wxString(err.what(), wxConvUTF8));
	message.Append(newline);
	message.Append(wxString::Format(
		wxGetTranslation(wxT("GL function: '%s'"), wxT("Error")),
		wxString(err.GLFunc(), wxConvUTF8).c_str()
	));
	message.Append(newline);
	message.Append(wxString::Format(
		wxGetTranslation(wxT("Class: '%s'"), wxT("Error")),
		wxString(err.ObjectTypeName(), wxConvUTF8).c_str()
	));
	message.Append(newline);
	message.Append(wxString::Format(
		wxGetTranslation(wxT("Object: '%s'"), wxT("Error")),
		wxString(err.ObjectDesc().c_str(), wxConvUTF8).c_str()
	));
	message.Append(newline);
	message.Append(wxString::Format(
		wxGetTranslation(wxT("Binding point: '%s'"), wxT("Error")),
		wxString(err.TargetName(), wxConvUTF8).c_str()
	));
	message.Append(newline);
	message.Append(wxString::Format(
		wxGetTranslation(wxT("File: '%s'"), wxT("Error")),
		wxString(err.SourceFile(), wxConvUTF8).c_str()
	));
	message.Append(newline);
	message.Append(wxString::Format(
		wxGetTranslation(wxT("Line: '%u'"), wxT("Error")),
		err.SourceLine()
	));
	message.Append(newline);

	return message;
}

void SpectraApp::HandleError(
	oglplus::Error& err,
	wxWindow* parent
)
{
	wxMessageBox(
		MakeCommonErrorMessage(err),
		wxGetTranslation(wxT("OpenGL Error"), wxT("Error")),
		wxOK | wxICON_ERROR,
		parent
	);
}

void SpectraApp::HandleError(
	const std::exception& err,
	wxWindow* parent
)
{
	wxMessageBox(
		wxString(err.what(), wxConvUTF8),
		wxGetTranslation(wxT("Unspecified error"), wxT("Error")),
		wxOK | wxICON_ERROR,
		parent
	);
}

bool SpectraApp::OnInit(void)
{
	bool result = false;
	try
	{
		std::string arg0str(
			wxStandardPaths::Get()
				.GetExecutablePath().mb_str(wxConvUTF8)
		);
		const char* arg0 = arg0str.c_str();
		oglplus::Application::ParseCommandLineOptions(1, &arg0);
		SetTopWindow(new SpectraMainFrame(*this));
		result = true;
	}
	catch(oglplus::MissingFunction& mfe) { HandleError(mfe); }
	catch(oglplus::ProgramBuildError& pbe) { HandleError(pbe); }
	catch(oglplus::LimitError& le) { HandleError(le); }
	catch(oglplus::Error& err) { HandleError(err); }
	catch(const std::exception& se) { HandleError(se); }
	return result;
}

