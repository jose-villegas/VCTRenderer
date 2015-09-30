/**
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for the chrono header
 */
#include <chrono>

int main(int argc, const char** argv)
{
	return (
		(std::chrono::system_clock::period::den > 0) &&
		(std::chrono::steady_clock::period::den > 0)
	) ? 0:1;
}
