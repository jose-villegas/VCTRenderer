/**
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks if unified initialization syntax is implemented properly
 */
#include <string>

struct uis
{
	int i;
	float f;
	std::string s;
};

inline uis make_uis(void)
{
	return {123, 456.789f, "0"};
}

int main(int argc, const char** argv)
{
	uis x = make_uis();
	return (x.i == 123)?0:1;
}
