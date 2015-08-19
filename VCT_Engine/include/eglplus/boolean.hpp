/**
 *  @file eglplus/boolean.hpp
 *  @brief Wrapper for EGLboolean
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_BOOLEAN_1501311633_HPP
#define EGLPLUS_BOOLEAN_1501311633_HPP

#include <oglplus/detail/boolean.hpp>

namespace eglplus {

typedef oglplus::BoolImpl<EGLBoolean, EGLBoolean, EGL_TRUE, EGL_FALSE> Boolean;

} // namespace eglplus

#endif // include guard
