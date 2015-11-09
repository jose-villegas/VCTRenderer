/**
 *  @file oalplus/lib.hpp
 *  @brief All-in-one Include file for the separatelly-built library
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_LIB_1306012247_HPP
#define OALPLUS_LIB_1306012247_HPP

#ifndef OALPLUS_IMPLEMENTING_LIBRARY
#define OALPLUS_IMPLEMENTING_LIBRARY 1
#endif

#include <oalplus/config.hpp>
#include <oalplus/string.hpp>
#include <oalplus/fwd.hpp>

#include <oalplus/error/basic.hpp>
#include <oalplus/error/al.hpp>
#include <oalplus/error/object.hpp>
#include <oalplus/error/alut.hpp>

#include <oalplus/context.hpp>

#include <oalplus/detail/base_range.hpp>
#include <oalplus/detail/enum_class.hpp>

namespace oalplus {

// EnumValueName implementations
#include <oalplus/lib/enum_value_name.ipp>
// EnumValueRange implementations
#include <oalplus/lib/enum_value_range.ipp>

} // namespace oalplus

#undef OALPLUS_IMPLEMENTING_LIBRARY

#endif // include guard
