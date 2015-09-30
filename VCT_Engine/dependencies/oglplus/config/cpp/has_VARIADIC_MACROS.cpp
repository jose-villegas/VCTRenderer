/**
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for support for variadic macros
 */

#define OGLPLUS_VM_TEST(FUNC, ...) FUNC(__VA_ARGS__)

int fn1(int i0)
{
	return i0;
}

int fn4(int i0, int i1, int i2, int i3)
{
	return i0+i1+i2+i3;
}

int main(int argc, const char** argv)
{
	return	OGLPLUS_VM_TEST(fn1, 10)-
		OGLPLUS_VM_TEST(fn4, 1, 2, 3, 4);
}
