/**
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for the override keyword
 */
struct intf
{
	virtual ~intf(void) { }

	virtual int func(void) = 0;
};

class impl : public intf
{
	int func(void) override { return 0; }
};

int main(void)
{
	impl i;
	intf* pi = &i;

	return pi->func();
}
