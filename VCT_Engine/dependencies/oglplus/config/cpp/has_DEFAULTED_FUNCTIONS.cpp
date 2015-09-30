/**
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks if defaulted functions are implemented properly
 */

class df
{
public:
	df(void) = default;
	df(df&&) = default;
};

int main(int argc, const char** argv)
{
	return 0;
}
