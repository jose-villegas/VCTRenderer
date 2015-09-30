/**
 *  @file oglplus/glx/version.hpp
 *  @brief Wrapper for GLX version-related functionality
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_GLX_VERSION_1107121519_HPP
#define UTILS_OGLPLUS_GLX_VERSION_1107121519_HPP

#include <oglplus/x11/display.hpp>

#include <GL/glx.h>
#include <stdexcept>

namespace oglplus {
namespace glx {

class Version
{
private:
	int _major;
	int _minor;
public:
	Version(const x11::Display& display)
	 : _major(0)
	 , _minor(0)
	{
		if(!::glXQueryVersion(
			display,
			&_major,
			&_minor
		)) throw std::runtime_error("Error querying GLX version");
	}

	void AssertAtLeast(int major, int minor) const
	{
		if((_major < major) || ((_major == major) && (_minor < minor)))
			throw std::runtime_error("Invalid GLX version");
	}

	int Major(void) const
	{
		return _major;
	}

	int Minor(void) const
	{
		return _minor;
	}
};

} // namespace glx
} // namespace oglplus

#endif // include guard
