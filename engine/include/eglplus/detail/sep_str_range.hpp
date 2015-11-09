/**
 *  .file eglplus/detail/sep_str_range.hpp
 *  .brief Range for traversal of separated string values
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_AUX_SEP_STR_RANGE_1305291314_HPP
#define EGLPLUS_AUX_SEP_STR_RANGE_1305291314_HPP

#include <eglplus/string.hpp>
#include <oglplus/detail/sep_str_range.hpp>

namespace eglplus {
namespace aux {

typedef oglplus::aux::SepStrRangeTpl<String> SepStrRange;

} // namespace aux
} // namespace eglplus

#endif // include guard
