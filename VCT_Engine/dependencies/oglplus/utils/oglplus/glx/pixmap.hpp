/**
 *  @file oglplus/glx/pixmap.hpp
 *  @brief Wrapper for GLX pixmaps
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_GLX_PIXMAP_1107121519_HPP
#define UTILS_OGLPLUS_GLX_PIXMAP_1107121519_HPP

#include <oglplus/x11/display.hpp>
#include <oglplus/x11/pixmap.hpp>
#include <oglplus/x11/visual_info.hpp>

#include <GL/glx.h>
#include <stdexcept>

namespace oglplus {
namespace glx {

class Pixmap
 : public x11::DisplayObject< ::GLXPixmap, void(::Display*, ::GLXPixmap)>
{
public:
	Pixmap(
		const x11::Display& display,
		const x11::VisualInfo& vi,
		const x11::Pixmap& pixmap
	): x11::DisplayObject< ::GLXPixmap, void(::Display*, ::GLXPixmap)>(
		display,
		::glXCreateGLXPixmap(display, vi, pixmap),
		::glXDestroyGLXPixmap,
		"Error creating glX Pixmap"
	){ }
};

} // namespace glx
} // namespace oglplus

#endif // include guard
