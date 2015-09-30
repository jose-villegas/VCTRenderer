/**
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for the constexpr keyword
 */
template <int I>
struct test
{
	friend constexpr int get_i(const test<I>&)
	{
		return I;
	}
};

int main(int argc, const char** argv)
{
	test<1> t;
	return (argc == get_i(t))?0:1;
}
