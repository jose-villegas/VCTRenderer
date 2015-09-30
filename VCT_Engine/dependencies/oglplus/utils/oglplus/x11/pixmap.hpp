/**
 *  @file oglplus/x11/pixmap.hpp
 *  @brief Wrapper for X11 pixmap class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_X11_PIXMAP_1107121519_HPP
#define UTILS_OGLPLUS_X11_PIXMAP_1107121519_HPP

#include <oglplus/x11/display.hpp>
#include <oglplus/x11/visual_info.hpp>

#include <X11/Xlib.h>
#include <stdexcept>

namespace oglplus {
namespace x11 {

class Pixmap
 : public DisplayObject< ::Pixmap>
{
public:
	Pixmap(
		const Display& display,
		const VisualInfo& vi,
		unsigned width,
		unsigned height
	): DisplayObject< ::Pixmap>(
		display,
		::XCreatePixmap(
			display,
			RootWindow(display.Get(), vi->screen),
			width,
			height,
			unsigned(vi->depth)
		),
		::XFreePixmap,
		"Error creating X Pixmap"
	){ }
};

} // namespace x11
} // namespace oglplus

#endif // include guard
