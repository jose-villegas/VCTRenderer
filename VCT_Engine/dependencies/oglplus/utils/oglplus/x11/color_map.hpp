/**
 *  @file oglplus/x11/color_map.hpp
 *  @brief Wrapper for X11 colormap class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_X11_COLOR_MAP_1107121519_HPP
#define UTILS_OGLPLUS_X11_COLOR_MAP_1107121519_HPP

#include <oglplus/x11/display.hpp>
#include <oglplus/x11/visual_info.hpp>

#include <X11/Xlib.h>
#include <stdexcept>

namespace oglplus {
namespace x11 {

class Colormap
 : public DisplayObject< ::Colormap>
{
public:
	Colormap(const Display& display, const VisualInfo& vi)
	 : DisplayObject< ::Colormap>(
		display,
		::XCreateColormap(
			display,
			RootWindow(display.Get(), vi->screen),
			vi->visual,
			AllocNone
		),
		::XFreeColormap,
		"Error creating X Colormap"
	){ }
};

} // namespace x11
} // namespace oglplus

#endif // include guard
