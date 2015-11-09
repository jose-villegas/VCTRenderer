/**
 *  @file eglplus/eglfunc.hpp
 *  @brief Helper macro expanding into EGL function name
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_EGLFUNC_1305291005_HPP
#define EGLPLUS_EGLFUNC_1305291005_HPP

#include <eglplus/config.hpp>

namespace eglplus {

#define EGLPLUS_EGLFUNC(FUNCNAME) \
	::egl ## FUNCNAME

} // namespace eglplus

#endif // include guard
