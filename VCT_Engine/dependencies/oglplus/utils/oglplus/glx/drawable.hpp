/**
 *  @file oglplus/glx/drawable.hpp
 *  @brief Wrapper for GLX drawables
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_GLX_DRAWABLE_1107121519_HPP
#define UTILS_OGLPLUS_GLX_DRAWABLE_1107121519_HPP

#include <oglplus/x11/display.hpp>

#include <GL/glx.h>
#include <stdexcept>

namespace oglplus {
namespace glx {

typedef x11::BaseDisplayObject< ::GLXDrawable> Drawable;

} // namespace glx
} // namespace oglplus

#endif // include guard
