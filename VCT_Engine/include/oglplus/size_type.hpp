/**
 *  @file oglplus/size_type.hpp
 *  @brief Wrapper for GLsizei and GLsizeiptr
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_SIZE_TYPE_1501311633_HPP
#define OGLPLUS_SIZE_TYPE_1501311633_HPP

#include <oglplus/detail/size.hpp>

namespace oglplus {

typedef SizeImpl<GLsizei> SizeType;
typedef SizeImpl<GLsizeiptr> BigSizeType;

} // namespace oglplus

#endif // include guard
