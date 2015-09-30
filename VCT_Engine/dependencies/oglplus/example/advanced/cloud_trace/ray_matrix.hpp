/**
 *  @file advanced/cloud_trace/ray_matrix.hpp
 *  @brief Declares ray projection matrix constructor function
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_RAYMATRIX_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_RAYMATRIX_1119071146_HPP

#include "app_data.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>
#include <oglplus/math/matrix.hpp>

namespace oglplus {
namespace cloud_trace {

extern Mat4f RayMatrix(const AppData&, unsigned face);

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
