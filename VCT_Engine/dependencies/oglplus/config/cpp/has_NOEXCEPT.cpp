/**
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for the noexcept keyword
 */
#include <type_traits>

#define OGLPLUS_NOEXCEPT_IF(...) noexcept(noexcept(__VA_ARGS__))

template <typename T>
T get_default(void) OGLPLUS_NOEXCEPT_IF(T(T()))
{
	return T();
}

struct add
{
	template <typename T>
	T operator()(T a, T b) OGLPLUS_NOEXCEPT_IF(T(a + b))
	{
		return a + b;
	}
};

template <typename T, typename Op>
T call_on_default(Op operation)
OGLPLUS_NOEXCEPT_IF(T(operation(std::declval<T>(), std::declval<T>())))
{
	return operation(get_default<T>(), get_default<T>());
}


int main(int argc, const char** argv)
{
	static_assert(noexcept(::get_default<int>()), "Error");
	return (argc == call_on_default<int>(::add()))?0:1;
}
