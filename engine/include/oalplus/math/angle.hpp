/**
 *  @file oalplus/angle.hpp
 *  @brief Implementation of angle.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_ANGLE_1303201602_HPP
#define OALPLUS_ANGLE_1303201602_HPP

#include <oalplus/config.hpp>
#include <oglplus/math/angle.hpp>

namespace oalplus {

/// Reused Angle<T> template from OGLplus
using oglplus::Angle;
/// Instantiation of Angle using float as data type
typedef Angle<ALfloat> Anglef;

/// Reused the Radians Angle constructor from OGLplus
using oglplus::Radians;
/// Reused the Degrees Angle constructor from OGLplus
using oglplus::Degrees;
/// Reused the FullCircle Angle constructor from OGLplus
using oglplus::FullCircle;
/// Reused the FullCircles Angle constructor from OGLplus
using oglplus::FullCircles;
/// Reused the RightAngle Angle constructor from OGLplus
using oglplus::RightAngle;
/// Reused the RightAngles Angle constructor from OGLplus
using oglplus::RightAngles;
/// Reused the ArcSin Angle constructor from OGLplus
using oglplus::ArcSin;
/// Reused the ArcCos Angle constructor from OGLplus
using oglplus::ArcCos;
/// Reused the ArcTan Angle constructor from OGLplus
using oglplus::ArcTan;

} // namespace oalplus

#endif // include guard
