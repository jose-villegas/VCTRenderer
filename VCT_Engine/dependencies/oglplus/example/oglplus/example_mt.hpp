/**
 *  @file oglplus/example_mt.hpp
 *  @brief Declares a common utilities for multi-threaded examples
 *
 *  Copyright 2008-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef __OGLPLUS_EXAMPLE_EXAMPLE_MT_1119071146_HPP__
#define __OGLPLUS_EXAMPLE_EXAMPLE_MT_1119071146_HPP__

#include <cassert>

#include <queue>
#include <mutex>
#include <condition_variable>


namespace oglplus {

class ExampleSyncQueue
{
private:
	std::queue<Sync> sync_queue;
	std::mutex sync_mutex;
	std::condition_variable sync_cv;
	bool cancelled;
public:
	ExampleSyncQueue(void)
	 : cancelled(false)
	{ }

	ExampleSyncQueue(ExampleSyncQueue&& tmp)
	 : sync_queue(std::move(tmp.sync_queue))
	 , sync_mutex()
	 , sync_cv()
	 , cancelled(tmp.cancelled)
	{
		tmp.cancelled = true;
	}

	ExampleSyncQueue& operator = (ExampleSyncQueue&& tmp)
	{
		sync_queue = std::move(tmp.sync_queue);
		cancelled = tmp.cancelled;
		tmp.cancelled = true;
		return *this;
	}

	~ExampleSyncQueue(void)
	{
		Cancel();
	}

	void Cancel(void)
	{
		std::unique_lock<std::mutex> l(sync_mutex);
		if(!cancelled)
		{
			cancelled = true;
			std::queue<Sync> empty;
			std::swap(sync_queue, empty);
			sync_cv.notify_all();
		}
	}

	void Push(Sync&& sync)
	{
		std::unique_lock<std::mutex> l(sync_mutex);
		sync_queue.emplace(std::move(sync));
		sync_cv.notify_all();
	}

	void Signal(void)
	{
		Push(Sync());
	}

	Sync Pop(void)
	{
		std::unique_lock<std::mutex> l(sync_mutex);
		while(sync_queue.empty() && !cancelled) sync_cv.wait(l);
		Sync tmp(cancelled?Sync():std::move(sync_queue.front()));
		if(!cancelled) sync_queue.pop();
		return std::move(tmp);
	}

	void Wait(void)
	{
		Pop().Wait();
	}
};

class ExampleFrameSyncQueue
{
private:
	std::mutex sync_mutex;
	std::condition_variable sync_cv;
	std::queue<Sync> sync_queue;
	std::queue<GLuint> value_queue;
	bool cancelled;
public:
	ExampleFrameSyncQueue(void)
	 : cancelled(false)
	{ }

	ExampleFrameSyncQueue(ExampleFrameSyncQueue&& tmp)
	 : sync_mutex()
	 , sync_cv()
	 , sync_queue(std::move(tmp.sync_queue))
	 , value_queue(std::move(tmp.value_queue))
	 , cancelled(tmp.cancelled)
	{
		tmp.cancelled = true;
	}

	ExampleFrameSyncQueue& operator = (ExampleFrameSyncQueue&& tmp)
	{
			sync_queue = std::move(tmp.sync_queue);
			value_queue = std::move(tmp.value_queue);
			cancelled = tmp.cancelled;
			tmp.cancelled = true;
			return *this;
	}

	~ExampleFrameSyncQueue(void)
	{
		Cancel();
	}

	void Cancel(void)
	{
		std::unique_lock<std::mutex> l(sync_mutex);
		if(!cancelled)
		{
			cancelled = true;
			std::queue<Sync> tsq;
			std::queue<GLuint> tvq;
			std::swap(sync_queue, tsq);
			std::swap(value_queue, tvq);
			sync_cv.notify_all();
		}
	}

	void Push(GLuint value)
	{
		std::unique_lock<std::mutex> l(sync_mutex);
		sync_queue.push(Sync());
		value_queue.push(value);
		sync_cv.notify_all();
	}

	GLuint Pop(void)
	{
		std::unique_lock<std::mutex> l(sync_mutex);
		while(!cancelled && sync_queue.empty()) sync_cv.wait(l);

		if(!cancelled)
		{
			sync_queue.front().Wait();
			sync_queue.pop();

			GLuint result = value_queue.front();
			value_queue.pop();
			return result;
		}
		else return 0;
	}
};

} // namespace oglplus

#endif // include guard
