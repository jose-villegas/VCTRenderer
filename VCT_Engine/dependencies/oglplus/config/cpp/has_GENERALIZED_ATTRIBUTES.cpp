/**
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for the availability of generalized attribues
 */

[[noreturn]]
void test(void)
{
	throw 1;
}

int main(int argc, const char** argv)
{
	try { test(); }
	catch(...){ return 0; }
	return 1;
}
