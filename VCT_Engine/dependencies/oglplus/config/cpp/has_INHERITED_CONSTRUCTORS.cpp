/**
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks if inherited constructors are supported
 */

template <typename T>
struct Base
{
	T x;

	Base(long l, char, double) : x(T(l)) { }
	Base(int i, float) : x(T(i)) { }
};

class Derived : public Base<long>
{
private:
	typedef Base<long> Parent;
public:
	using Parent::Parent;
};

int main(int argc, const char** argv)
{
	Derived a(123l, '4', 567.89);
	Derived b(123, 456.789f);
	return int(a.x-b.x);
}
