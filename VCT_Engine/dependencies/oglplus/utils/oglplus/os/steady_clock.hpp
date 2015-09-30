/**
 *  @file oglplus/os/steady_clock.hpp
 *  @brief Wrapper for OS real time clocks
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_OS_STEADY_CLOCK_1203040931_HPP
#define UTILS_OGLPLUS_OS_STEADY_CLOCK_1203040931_HPP

#include <oglplus/config/compiler.hpp>

#if !OGLPLUS_NO_CHRONO
#include <chrono>
#else
#include <ctime>
#endif

namespace oglplus {
namespace os {

// Can be used for measuring time in seconds since the clocks creation
class steady_clock
{
private:
#if !OGLPLUS_NO_CHRONO
	typedef std::chrono::system_clock std_clock;
	std::chrono::time_point<std_clock> _start;
#else
	std::clock_t _start;
#endif
public:
	steady_clock(void)
	{
		reset();
	}

	void reset(void)
	{
#if !OGLPLUS_NO_CHRONO
		_start = std_clock::now();
#else
		_start = std::clock();
#endif
	}

	double seconds(void) const
	{
#if !OGLPLUS_NO_CHRONO
		return 	double((std_clock::now() - _start).count())*
			double(std_clock::period::num)/
			double(std_clock::period::den);
#else
		return	double(std::clock() - _start)/
			double(CLOCKS_PER_SEC);
#endif
	}
};

} // namespace os
} // namespace oglplus

#endif // include guard
