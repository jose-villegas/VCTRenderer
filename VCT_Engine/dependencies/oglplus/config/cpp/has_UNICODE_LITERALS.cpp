/**
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks if unicode literals are supported
 */

int main(int argc, const char** argv)
{
	const char* s = u8"unicode literal";
	return s != 0;
}
