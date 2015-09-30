/**
 *  @file advanced/cloud_trace/threads.hpp
 *  @brief Implementation of thread-helper classes
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "threads.hpp"

namespace oglplus {
namespace cloud_trace {

inline void Semaphore::_decr(void)
{
	std::unique_lock<std::mutex> lock(_mutex);
	while(_value == 0) _cv.wait(lock);
	--_value;
}

inline void Semaphore::_incr(std::size_t n)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_value += n;
	_cv.notify_all();
}

Semaphore::Semaphore(std::size_t initial)
 : _value(initial)
{ }

void Semaphore::Wait(std::size_t n)
{
	while(n--) _decr();
}

void Semaphore::Signal(std::size_t n)
{
	_incr(n);
}

bool Semaphore::Signalled(std::size_t n)
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _value >= n;
}

} // namespace cloud_trace
} // namespace oglplus
