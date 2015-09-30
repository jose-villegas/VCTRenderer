/**
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks if scoped enum template parameters are supported.
 */

template <typename Enum, template <Enum> class Wrap>
struct EnumToClass;

enum class Numbers : int
{
	One = 1,
	Two = 2,
	Three = 3
};

template <template <Numbers> class Wrap>
struct EnumToClass<Numbers, Wrap>
{
	Wrap<Numbers::One> one;
	Wrap<Numbers::Two> two;
	Wrap<Numbers::Three> three;
};

template <Numbers Value>
struct Wrapper
{
	static int get(void)
	{
		return int(Value);
	}
};

int main(int argc, const char** argv)
{
	EnumToClass<Numbers, Wrapper> test;

	if(test.one.get() != 1) return 1;
	if(test.two.get() != 2) return 2;
	if(test.three.get() != 3) return 3;

	return 0;
}
