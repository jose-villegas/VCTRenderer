/*
 *  .file advanced/spectra/coroutine.cpp
 *  .brief Implements coroutine execution
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "coroutine.hpp"

#include <wx/progdlg.h>

class SpectraCoroutineExecution
 : public wxProgressDialog // TODO reimplement as widgets on main_frame
 , public SpectraCoroutine
{
private:
	std::shared_ptr<SpectraCoroutine> coroutine;
public:
	SpectraCoroutineExecution(
		wxWindow* parent,
		const std::shared_ptr<SpectraCoroutine>& coro
	);

	wxString Description(void) const;

	void Cancel(void);

	bool DoWork(void);

	int PercentDone(void) const;
};

SpectraCoroutineExecution::SpectraCoroutineExecution(
	wxWindow* parent,
	const std::shared_ptr<SpectraCoroutine>& coro
): wxProgressDialog(
	wxGetTranslation(wxT("Executing action")),
	coro->Description(),
	100,
	parent,
	wxPD_CAN_ABORT|wxPD_AUTO_HIDE
), coroutine(coro)
{
	assert(coroutine);
	Show();
}

wxString SpectraCoroutineExecution::Description(void) const
{
	return coroutine->Description();
}

int SpectraCoroutineExecution::PercentDone(void) const
{
	return coroutine->PercentDone();
}

void SpectraCoroutineExecution::Cancel(void)
{
	coroutine->Cancel();
}

bool SpectraCoroutineExecution::DoWork(void)
{
	bool result = coroutine->DoWork();
	int pd = coroutine->PercentDone();
	if(pd >= 0 && pd <= 100)
	{
		if(!Update(pd)) return true;
	}
	else
	{
		if(!Pulse()) return true;
	}
	return result;
}


// SpectraCoroutineExecutor
SpectraCoroutineExecutor::SpectraCoroutineExecutor(SpectraApp& app, wxWindow* win)
 : parent_app(app)
 , parent_window(win)
{
}

void SpectraCoroutineExecutor::Start(
	const std::shared_ptr<SpectraCoroutine>& coroutine,
	bool quiet_exec
)
{
	try
	{
		if(quiet_exec)
		{
			coroutines.push(coroutine);
		}
		else if(!coroutine->DoWork())
		{
			coroutines.push(
				std::make_shared<SpectraCoroutineExecution>(
					parent_window,
					coroutine
				)
			);
		}
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err); }
	catch(const std::exception& se) { parent_app.HandleError(se); }
}

bool SpectraCoroutineExecutor::UpdateSingle(const std::shared_ptr<SpectraCoroutine>& coroutine)
{
	try
	{
		return coroutine->DoWork();
	}
	catch(oglplus::Error& err) { parent_app.HandleError(err); }
	catch(const std::exception& se) { parent_app.HandleError(se); }
	return true;
}

bool SpectraCoroutineExecutor::DoWork(void)
{
	if(!coroutines.empty())
	{
		coroutine_queue tmp_queue;
		while(!coroutines.empty())
		{
			std::shared_ptr<SpectraCoroutine> coro = coroutines.front();
			coroutines.pop();
			if(!UpdateSingle(coro))
			{
				tmp_queue.push(coro);
			}
		}
		coroutines.swap(tmp_queue);
	}
	return coroutines.empty();
}

