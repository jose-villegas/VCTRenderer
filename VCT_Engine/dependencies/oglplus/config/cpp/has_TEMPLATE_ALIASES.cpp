/**
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks if type / template aliases are supported
 */

template <typename A, typename B, typename C>
struct Template
{
	A a;
	B b;
	C c;

	Template(A pa, B pb, C pc)
	 : a(pa)
	 , b(pb)
	 , c(pc)
	{ }
};

template <typename A>
using Alias = Template<A, char, double>;

int main(int argc, const char** argv)
{
	Alias<int> x(0,'1', 2.3);
	return x.a;
}
