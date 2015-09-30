/**
 *  @file oalplus/opt/smart_enums.hpp
 *  @brief Implements syntax sugar for shortening enumerated values
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_OPT_SMART_ENUMS_1204051600_HPP
#define OALPLUS_OPT_SMART_ENUMS_1204051600_HPP

#include <oalplus/config.hpp>

namespace oalplus {
namespace smart_enums {

#if !OGLPLUS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
# include <oalplus/detail/smart_enums.ipp>
#else
# error "Smart enumerations require support for function template default args!"
#endif

} // namespace smart_enums

namespace smart_values
{
#if !OGLPLUS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
# include <oalplus/detail/smart_values.ipp>
#endif
} // namespace smart_values
} // namespace oalplus

#endif // include guard
