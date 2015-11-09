/**
 *  @file oalplus/curve.hpp
 *  @brief Implementation rational curves
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_CURVE_1303201602_HPP
#define OALPLUS_CURVE_1303201602_HPP

#include <oalplus/config.hpp>
#include <oglplus/math/curve.hpp>

namespace oalplus {

/// Reused sequence of BezierCurves from OGLplus
using oglplus::BezierCurves;
/// Reused CubicBezierLoop from OGLplus
using oglplus::CubicBezierLoop;

} // namespace oalplus

#endif // include guard
