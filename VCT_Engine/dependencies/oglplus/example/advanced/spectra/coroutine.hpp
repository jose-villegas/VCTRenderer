/*
 *  .file advanced/spectra/coroutine.hpp
 *  .brief Declares the abstract interface for coroutines
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_EXAMPLE_SPECTRA_COROUTINE_HPP
#define OGLPLUS_EXAMPLE_SPECTRA_COROUTINE_HPP

#include "spectra_app.hpp"

#include <wx/wx.h>

#include <memory>
#include <queue>

struct SpectraCoroutine
{
	virtual ~SpectraCoroutine(void) { }

	virtual wxString Description(void) const = 0;

	virtual void Cancel(void) = 0;

	virtual bool DoWork(void) = 0;

	virtual int PercentDone(void) const = 0;
};

class SpectraCoroutineExecutor
{
private:
	SpectraApp& parent_app;
	wxWindow* parent_window;

	typedef std::queue<std::shared_ptr<SpectraCoroutine> > coroutine_queue;
	coroutine_queue coroutines;

	bool UpdateSingle(const std::shared_ptr<SpectraCoroutine>& coroutine);
public:
	SpectraCoroutineExecutor(SpectraApp& app, wxWindow* win);

	void Start(
		const std::shared_ptr<SpectraCoroutine>& coroutine,
		bool quiet_exec
	);

	bool DoWork(void);
};

#endif // include guard
