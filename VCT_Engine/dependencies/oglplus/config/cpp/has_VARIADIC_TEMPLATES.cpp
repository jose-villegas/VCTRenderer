/**
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for support for variadic templates
 */

template <typename T, T ... P>
struct vt1
{ };

template <typename ... P>
struct vt2
{
	vt2(void){ }

	vt2(P&& ...){ }

	static unsigned size(void)
	{
		return sizeof...(P);
	}
};

template <template <class...> class T, typename ... P>
struct vt3
 : public T<P...>
{
	vt3(void)
	 : T<P...>(P()...)
	{ }
};

int main(int argc, const char** argv)
{
	typedef vt1<int, 1, 2, 3, 4, 5, 6, 7> a;
	vt2<short, int, long, float, double, a> b;
	vt3<vt2, bool, char, wchar_t, short, int, long, float, a> c;
	return (b.size() == c.size()-2)?0:1;
}
