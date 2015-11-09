/**
 *  @file oalplus/vector.hpp
 *  @brief Implementation of a N-dimensional vector
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_VECTOR_1303201602_HPP
#define OALPLUS_VECTOR_1303201602_HPP

#include <oalplus/config.hpp>
#include <oglplus/math/vector.hpp>

namespace oalplus {

/// Reused Vector<T, N> template from OGLplus
using oglplus::Vector;
/// 3D Vector class
typedef Vector<ALfloat, 3> Vec3f;

} // namespace oalplus

#endif // include guard
