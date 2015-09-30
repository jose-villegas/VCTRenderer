/*
 *  .file advanced/spectra/openal_document.hpp
 *  .brief Declares a document class using the OpenAL library
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_OPENAL_DOCUMENT_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_OPENAL_DOCUMENT_HPP

#include "document.hpp"

#include <wx/string.h>

#include <memory>
#include <cstddef>

class SpectraSharedObjects;

extern std::shared_ptr<SpectraDocument> SpectraOpenOpenALDoc(
	const wxString& file_path
);

#endif // include guard
