/**
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Checks for the availability of user-defined literals
 */
#include <string>

namespace oglplus {

std::string operator "" _oglplus_str(const char* str, size_t size)
{
	return std::string(str, size);
}

} // namespace oglplus

int main(void)
{
	using oglplus::operator "" _oglplus_str;
	auto s = "OGLplus"_oglplus_str;
	return (s.empty())?1:0;
}
