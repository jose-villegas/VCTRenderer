/**
 *  @file oglplus/glx/fb_config.hpp
 *  @brief Wrapper for GLX framebuffer config class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_GLX_FB_CONFIG_1107121519_HPP
#define UTILS_OGLPLUS_GLX_FB_CONFIG_1107121519_HPP

#include <GL/glx.h>
#include <stdexcept>

namespace oglplus {
namespace glx {

class FBConfigs;

class FBConfig
{
private:
	::GLXFBConfig _handle;

	FBConfig(::GLXFBConfig handle)
	 : _handle(handle)
	{ }

	friend class FBConfigs;
public:
	::GLXFBConfig Handle(void) const
	{
		return _handle;
	}

	operator ::GLXFBConfig(void) const
	{
		return Handle();
	}
};

} // namespace glx
} // namespace oglplus

#endif // include guard
