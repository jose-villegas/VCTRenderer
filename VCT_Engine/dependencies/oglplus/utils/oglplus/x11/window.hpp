/**
 *  @file oglplus/x11/window.hpp
 *  @brief Wrapper for X11 window class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_X11_WINDOW_1107121519_HPP
#define UTILS_OGLPLUS_X11_WINDOW_1107121519_HPP

#include <oglplus/x11/display.hpp>
#include <oglplus/x11/visual_info.hpp>
#include <oglplus/x11/color_map.hpp>

#include <X11/Xlib.h>
#include <stdexcept>
#include <cassert>

namespace oglplus {
namespace x11 {

class Window
 : public DisplayObject< ::Window>
{
private:
	static ::Window make_window(
		const Display& display,
		const VisualInfo& vi,
		const Colormap& cmap,
		unsigned width,
		unsigned height
	)
	{
		::XSetWindowAttributes swa;
		swa.colormap = cmap.Handle();
		swa.background_pixmap = None;
		swa.border_pixel = 0;
		swa.event_mask = StructureNotifyMask;
		//
		return ::XCreateWindow(
			display,
			RootWindow(display.Get(), vi->screen),
			0, 0, width, height,
			0,
			vi->depth,
			InputOutput,
			vi->visual,
			CWBorderPixel | CWColormap | CWEventMask,
			&swa
		);
	}
public:
	Window(
		const Display& display,
		const VisualInfo& vi,
		const Colormap& cmap,
		const char* title,
		unsigned width,
		unsigned height
	): DisplayObject< ::Window>(
		display,
	 	make_window(display, vi, cmap, width, height),
		::XDestroyWindow,
		"Error creating X Window"
	)
	{
		::XSizeHints size_hints;
		size_hints.width = int(width);
		size_hints.height = int(height);
		size_hints.flags = USSize;
		::XSetNormalHints(display, this->Handle(), &size_hints);

		::Atom wmDelete = ::XInternAtom(
			display,
			"WM_DELETE_WINDOW",
			True
		);
		::XSetWMProtocols(display, this->Handle(), &wmDelete, 1);

		::XStoreName(display, this->Handle(), title);
		::XMapWindow(display, this->Handle());
	}

	void SelectInput(long event_mask) const
	{
		::XSelectInput(
			this->DisplayRef(),
			this->Handle(),
			event_mask
		);
	}
};

} // namespace x11
} // namespace oglplus

#endif // include guard
