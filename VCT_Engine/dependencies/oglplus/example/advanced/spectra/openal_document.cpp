/*
 *  .file advanced/spectra/openal_document.cpp
 *  .brief Implements a document class using the OpenAL library
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oalplus/al.hpp>
#include <oalplus/device.hpp>
#include <oalplus/context.hpp>
#include <oalplus/listener.hpp>
#include <oalplus/source.hpp>
#include <oalplus/alut.hpp>

#include <oalplus/all.hpp> //TODO

#include "openal_document.hpp"

#include <vector>
#include <stdexcept>
#include <algorithm>

#if OGLPLUS_OPENAL_FOUND

class SpectraOpenALDocument
 : public SpectraDocument
{
private:
	const wxString file_path;
	oalplus::DataFormat format;
	ALfloat frequency;

	std::vector<ALubyte> raw_data;
	std::vector<ALfloat> samples;

	oalplus::Device device;
	oalplus::ContextMadeCurrent context;
	oalplus::Listener listener;
	oalplus::Buffer sound_buf;
	oalplus::Source sound_src;
public:
	SpectraOpenALDocument(const wxString& path);

	bool FinishLoading(void);

	int PercentLoaded(void) const;

	std::size_t SamplesPerSecond(void) const;

	std::size_t SignalSampleCount(void) const;

	float MaxTime(void) const;

	wxString Name(void) const;

	std::size_t QuerySignalSamples(
		float* buffer,
		std::size_t bufsize,
		std::size_t start,
		std::size_t end
	);

	bool CanPlay(void) const;

	void Play(float from, float to);
};


SpectraOpenALDocument::SpectraOpenALDocument(const wxString& path)
 : file_path(path)
 , frequency(1024)
 , device()
 , context(device)
{
	listener.Position(0.0f, 0.0f, 0.0f);
	listener.Velocity(0.0f, 0.0f, 0.0f);
	listener.Orientation(0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f);

	sound_src.Position(0.0f, 0.0f,-1.0f);
}

bool SpectraOpenALDocument::FinishLoading(void)
{
	context.MakeCurrent();

	oalplus::ALUtilityToolkit alut(false);

	alut.LoadMemoryFromFile(
		raw_data,
		samples,
		(const char*)file_path.mb_str(wxConvUTF8),
		&format,
		&frequency
	);
	// TODO: stereo -> mono ?
	return true;
}

int SpectraOpenALDocument::PercentLoaded(void) const
{
	return samples.empty()?0:100;
}

std::size_t SpectraOpenALDocument::SamplesPerSecond(void) const
{
	return std::size_t(frequency);
}

std::size_t SpectraOpenALDocument::SignalSampleCount(void) const
{
	return samples.size();
}

float SpectraOpenALDocument::MaxTime(void) const
{
	return samples.size()/frequency;
}

wxString SpectraOpenALDocument::Name(void) const
{
	return file_path;
}

std::size_t SpectraOpenALDocument::QuerySignalSamples(
	float* buffer,
	std::size_t bufsize,
	std::size_t start,
	std::size_t end
)
{
	assert(bufsize >= end-start);

	std::size_t n = end;
	if(n > samples.size())
		n = samples.size();
	if(n < start)
		n = 0;
	else n -= start;

	std::copy(samples.data()+start, samples.data()+start+n, buffer);
	return n;
}

bool SpectraOpenALDocument::CanPlay(void) const
{
	return true;
}

void SpectraOpenALDocument::Play(float from, float to)
{
	if(from < 0.0f) from = 0.0f;
	if(from < to)
	{
		std::size_t mult = 1;
		switch(format)
		{
			case oalplus::DataFormat::Mono16:
			case oalplus::DataFormat::Stereo8:
				mult = 2;
				break;
			case oalplus::DataFormat::Stereo16:
				mult = 4;
				break;
			default:break;
		}

		std::size_t begin = std::size_t(frequency*from)*mult;
		std::size_t end = std::size_t(frequency*to)*mult;

		if(end > raw_data.size())
		{
			end = raw_data.size();
		}

		if(begin < end)
		{
			sound_src.DetachBuffers();
			sound_buf.Data(
				format,
				raw_data.data()+begin,
				ALsizei(end-begin),
				ALsizei(frequency)
			);
			sound_src.Buffer(sound_buf);
			sound_src.Play();
		}
	}
}

std::shared_ptr<SpectraDocument> SpectraOpenOpenALDoc(
	const wxString& file_path
)
{
	return std::make_shared<SpectraOpenALDocument>(file_path);
}

#else

std::shared_ptr<SpectraDocument> SpectraOpenOpenALDoc(
	const wxString& /*file_path*/
)
{
	throw std::runtime_error("OpenAL documents not supported");
	return std::shared_ptr<SpectraDocument>();
}

#endif
