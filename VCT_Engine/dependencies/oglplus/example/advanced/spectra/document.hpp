/*
 *  .file advanced/spectra/document.hpp
 *  .brief Declares the document class.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_DOCUMENT_HPP

#include <wx/wx.h>
#include <wx/string.h>

#include <memory>
#include <functional>

struct SpectraDocument
{
	virtual ~SpectraDocument(void) { }

	virtual bool FinishLoading(void) = 0;

	virtual int PercentLoaded(void) const = 0;

	virtual std::size_t SamplesPerSecond(void) const = 0;

	virtual std::size_t SignalSampleCount(void) const = 0;

	virtual std::size_t QuerySignalSamples(
		float* buffer,
		std::size_t bufsize,
		std::size_t start,
		std::size_t end
	) = 0;

	virtual float MaxTime(void) const = 0;

	virtual wxString Name(void) const = 0;

	virtual bool CanPlay(void) const = 0;

	virtual void Play(float from, float to) = 0;
};

class SpectraSharedObjects;

extern std::shared_ptr<SpectraDocument> SpectraOpenTestDoc(
	const std::function<float (float)>& signal_func,
	std::size_t samples_per_second,
	float max_time
);

extern std::shared_ptr<SpectraDocument> SpectraLoadDocFromFile(
	const wxString& file_path
);

#endif // include guard
