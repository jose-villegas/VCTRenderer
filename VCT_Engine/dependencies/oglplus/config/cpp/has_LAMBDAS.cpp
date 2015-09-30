/**
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

int main(int argc, const char** argv)
{
	auto lambda = [&argc](int argn) -> bool
	{
		return argc == argn;
	};
	return lambda(argc)?0:1;
}
